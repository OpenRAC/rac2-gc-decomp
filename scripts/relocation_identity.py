"""Conservative EE relocation templates, never semantic/source equivalence.

Only recorded, mapped address fields change. Every result is reconstructed
exactly; unproved fields, literal values, register operands and tail NOPs stay
raw. Templates are private bytes; public records contain addresses/evidence.
"""
from __future__ import annotations

from dataclasses import dataclass, replace
import hashlib
import json
from pathlib import Path
import re
import struct
from collections import deque

HASH = re.compile(r'[0-9a-f]{64}\Z')
# Opcode -> (load/store, GPR destination/value, accessed width). COP1/COP2
# memory opcodes use GPR bases but retain all FPR/VF operand bits.
MEMORY = {0x20:('load',True,1),0x21:('load',True,2),0x22:('load',True,1),
          0x23:('load',True,4),0x24:('load',True,1),0x25:('load',True,2),
          0x26:('load',True,1),0x27:('load',True,4),0x1a:('load',True,1),
          0x1b:('load',True,1),0x1e:('load',True,16),0x37:('load',True,8),
          0x28:('store',True,1),0x29:('store',True,2),0x2a:('store',True,1),
          0x2b:('store',True,4),0x2c:('store',True,1),0x2d:('store',True,1),
          0x2e:('store',True,1),0x1f:('store',True,16),0x3f:('store',True,8),
          0x31:('load',False,4),0x35:('load',False,8),0x36:('load',False,16),
          0x39:('store',False,4),0x3d:('store',False,8),0x3e:('store',False,16)}


def sha(data):
    return hashlib.sha256(data).hexdigest()


NORMALIZER_SHA256 = sha(Path(__file__).read_bytes())


def signed16(value):
    return value-0x10000 if value & 0x8000 else value


@dataclass(frozen=True)
class PreparedContext:
    program: str
    reference_sha256: str
    sections: tuple
    entries: frozenset
    gp: int | None
    gp_evidence: str
    symbols: tuple
    roles: dict
    gp_call_preserved: bool
    pointer_roles: dict | None = None


def prepare_context(context):
    """Validate once per pinned program, then reuse for every complete body."""
    if isinstance(context,PreparedContext):
        return context
    if (not isinstance(context,dict) or not isinstance(context.get('program'),str)
            or not context['program'] or not HASH.fullmatch(context.get('reference_sha256',''))):
        raise ValueError('Context needs its pinned program and reference SHA-256')
    sections=[]
    for section in context.get('sections',[]):
        address=section.get('address');size=section.get('size');flags=section.get('flags');kind=section.get('type',1)
        if (any(type(v) is not int for v in (address,size,flags,kind)) or address<0 or size<=0
                or address+size>0x100000000 or flags<0):
            raise ValueError('Invalid mapped section')
        sections.append((address,size,flags,kind,section.get('name','mapped section')))
    entries=context.get('function_entries',[])
    if any(type(entry) is not int or entry<0 or entry%4 for entry in entries):
        raise ValueError('Invalid complete/flow-supported code entry')
    verified=context.get('gp_verified',False)
    if type(verified) is not bool:
        raise ValueError('GP verification must be explicit boolean evidence')
    gp=context.get('gp') if verified else None
    evidence=context.get('gp_evidence','')
    if verified and (type(gp) is not int or gp<0 or gp>=0x02000000 or gp%4
                     or not isinstance(evidence,str) or not evidence.strip()):
        raise ValueError('Verified GP needs a real retail value and initialization evidence')
    symbols=[]
    for item in context.get('data_symbols',[]):
        a,s=item.get('address'),item.get('size',1)
        if type(a) is not int or type(s) is not int or a<0 or s<=0:
            raise ValueError('Invalid data allocation/symbol')
        symbols.append((a,s,item.get('role','')))
    roles={}
    for target,role in context.get('role_bindings',{}).items():
        target=int(target,0) if isinstance(target,str) else target
        if type(target) is not int or not isinstance(role,str) or not role.strip():
            raise ValueError('Invalid qualified role binding')
        roles[target]=role
    preserved=context.get('gp_call_preserved',False)
    if type(preserved) is not bool:
        raise ValueError('GP call-preservation evidence must be explicit')
    if preserved and (not isinstance(context.get('gp_call_preservation_evidence'),str)
                      or not context['gp_call_preservation_evidence'].strip()):
        raise ValueError('GP call preservation needs its own machine theorem evidence')
    prepared=PreparedContext(context['program'],context['reference_sha256'],tuple(sections),
                             frozenset(entries),gp,evidence,tuple(symbols),roles,preserved,{})
    pins=context.get('function_pins',{})
    for proof in context.get('pointer_argument_roles',[]):
        callee=proof.get('callee_address');register=proof.get('argument_register')
        size=proof.get('callee_size');offset=proof.get('dereference_instruction_offset')
        addend=proof.get('dereference_offset');width=proof.get('width')
        pin=pins.get(callee,pins.get(str(callee),pins.get(hex(callee) if type(callee) is int else '',{})))
        if (any(type(v) is not int for v in (callee,register,size,offset,addend,width))
                or not 4<=register<=11 or callee not in prepared.entries or size<=0 or size%4
                or offset<0 or offset%4 or offset>=size or width not in (1,2,4,8,16)
                or not -(1<<31)<=addend<(1<<31)
                or proof.get('program')!=prepared.program
                or proof.get('reference_sha256')!=prepared.reference_sha256
                or proof.get('proof_status')!='verified_incoming_dereference'
                or proof.get('boundary_tier') not in ('qualified_complete','flow_supported_inferred')
                or not HASH.fullmatch(proof.get('callee_raw_sha256',''))
                or not HASH.fullmatch(proof.get('proof_decoder_sha256',''))
                or not HASH.fullmatch(proof.get('decoder_dependency_sha256',''))
                or not isinstance(proof.get('evidence'),str) or not proof['evidence'].strip()
                or proof.get('action') not in ('load','store')
                or _mapped(prepared,callee,size,code=True) is None
                or pin.get('size')!=size or pin.get('raw_sha256')!=proof['callee_raw_sha256']
                or pin.get('boundary_tier')!=proof['boundary_tier']):
            raise ValueError('Incoming-pointer evidence lacks matching current callee/reference/decoder/boundary pins')
        key=(callee,register)
        fields=('callee_address','argument_register','callee_size','callee_raw_sha256','boundary_tier',
                'dereference_instruction_offset','dereference_offset','width','action','evidence',
                'proof_decoder_sha256','decoder_dependency_sha256')
        prepared.pointer_roles.setdefault(key,[]).append({field:proof[field] for field in fields})
    return prepared


def _mapped(context,target,width=1,code=False):
    # EE main RAM only. MMIO, scratchpad, null and unallocated numeric
    # constants do not gain address identity from their integer value.
    if not 0<target<0x02000000 or target+width>0x02000000:
        return None
    for base,size,flags,kind,name in context.sections:
        if (flags&2 and bool(flags&4)==code and kind in (1,8)
                and (not code or 'vutext' not in name.lower())
                and base<=target and target+width<=base+size):
            return name
    return None


def _data_symbol(context,target):
    return next((item for item in context.symbols if item[0]==target),None)


@dataclass(frozen=True)
class Expr:
    root: int
    value: int
    high: int | None
    low: int | None
    mode: str
    anchor: int


def _origin(value):
    return value.root if value is not None and value.root>=0 else None


def _control(word,index,address,count):
    op=word>>26;rs=(word>>21)&31;rt=(word>>16)&31;fn=word&63
    if op in (2,3):
        target=((address+index*4+4)&0xf0000000)|((word&0x03ffffff)<<2)
        return ('call' if op==3 else 'jump',target,False)
    if op in (4,5,6,7,20,21,22,23) or op==1 or (op in (0x11,0x12) and rs==8):
        # Only branch REGIMM encodings; traps are not branches.
        if op==1 and rt not in (0,1,2,3,16,17,18,19):
            return None
        target=address+index*4+4+signed16(word&0xffff)*4
        likely=op in (20,21,22,23) or (op==1 and rt in (2,3,18,19)) or (op in (0x11,0x12) and rt&2)
        return ('branch',target,bool(likely))
    if op==0 and fn in (8,9):
        return ('return' if fn==8 and rs==31 else 'indirect-call' if fn==9 else 'indirect-jump',None,False)
    return None


def _cfg(words,address):
    count=len(words);edges={i:[i+1] if i+1<count else [] for i in range(count)}
    controls={};delays={}
    for i,word in enumerate(words):
        if word>>26==1 and (word>>16)&31 in (16,17,18,19):
            raise ValueError('REGIMM link/local-return flow is not certified')
        control=_control(word,i,address,count)
        if not control:
            continue
        if i+1>=count or _control(words[i+1],i+1,address,count):
            raise ValueError('Incomplete or control-transfer delay slot')
        controls[i]=control;delays[i+1]=(i,control)
        kind,target,likely=control
        internal=target is not None and address<=target<address+count*4 and target%4==0
        dest=(target-address)//4 if internal else None
        fall=i+2 if i+2<count else None
        edges[i]=[i+1]+([fall] if likely and fall is not None else [])
        if kind in ('call','indirect-call'):
            edges[i+1]=[fall] if fall is not None else []
        elif kind=='branch':
            edges[i+1]=([dest] if dest is not None else [])+([fall] if not likely and fall is not None else [])
        elif kind=='jump':
            edges[i+1]=[dest] if dest is not None else []
        else:
            edges[i+1]=[]
    for delay,(owner,_control_info) in delays.items():
        if any(delay in targets and predecessor!=owner for predecessor,targets in edges.items()):
            raise ValueError('Control flow enters an unrelated delay slot')
    return edges,controls,delays


def _uses_defs(word,delay,context):
    op=word>>26;rs=(word>>21)&31;rt=(word>>16)&31;rd=(word>>11)&31;fn=word&63
    reads,writes=set(),set()
    if op==0x0f:writes.add(rt)
    elif op in (8,9,0xa,0xb,0xc,0xd,0xe,0x18,0x19):reads.add(rs);writes.add(rt)
    elif op in MEMORY:
        action,gpr,_width=MEMORY[op];reads.add(rs)
        if gpr:(writes if action=='load' else reads).add(rt)
    elif op in (2,3):
        if op==3:writes.add(31)
    elif op in (4,5,6,7,20,21,22,23,1):
        reads.add(rs)
        if op in (4,5,20,21):reads.add(rt)
        if op==1 and rt in (16,17,18,19):writes.add(31)
    elif op in (0x11,0x12):
        if rs in (0,1,2):writes.add(rt)
        elif rs in (4,5,6):reads.add(rt)
    elif op==0:
        if word==0:pass
        elif fn in (0x10,0x12):writes.add(rd)
        elif fn in (0x11,0x13,8):reads.add(rs)
        elif fn==9:reads.add(rs);writes.add(rd)
        elif fn in (0x0c,0x0d):reads.update(range(1,32));writes.update(range(1,32))
        elif fn==0x0f:pass
        elif fn in (0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f):reads.update((rs,rt))
        else:reads.update((rs,rt));writes.add(rd)
    else:reads.update(range(1,32));writes.update(range(1,32))
    if delay:
        kind=delay[1][0]
        if kind in ('call','indirect-call'):
            reads.update(range(4,12));writes.update(set(range(1,32))-{29})
            if context.gp_call_preserved:writes.discard(28)
        elif kind in ('return','jump','indirect-jump','external-branch'):
            reads.update((2,3))
            if kind!='return':reads.update(range(4,12))
    return reads-{0},writes-{0}


def _execute(words,index,state,delay,context,record=None,address=0):
    regs=list(state[0]);stack=dict(state[1]);word=words[index]
    op=word>>26;rs=(word>>21)&31;rt=(word>>16)&31;rd=(word>>11)&31;fn=word&63
    immediate=word&0xffff;simm=signed16(immediate);source=regs[rs]
    def use(value,reason):
        root=_origin(value)
        if record is not None and root is not None:
            record['blocked'].setdefault(root,set()).add(f'+0x{index*4:x}: {reason}')
    def write(register,value=None):
        if register:
            regs[register]=value
    if op==0x0f:
        write(rt,Expr(index,(immediate<<16)&0xffffffff,index,None,'high',(immediate<<16)&0xffffffff))
    elif op in (9,0x19,0x0d):
        if source is not None and source.mode=='sp' and op in (9,0x19):
            write(rt,replace(source,value=source.value+simm))
        elif source is not None and source.root>=0:
            if (source.mode=='high' and op in (9,0x19)
                    and _data_symbol(context,source.value) and _mapped(context,source.value)):
                # The LUI already establishes an exact aligned global base.
                # A following addend is its field offset, not a LO relocation.
                write(rt,replace(source,value=(source.value+simm)&0xffffffff,mode='none',anchor=source.value))
            elif source.mode=='high':
                target=(source.value|immediate) if op==0x0d else (source.value+simm)&0xffffffff
                write(rt,replace(source,value=target,low=index,mode={9:'addiu',0x19:'daddiu',0x0d:'ori'}[op],anchor=target))
            elif op in (9,0x19):
                # Later arithmetic is a structure/index offset, never a LO relocation.
                write(rt,replace(source,value=(source.value+simm)&0xffffffff))
            else:
                use(source,'non-address ORI after completed value');write(rt)
        else:
            write(rt)
    elif op in MEMORY:
        action,gpr,width=MEMORY[op]
        target=None if source is None else (source.value+simm)&0xffffffff
        if source is not None and source.mode=='sp':
            slot=source.value+simm
            if action=='store' and gpr:
                use(regs[rt],'stack-stored value lacks qualified pointer ownership');stack={}
            elif action=='load' and gpr:
                write(rt)
        else:
            if record is not None:
                root=_origin(source)
                if rs==28 and (source is None or source.mode!='gp'):
                    record['unresolved'].append({'offset':index*4,'reason':'Real GP value/lifetime is unproved at this instruction','retained':'raw'})
                if source is not None and source.mode=='gp':
                    name=_mapped(context,target,width)
                    if name:
                        record['gp'].append({'offset':index*4,'kind':'gp16','target':target,'gp':context.gp,
                            'role':context.roles.get(target,f'gp-memory-slot:{index*4:x}'),
                            'evidence':f'Opcode 0x{op:x} GPR-base memory; mapped {name}; {context.gp_evidence}'})
                    else:
                        record['unresolved'].append({'offset':index*4,'reason':'GP target is not mapped EE data','retained':'raw'})
                elif root is not None:
                    name=_mapped(context,target,width)
                    if not name:
                        use(source,'memory target is not mapped EE data')
                    else:
                        record['memory'].setdefault(root,[]).append((index,source,target,name,width))
            if action=='store' and gpr:
                use(regs[rt],'value stored to non-stack memory without qualified pointer type')
            if action=='load' and gpr:
                write(rt)
    elif op==0:
        if fn in (0x21,0x2d,0x25) and (rs==0 or rt==0):
            write(rd,regs[rt] if rs==0 else regs[rs])
        elif fn in (8,9):
            if rs!=31:
                use(regs[rs],'indirect control target not a qualified pointer role')
            if fn==9:
                write(rd)
        elif word==0:
            pass
        elif fn in (0x10,0x12):
            write(rd)
        elif fn in (0x11,0x13):
            use(regs[rs],'HI/LO scalar operand')
        elif fn in (0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f):
            use(regs[rs],'multiply/divide scalar operand');use(regs[rt],'multiply/divide scalar operand')
        elif fn in (0x0c,0x0d,0x0f):
            # Syscall/break/sync cannot certify escaped register values.
            if fn in (0x0c,0x0d):
                for value in regs:use(value,'syscall/break register escape')
                regs=[None]*32;stack={}
        else:
            use(regs[rs],'unqualified scalar/shift/register operation');use(regs[rt],'unqualified scalar/shift/register operation');write(rd)
    elif op in (0x11,0x12):
        # Macro COP2 VF operations do not read GPRs: rs=28 is not $gp.
        if rs in (0,1,2):
            write(rt)
        elif rs in (4,5,6):
            use(regs[rt],'GPR transferred to COP1/COP2 scalar/vector operand')
    elif op in (2,3):
        if op==3:write(31)
    elif op in (4,5,6,7,20,21,22,23,1):
        use(regs[rs],'branch compares unqualified scalar/address value')
        if op in (4,5,20,21):use(regs[rt],'branch compares unqualified scalar/address value')
        if op==1 and rt in (16,17,18,19):write(31)
    elif op in (8,0xa,0xb,0xc,0xe,0x18):
        use(source,'unqualified scalar immediate operation');write(rt)
    else:
        # Unsupported MMI/COP0/cache/atomic encodings must not leave a
        # possibly overwritten GPR available to a later address proof.
        for value in regs:use(value,'unsupported instruction may read/write GPR')
        regs=[None]*32;stack={}
    if delay is not None:
        _owner,(kind,_target,_likely)=delay
        if kind in ('call','indirect-call'):
            callee=None
            if kind=='call':
                callee=((address+_owner*4+4)&0xf0000000)|((words[_owner]&0x03ffffff)<<2)
            for register in range(4,12):
                value=regs[register];proofs=(context.pointer_roles or {}).get((callee,register),[])
                valid=bool(proofs and value is not None and _origin(value) is not None)
                if valid:
                    for proof in proofs:
                        effective=(value.value+proof['dereference_offset'])&0xffffffff
                        if _mapped(context,effective,proof['width']) is None:
                            valid=False;break
                if valid and record is not None:
                    root=_origin(value)
                    for proof in proofs:
                        effective=(value.value+proof['dereference_offset'])&0xffffffff
                        name=_mapped(context,effective,proof['width'])
                        record['memory'].setdefault(root,[]).append((index,value,effective,name,proof['width'],proof))
                elif not valid:
                    use(value,'call argument without verified incoming CPU-address use')
            for value in stack.values():use(value,'stack value may escape to called function')
            # Callee machine preservation has not been proved merely by
            # seeing a JAL. Do not borrow a C ABI assumption for nested ASM.
            for register in set(range(1,32))-{29,28}:
                write(register)
            if not context.gp_call_preserved:write(28)
        elif kind in ('return','jump','indirect-jump','external-branch'):
            # Internal jumps do not return: the caller passes a false
            # delay marker for those in _analyze below.
            use(regs[2],'return value without qualified pointer ABI');use(regs[3],'return value without qualified pointer ABI')
            if kind!='return':
                for register in range(4,12):use(regs[register],'tail-transfer argument without qualified pointer ABI')
    regs[0]=None
    return tuple(regs),tuple(sorted(stack.items()))


def _merge(states,record=None,index=0,live=None):
    if len(states)==1:return states[0]
    regs=[]
    for register in range(32):
        values=[state[0][register] for state in states]
        if all(value==values[0] for value in values):regs.append(values[0])
        else:
            regs.append(None)
            if record is not None and (live is None or register in live):
                for value in values:
                    root=_origin(value)
                    if root is not None:record['blocked'].setdefault(root,set()).add(f'+0x{index*4:x}: unresolved CFG join')
    stacks=[dict(state[1]) for state in states];stack={}
    for key in set().union(*(s.keys() for s in stacks)):
        values=[s.get(key) for s in stacks]
        if all(v==values[0] for v in values) and values[0] is not None:stack[key]=values[0]
        elif record is not None:
            for value in values:
                root=_origin(value)
                if root is not None:record['blocked'].setdefault(root,set()).add(f'+0x{index*4:x}: unresolved stack CFG join')
    return tuple(regs),tuple(sorted(stack.items()))


def _analyze(words,address,context):
    edges,controls,delays=_cfg(words,address)
    predecessors={i:[] for i in range(len(words))}
    for origin,targets in edges.items():
        for target in targets:predecessors[target].append(origin)
    initial=[None]*32;initial[29]=Expr(-2,0,None,None,'sp',0)
    if context.gp is not None:initial[28]=Expr(-1,context.gp,None,None,'gp',context.gp)
    entry=(tuple(initial),())
    incoming={};outgoing={};queue=deque([0]);queued={0};iterations=0
    def delay_at(index):
        delay=delays.get(index)
        if delay and delay[1][0]=='jump' and edges[index]:return None
        if delay and delay[1][0]=='branch' and not address<=delay[1][1]<address+len(words)*4:
            return delay[0],('external-branch',delay[1][1],delay[1][2])
        return delay
    # Backward liveness prevents a dead, overwritten join register from
    # disqualifying an otherwise proved earlier address construction.
    live=[set() for _ in words];changing=True
    while changing:
        changing=False
        for index in range(len(words)-1,-1,-1):
            reads,writes=_uses_defs(words[index],delay_at(index),context)
            after=set().union(*(live[target] for target in edges[index]))
            before=reads|(after-writes)
            if before!=live[index]:live[index]=before;changing=True
    while queue:
        index=queue.popleft();queued.discard(index);iterations+=1
        if iterations>max(128,len(words)*64):raise ValueError('CFG did not reach a bounded fixed point')
        candidates=[outgoing[p] for p in predecessors[index] if p in outgoing]
        if index==0:candidates.append(entry)
        if not candidates:continue
        state=_merge(candidates);incoming[index]=state
        result=_execute(words,index,state,delay_at(index),context,address=address)
        if outgoing.get(index)==result:continue
        outgoing[index]=result
        for target in edges[index]:
            if target not in queued:queue.append(target);queued.add(target)
    record={'blocked':{},'memory':{},'gp':[],'unresolved':[]}
    for index,state in incoming.items():
        candidates=[outgoing[p] for p in predecessors[index] if p in outgoing]
        if index==0:candidates.append(entry)
        _merge(candidates,record,index,live[index])
        _execute(words,index,state,delay_at(index),context,record,address)
        if not edges[index] and index not in delays:
            for value in state[0][2:4]:
                root=_origin(value)
                if root is not None:record['blocked'].setdefault(root,set()).add('body end exposes unqualified return value')
    return record,controls,set(incoming)


def _field_parts(relocation,address):
    kind=relocation['kind'];target=relocation['target']
    if kind=='j26':
        return [(relocation['offset'],0x03ffffff,(target>>2)&0x03ffffff)]
    if kind=='gp16':
        delta=target-relocation['gp']
        if not -32768<=delta<=32767:raise ValueError('GP relocation is out of signed range')
        return [(relocation['offset'],0xffff,delta&0xffff)]
    mode=relocation['lo_mode'];low=relocation['low_offset']
    high=((target+0x8000)>>16)&0xffff if mode in ('addiu','daddiu','memory') else (target>>16)&0xffff
    parts=[(relocation['high_offset'],0xffff,high)]
    if low is not None:parts.append((low,0xffff,target&0xffff))
    return parts


def reconstruct(template,address,relocations):
    """Reapply address-derived fields; overlapping pairs must agree exactly."""
    result=bytearray(template);written={}
    for relocation in relocations:
        for offset,mask,value in _field_parts(relocation,address):
            if offset%4 or not 0<=offset<=len(result)-4:raise ValueError('Relocation field outside complete body')
            key=(offset,mask)
            if key in written and written[key]!=value:raise ValueError('Conflicting paired address fields')
            written[key]=value;word=struct.unpack_from('<I',result,offset)[0]
            struct.pack_into('<I',result,offset,(word&~mask)|(value&mask))
    return bytes(result)


def normalize(body,address,context):
    context=prepare_context(context)
    if (not isinstance(body,bytes) or not body or len(body)%4 or type(address) is not int or address%4
            or _mapped(context,address,len(body),code=True) is None):
        raise ValueError('Complete body must lie in mapped executable EE bytes')
    words=struct.unpack('<'+'I'*(len(body)//4),body)
    unresolved=[];relocations=[]
    try:record,controls,reachable=_analyze(words,address,context)
    except ValueError as error:
        record={'blocked':{},'memory':{},'gp':[],'unresolved':[]};controls={};reachable=set()
        unresolved.append({'offset':0,'reason':str(error),'retained':'raw'})
    aliases={}
    for index,(kind,target,_likely) in sorted(controls.items()):
        if index not in reachable or kind not in ('call','jump'):continue
        internal=address<=target<address+len(body) and target%4==0
        name=_mapped(context,target,4,code=True)
        if not internal and (name is None or target not in context.entries):
            unresolved.append({'offset':index*4,'reason':'J/JAL target lacks mapped supported code entry','retained':'raw'});continue
        if internal:
            relative=target-address;role=f'internal-{kind}:+0x{relative:x}'
        else:
            alias=aliases.setdefault(target,len(aliases));role=context.roles.get(target,f'external-{kind}-callee{alias}')
        relocation={'offset':index*4,'kind':'j26','target':target,'role':role,'internal':internal,
                    'evidence':'Opcode J/JAL target is aligned internal code' if internal else f'Opcode J/JAL target is mapped {name} and a supported function entry; positional call role preserves target aliases'}
        if internal:relocation['relative_target']=relative
        relocations.append(relocation)
    gp_aliases={}
    for relocation in record['gp']:
        target=relocation['target'];alias=gp_aliases.setdefault(target,len(gp_aliases))
        if target not in context.roles:relocation['role']+=f':data-alias{alias}'
        relocations.append(relocation)
    data_aliases={}
    for root,uses in sorted(record['memory'].items()):
        if root in record['blocked']:
            unresolved.append({'offset':root*4,'reason':'; '.join(sorted(record['blocked'][root])),'retained':'raw'});continue
        proposals=[]
        proved_aligned_base=any(len(use)>5 and use[1].mode=='high' and _mapped(context,use[1].value)
                               for use in uses)
        for witnessed in uses:
            index,expr,target,name,width=witnessed[:5]
            pointer_proof=witnessed[5] if len(witnessed)>5 else None
            mode=expr.mode;anchor=expr.anchor;low=expr.low
            if mode=='high':
                # A bare LUI base with a small memory offset can be a
                # structure base: only its high address is normalized,
                # and only if the exact base has a mapped data symbol.
                if (_data_symbol(context,expr.value) or proved_aligned_base) and _mapped(context,expr.value):
                    mode='none';anchor=expr.value;low=None
                # Direct HI + memory LO needs an exact effective symbol.
                elif _data_symbol(context,target):
                    mode='memory';anchor=target;low=index
                else:
                    unresolved.append({'offset':root*4,'reason':'Bare LUI memory base/LO has no precise data symbol; structure offsets retained','retained':'raw'});proposals=[];break
            elif not _mapped(context,anchor):
                unresolved.append({'offset':root*4,'reason':'Completed address anchor is not mapped EE data','retained':'raw'});proposals=[];break
            alias=data_aliases.setdefault(anchor,len(data_aliases))
            role=context.roles.get(anchor,f'data-address-root:{root*4:x}:data-alias{alias}')
            if target!=anchor:role+=f':field-addend:{target-anchor}'
            relocation={'offset':index*4 if low is None else low*4,'kind':'hi16_lo16',
                        'high_offset':expr.high*4,'low_offset':None if low is None else low*4,
                        'target':anchor,'lo_mode':mode,'role':role,
                        'evidence':f'Forward GPR def-use/CFG fixed point: only mapped memory address uses; {name}; later structure offsets remain raw'}
            if pointer_proof:
                relocation['pointer_evidence']={key:pointer_proof[key] for key in
                    ('callee_address','argument_register','callee_size','callee_raw_sha256',
                     'dereference_instruction_offset','dereference_offset','width',
                     'proof_decoder_sha256','decoder_dependency_sha256')}
                relocation['role']+=f":incoming-r{pointer_proof['argument_register']}:dereference-addend:{pointer_proof['dereference_offset']}:width:{pointer_proof['width']}"
                relocation['evidence']+='; pinned complete callee incoming-register dereference theorem: '+pointer_proof['evidence']
            proposals.append(relocation)
        # Verify architectural signed carry and every overlapping original
        # field before accepting any member of this address root.
        if proposals:
            try:
                for relocation in proposals:
                    for offset,mask,value in _field_parts(relocation,address):
                        if words[offset//4]&mask != value:raise ValueError('HI/LO address relation is not architectural')
                # Shared high halves must agree across all actual LO uses.
                reconstruct(body,address,proposals)
            except ValueError as error:
                unresolved.append({'offset':root*4,'reason':str(error),'retained':'raw'})
            else:relocations.extend(proposals)
    # Report address-like origins consumed as scalar/escaped values even
    # when no mapped memory use was seen. They are never normalized.
    for root,reasons in sorted(record['blocked'].items()):
        if root not in record['memory']:
            unresolved.append({'offset':root*4,'reason':'; '.join(sorted(reasons)),'retained':'raw'})
    unresolved.extend(record['unresolved'])
    unique={json.dumps(item,sort_keys=True):item for item in relocations}
    relocations=sorted(unique.values(),key=lambda r:(r['offset'],r['kind'],r.get('high_offset',-1)))
    template=bytearray(body)
    for item in relocations:
        for offset,mask,_value in _field_parts(item,address):
            word=struct.unpack_from('<I',template,offset)[0]
            replacement=(item['relative_target']>>2) if item['kind']=='j26' and item['internal'] else 0
            struct.pack_into('<I',template,offset,(word&~mask)|(replacement&mask))
    template=bytes(template)
    rebuilt=reconstruct(template,address,relocations)
    if rebuilt!=body:raise ValueError('Relocation certificate failed exact body reconstruction')
    # Include field/role schema so an untouched literal zero does not
    # collide with a normalized address, and internal CFG/alias patterns
    # remain part of identity even though absolute targets differ.
    schema=[{k:v for k,v in item.items() if k in ('offset','kind','high_offset','low_offset','lo_mode','role','internal','relative_target')}
            for item in relocations]
    signature=sha(b'ee-relocation-template-v1\0'+template+json.dumps(schema,sort_keys=True,separators=(',',':')).encode())
    return {'template':template,'signature_sha256':signature,'relocations':relocations,
            'unresolved':unresolved,'proof_status':'verified_relocation_template' if relocations else 'raw_identity',
            'certificate':{'raw_sha256':sha(body),'normalized_sha256':sha(template),
                           'reconstructed_sha256':sha(rebuilt),'normalizer_sha256':NORMALIZER_SHA256,'exact':True}}

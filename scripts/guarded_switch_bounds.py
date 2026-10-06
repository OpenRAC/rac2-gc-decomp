"""Narrow static switch-table replay; never original-boundary/runtime certification."""
from collections import deque
import hashlib
import struct
import rabbitizer


def _writes(word,address):
    ins=rabbitizer.Instruction(word,address,rabbitizer.InstrCategory.R5900)
    if not ins.isValid():return None
    return {getattr(ins,k.lower()).value for k in ("Rs","Rt","Rd") if getattr(ins,"modifies"+k)()}


def cfg_replay(address,size,controls,tables=None,skip_node=None,skip_edge=None):
    """Internal targets are expanded; outside flow and unknown transfers persist."""
    tables=tables or {};end=address+size;queue=[address];seen=set();visited=set();escapes=set();unknown=set();missing=set();exits=[]
    while queue:
        pc=queue.pop()
        if pc==skip_node or pc in visited:continue
        if not address<=pc<end:escapes.add(pc);continue
        visited.add(pc);seen.add(pc);c=controls.get(pc)
        if not c:
            nexts=[pc+4]
        else:
            if c['delay']:
                slot=pc+4
                if slot>=end:missing.add(slot)
                else:
                    seen.add(slot)
                    if slot in controls:unknown.add(slot)
            after=pc+8 if c['delay'] else pc+4
            kind=c['kind'];target=c.get('target');nexts=[]
            if kind=='return':exits.append(pc)
            elif kind=='call':nexts=[after]
            elif kind=='indirect_jump':
                if pc in tables:nexts=tables[pc]
                else:unknown.add(pc)
            elif kind in ('branch','jump'):
                nexts=[target]
                if kind=='branch' and c.get('conditional'):nexts.append(after)
        for nxt in nexts:
            if skip_edge!=(pc,nxt):queue.append(nxt)
    return {'reachable':seen,'escapes':sorted(escapes),'unknown':sorted(unknown),'missing_delay':sorted(missing),'returns':sorted(set(exits))}


def candidate(raw,address,jump,read,sections):
    """Recognize ONLY the observed LUI/SLL/ADDIU/ADDU/LW/JR sequence +bound guard."""
    words=list(struct.unpack('<'+'I'*(len(raw)//4),raw));index=(jump-address)//4
    if index<5 or index+1>=len(words):raise ValueError('lookup window or delay incomplete')
    a,b,c,d,e,j=words[index-5:index+1]
    op=lambda w:w>>26
    rs=lambda w:(w>>21)&31
    rt=lambda w:(w>>16)&31
    rd=lambda w:(w>>11)&31
    if op(a)!=15 or op(c)!=9 or rs(c)!=rt(a) or rt(c)!=rt(a):raise ValueError('not canonical literal table base')
    if op(b)!=0 or b&63!=0 or (b>>6)&31!=2 or rs(b)!=0:raise ValueError('not word index scale')
    base,selector,scaled=rt(a),rt(b),rd(b)
    if op(d)!=0 or d&63!=0x21 or rd(d)!=scaled or {rs(d),rt(d)}!={base,scaled}:raise ValueError('not indexed table sum')
    if op(e)!=0x23 or rs(e)!=scaled or e&65535!=0:raise ValueError('not zero-offset table load')
    if op(j)!=0 or j&63!=8 or rs(j)!=rt(e):raise ValueError('loaded target not used by indirect jump')
    if words[index+1]!=0:raise ValueError('observed lookup requires plain NOP delay slot')
    low=c&65535;low=low-65536 if low&0x8000 else low;table=((a&65535)<<16)+low
    # Find a BEQ guard-to-default immediately preceding the straight lookup window.
    branch=None;guard=None;bound=None
    for k in range(index-6,max(-1,index-30),-1):
        w=words[k]
        if op(w)!=4 or not ((rs(w)==0)!=(rt(w)==0)):continue
        predicate=rt(w) if rs(w)==0 else rs(w)
        for g in range(k-1,max(-1,k-8),-1):
            gw=words[g]
            if op(gw)==0xb and rs(gw)==selector and rt(gw)==predicate and 0<gw&65535<0x8000:
                if any(_writes(words[q],address+q*4) is None or predicate in _writes(words[q],address+q*4) for q in range(g+1,k)):continue
                if any(_writes(words[q],address+q*4) is None or selector in _writes(words[q],address+q*4) for q in range(g+1,index-4)):continue
                branch,guard,bound=address+k*4,address+g*4,gw&65535;break
        if branch is not None:break
    if branch is None:raise ValueError('no unclobbered unsigned selector guard')
    if not 0<=table<=0xffffffff or table%4:raise ValueError('unaligned/out-of-range table')
    owners=[s for s in sections if s['type']==1 and s['flags']&2 and not s['flags']&4 and s['address']<=table and table+bound*4<=s['address']+s['size']]
    if len(owners)!=1:raise ValueError('table not inside one file-backed allocated data section')
    blob=read(table,bound*4);targets=sorted(set(struct.unpack('<'+'I'*bound,blob)))
    if any(t%4 or not address<=t<address+len(raw) for t in targets):raise ValueError('case target outside complete declared body')
    return {'jump':jump,'guard':guard,'guard_branch':branch,'guard_pass_successor':branch+8,
            'selector_register':selector,'bound':bound,'table_address':table,'table_size':bound*4,
            'table_sha256':hashlib.sha256(blob).hexdigest(),'targets':targets,'section':owners[0]['name'],
            'table_writable':bool(owners[0]['flags']&1),'runtime_immutability_proven':False,
            'policy':'static pinned-table snapshot only; no original object allocation proof'}


def prove_tables(raw,address,controls,read,sections):
    proofs=[];failures=[]
    for pc,c in controls.items():
        if c['kind']!='indirect_jump':continue
        try:proofs.append(candidate(raw,address,pc,read,sections))
        except ValueError as error:failures.append({'jump':pc,'reason':str(error)})
    tables={p['jump']:p['targets'] for p in proofs}
    accepted=[]
    for proof in proofs:
        a=cfg_replay(address,len(raw),controls,tables,skip_node=proof['guard'])
        b=cfg_replay(address,len(raw),controls,tables,skip_edge=(proof['guard_branch'],proof['guard_pass_successor']))
        if proof['jump'] in a['reachable'] or proof['jump'] in b['reachable']:
            failures.append({'jump':proof['jump'],'reason':'guard definition/pass-edge does not dominate table jump'});continue
        proof['guard_and_pass_edge_dominate_lookup']=True;accepted.append(proof)
    return accepted,failures

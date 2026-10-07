"""Private incoming-pointer ABI evidence; never original data object claims.

All reachable uses are audited. Unrecognized operations and escapes disqualify
an argument. Function calls disqualify all live provenance because no universal
callee register-preservation theorem is available. No GP assumption is made.
"""
from collections import deque
import hashlib
import struct


def incoming_pointer_roles(body, address, normalizer):
    words=struct.unpack('<'+'I'*(len(body)//4),body)
    if not words:return []
    # b80's CFG does not model REGIMM link-call return edges. Such a body
    # cannot furnish a trusted incoming provenance theorem here.
    if any(w>>26==1 and (w>>16)&31 in (16,17,18,19) for w in words):return []
    edges,controls,delays=normalizer._cfg(words,address)
    # An external conditional branch has an unanalyzed continuation. A
    # register-indirect transfer other than return has unknown ABI behavior.
    if any(kind in ('indirect-jump','indirect-call') or
           (kind=='branch' and not address<=target<address+len(body))
           for kind,target,_likely in controls.values()):return []
    context=normalizer.PreparedContext('evidence','0'*64,(),frozenset(),None,'',(),{},False,{})
    initial=tuple(frozenset(((r,0),)) if 4<=r<=11 else frozenset() for r in range(32))
    states={0:initial};queue=deque([0]);blocked={r:set() for r in range(4,12)}
    witnesses={r:set() for r in range(4,12)}
    def block(origins,index,reason):
        for arg,addend in origins:blocked[arg].add((index*4,reason))
    iterations=0
    while queue:
        iterations+=1
        if iterations>len(words)*64:return []
        i=queue.popleft();regs=list(states[i]);w=words[i]
        op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;fn=w&63
        reads,writes=normalizer._uses_defs(w,None,context)
        allowed=set();updates={}
        if op==0 and fn in (0x21,0x2d,0x25) and (rs==0 or rt==0):
            source=rt if rs==0 else rs;allowed.add(source);updates[rd]=regs[source]
        elif op in (9,0x19):
            # Pointer constant addends are retained in caller relocation proof.
            allowed.add(rs);updates[rt]=frozenset((arg,addend+normalizer.signed16(w&65535)) for arg,addend in regs[rs])
        elif op in normalizer.MEMORY:
            action,gpr,width=normalizer.MEMORY[op]
            allowed.add(rs)
            for arg,addend in regs[rs]:witnesses[arg].add((i*4,addend+normalizer.signed16(w&65535),width,action))
            if action=='store' and gpr:block(regs[rt],i,'stored incoming value')
            # A stored incoming value escapes; SP spills are deliberately
            # unqualified until overlapping-slot/lifetime proof is added.
        for register in reads-allowed:block(regs[register],i,'non-pointer use or escape')
        for register in writes:regs[register]=frozenset()
        for register,value in updates.items():
            if register:regs[register]=value
        if i in delays:
            kind=delays[i][1][0]
            if kind in ('call','indirect-call'):
                for value in regs:block(value,i,'call preservation/argument escape unproved')
                regs=[frozenset() for _ in range(32)]
            elif kind in ('return','jump','indirect-jump'):
                for register in (2,3):block(regs[register],i,'returned incoming value')
                if kind!='return':
                    target=delays[i][1][1]
                    if target is None or not address<=target<address+len(body):
                        for register in range(4,12):block(regs[register],i,'tail argument escape unproved')
        result=tuple(regs)
        for successor in edges[i]:
            if successor not in states:new=result
            else:new=tuple(a|b for a,b in zip(states[successor],result))
            if any(len(value)>16 for value in new):return []
            if successor not in states or new!=states[successor]:states[successor]=new;queue.append(successor)
    pin=hashlib.sha256(body).hexdigest()
    return [{'callee_address':address,'argument_register':arg,'callee_size':len(body),
             'callee_raw_sha256':pin,'tier':'incoming-pointer-use',
             'dereferences':[{'dereference_instruction_offset':off,'dereference_offset':addend,'width':width,'action':action} for off,addend,width,action in sorted(witnesses[arg])],
             'evidence':'All reachable tracked incoming-value uses are register copies, constant address addends or memory bases; no scalar/return/store/call escape',
             'limits':'No original object identity, unconditional dereference, source type or GP preservation claim'}
            for arg in range(4,12) if witnesses[arg] and not blocked[arg]]

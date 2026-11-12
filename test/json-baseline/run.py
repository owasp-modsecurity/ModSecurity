#!/usr/bin/env python3
"""Explicit YAJL/ModSecurity v3 characterization matrix. Never learns expectations."""
import argparse
from collections import Counter
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import subprocess
import sys

@dataclass
class Case:
    id: str
    body: bytes
    args: list = field(default_factory=list)
    error: str = '0'
    config: str = ''
    uri: str = '/'
    content_type: str = 'application/json'
    mode: str = 'transaction'
    chunks: str = ''
    select: bool = True
    access: str = 'On'
    engine: str = 'On'
    phase: str = '1'
    status: int = 200
    inbound: str = '0'
    processor_error: str = None
    config_ok: bool = True
    pre_args: list = None
    complete: int = 1
    chunk_results: list = field(default_factory=lambda:[1])
    message: str = ''
    rule: str = ''
    value_seen: str = ''
    note: str = ''

def pair(k,v): return (k.encode() if isinstance(k,str) else k,v.encode() if isinstance(v,str) else v)
def p(k,v): return pair('json.'+k,v)
def cases():
    c=[]
    def add(id,b,args=(),**kw):
        c.append(Case(id,b.encode() if isinstance(b,str) else b,list(args),**kw))
    # A: the Transaction API calls parse/complete even at zero length, then masks its error.
    add('A-absent',b'',mode='absent',note='No append call; processRequestBody still invoked')
    add('A-zero',b'')
    add('A-whitespace',b' \t\r\n',error='1',message='parse')
    add('A-object','{}');add('A-array','[]');add('A-string','""',[pair('json','')])
    # B: decoded names are binary length-aware; dots deliberately collide.
    add('B-empty','{"":"value"}',[p('empty-key','value')])
    add('B-nested-empty','{"":{"":"v"},"z":"tail"}',[p('empty-key.empty-key','v'),p('z','tail')])
    add('B-siblings','{"":"a","":"b","x":"c"}',[p('empty-key','a'),p('empty-key','b'),p('x','c')])
    add('B-dot-flat','{"a.b":1}',[p('a.b','1')])
    add('B-dot-nested','{"a":{"b":1}}',[p('a.b','1')])
    add('B-dot-collision','{"a.b":1,"a":{"b":2}}',[p('a.b','1'),p('a.b','2')])
    add('B-backslash',b'{"a\\\\b":"v"}',[p('a\\b','v')])
    add('B-unicode','{"é€😀":"v"}',[p('é€😀','v')])
    add('B-nul',b'{"a\\u0000b":"v"}',[pair(b'json.a\0b',b'v')])
    for name,token,value in [('string','"str"','str'),('empty','""',''),('integer','123','123'),('decimal','1.25','1.25'),('true','true','true'),('false','false','false'),('null','null','')]:
        add('C-'+name,token,[pair('json',value)])
        add('C-'+name+'-ws',' \n'+token+'\t ',[pair('json',value)])
    add('D-object-walkthrough','{"a":1,"b":false,"c":null}',[p('a','1'),p('b','false'),p('c','')])
    add('D-root','[1,"a",true,false,null]',[pair('json.array_'+str(i),v) for i,v in enumerate(['1','a','true','false',''])])
    add('D-object-array','{"a":[1,2],"b":3}',[p('a.array_0','1'),p('a.array_1','2'),p('b','3')])
    add('D-array-object','[{"a":1},{"b":2}]',[pair('json.array_0.a','1'),pair('json.array_1.b','2')])
    add('D-multidim','[[1,2],[3],4]',[pair('json.array_0.array_0','1'),pair('json.array_0.array_1','2'),pair('json.array_1.array_0','3'),pair('json.array_2','4')])
    add('D-empty-slots','[{},1,[],2,{},[],3,{},[]]',[pair('json.array_1','1'),pair('json.array_3','2'),pair('json.array_6','3')])
    add('D-mixed','{"a":[{},1,[],2,{"b":[true,false]}],"z":null}',[p('a.array_1','1'),p('a.array_3','2'),p('a.array_4.b.array_0','true'),p('a.array_4.b.array_1','false'),p('z','')])
    add('D-walkthrough','{"items":[["A","B"],"C"],"tail":"D"}',[p('items.array_0.array_0','A'),p('items.array_0.array_1','B'),p('items.array_1','C'),p('tail','D')])
    add('D-tail','{"a":[[{},1],2],"b":{"c":3},"z":4}',[p('a.array_0.array_1','1'),p('a.array_1','2'),p('b.c','3'),p('z','4')])
    for kind in ['object','array','mixed']:
        for depth in [2,3,4]:
            if kind=='object':
                b='{"x":'*depth+'1'+'}'*depth; args=[p('.'.join(['x']*depth),'1')]
            elif kind=='array':
                b='['*depth+'1'+']'*depth;args=[pair('json'+'.array_0'*depth,'1')]
            else:
                b='{"x":['+'{"y":'*(depth-2)+'1'+'}'*(depth-2)+']}'; args=[p('x.array_0'+'.y'*(depth-2),'1')]
            add('E-'+kind+'-'+str(depth),b,args if depth<=3 else [],config='SecRequestBodyJsonDepthLimit 3',error='0' if depth<=3 else '1',message='' if depth<=3 else 'depth')
    add('E-partial','{"a":1,"x":{"y":{"z":{"w":2}}}}',[p('a','1')],config='SecRequestBodyJsonDepthLimit 3',error='1',message='depth')
    add('E-zero-object','{}',[],config='SecRequestBodyJsonDepthLimit 0',error='1',message='depth')
    add('E-zero-scalar','1',[pair('json','1')],config='SecRequestBodyJsonDepthLimit 0')
    add('E-default-depth','{"a":{"b":[1]}}',[p('a.b.array_0','1')])
    for bad in ['-1','nope','1.5']:
        add('E-config-'+bad,'{}',config='SecRequestBodyJsonDepthLimit '+bad,config_ok=False)
    for form in ['object','array']:
        for n in [2,3,4]:
            vals=['"s"','2','true','null'][:n]
            b='{'+','.join('"k'+str(i)+'":'+v for i,v in enumerate(vals))+'}' if form=='object' else '['+','.join(vals)+']'
            args=[p('k'+str(i),v) if form=='object' else pair('json.array_'+str(i),v) for i,v in enumerate(['s','2','true',''][:min(n,3)])]
            add('F-'+form+'-'+str(n),b,args,config='SecArgumentsLimit 3',error='1' if n>3 else '0',message='cancel' if n>3 else '')
    for typ,tok in [('string','"v"'),('number','2'),('boolean','false'),('boolean-true','true'),('null','null')]:
        add('F-callback-'+typ,'{"a":1,"b":'+tok+'}',[p('a','1')],config='SecArgumentsLimit 1',error='1',message='cancel')
    add('F-empty-containers','{"a":{},"b":[],"c":null}',[p('c','')],config='SecArgumentsLimit 1')
    add('F-duplicates','{"x":1,"x":2,"x":3}',[p('x','1'),p('x','2')],config='SecArgumentsLimit 2',error='1',message='cancel')
    add('F-query','{"a":1,"b":2}',[pair('q','v'),p('a','1')],uri='/?q=v',config='SecArgumentsLimit 2',error='1',message='cancel')
    add('F-query-full','{"a":true}',[pair('q','v')],uri='/?q=v',config='SecArgumentsLimit 1',error='1',message='cancel')
    add('F-zero','{"a":null}',config='SecArgumentsLimit 0',error='1',message='cancel')
    for n,chars in [('utf2','é'),('utf3','€'),('utf4','😀')]:
        add('G-'+n,'{"'+chars+'":"'+chars+'"}',[p(chars,chars)])
    add('G-escapes',b'{"\\u00e9\\u20ac\\ud83d\\ude00":"\\u00e9\\u20ac\\ud83d\\ude00"}',[p('é€😀','é€😀')])
    add('G-common-escapes',b'{"x":"\\\"\\\\\\/\\b\\f\\n\\r\\t"}',[p('x',b'"\\/\b\f\n\r\t')])
    add('G-nul-value',b'{"x":"a\\u0000b"}',[p('x',b'a\0b')])
    invalid=[('continuation',b'\x80'),('overlong',b'\xc0\xaf'),('truncated',b'\xe2\x82'),('outside',b'\xf4\x90\x80\x80')]
    for name,bad in invalid:
        accepted=name in ['overlong','outside']
        add('G-invalid-value-'+name,b'{"x":"'+bad+b'"}',[p('x',bad)] if accepted else [],error='0' if accepted else '1',message='lexical',note='Observed YAJL byte-shape validation; not an endorsed UTF-8 requirement' if accepted else '')
        add('G-invalid-key-'+name,b'{"'+bad+b'":"v"}',[pair(b'json.'+bad,b'v')] if accepted else [],error='0' if accepted else '1',message='lexical',note='Observed YAJL byte-shape validation; not an endorsed UTF-8 requirement' if accepted else '')
    for name,esc in [('bad-escape',b'\\q'),('short-unicode',b'\\u12'),('nonhex',b'\\uZZZZ'),('raw-control',b'\x01')]:
        add('G-'+name,b'{"x":"'+esc+b'"}',error='1',message='lexical')
    # YAJL 2.1.0 UTF-8 encodes a lone low surrogate and substitutes ? for a lone high surrogate.
    add('G-high-surrogate',b'{"x":"\\ud800"}',[p('x','?')],note='YAJL substitutes question mark for isolated high surrogate; not desired contract')
    add('G-low-surrogate',b'{"x":"\\udc00"}',[p('x',b'\xed\xb0\x80')],note='Interoperability issue, not desired Unicode contract')
    nums=['0','-0','1','-1','-9223372036854775808','9223372036854775807','-9223372036854775809','9223372036854775808','18446744073709551615','18446744073709551616','9007199254740991','9007199254740992','9007199254740993','1.0','1.2300','1e2','1E+02','1e-2','1e400','1e-400','9'*96]
    for i,n in enumerate(nums):
        add('H-number-'+str(i).zfill(2),'{"n":'+n+'}',[p('n',n)])
    for token in ['01','+1','1.','1e','NaN','Infinity']:
        add('H-invalid-'+token,'{"n":'+token+'}',[p('n','0')] if token=='01' else [],error='1',message='parse' if token=='01' else 'lexical')
    add('I-same','{"x":"a","x":"a"}',[p('x','a'),p('x','a')])
    add('I-different','{"x":"a","x":"b"}',[p('x','a'),p('x','b')])
    add('I-empty','{"":1,"":2}',[p('empty-key','1'),p('empty-key','2')])
    add('I-nested','{"a":{"x":1,"x":2},"b":{"x":3}}',[p('a.x','1'),p('a.x','2'),p('b.x','3')])
    add('I-case','{"x":1,"X":2}',[p('x','1'),p('X','2')],note='Collection lookups are case insensitive; byte keys retained')
    add('J-trailing-ws','{"a":1} \r\n\t',[p('a','1')])
    add('J-trailing-data','{"a":1}x',[p('a','1')],error='1',message='parse')
    add('J-two-docs','{"a":1}{"b":2}',[p('a','1')],error='1',message='parse')
    for name,b,args in [('object','{"a":1,',[p('a','1')]),('array','[1,',[pair('json.array_0','1')]),('string','{"x":"abc',[]),('number','1e',[]),('literal','tru',[])]:
        add('J-incomplete-'+name,b,args,error='1',message='lexical' if name in ['number','literal'] else 'parse')
    add('K-content-type-no-auto','{"x":1}',select=False)
    add('K-vendor-explicit','{"x":1}',[p('x','1')],content_type='application/vnd.example+json')
    add('K-case-explicit','{"x":1}',[p('x','1')],content_type='APPLICATION/JSON; charset=UTF-8')
    add('K-no-type-explicit','{"x":1}',[p('x','1')],content_type='')
    add('K-access-off','{"x":1}',[p('x','1')],access='Off',phase='',note='API bytes still parsed before body-access guard')
    add('K-engine-off','{"x":1}',engine='Off',phase='',error='',inbound='')
    add('K-explicit-deny','{"x":',error='1',status=422,phase='1',rule='SecRule REQBODY_ERROR "@eq 1" "id:3,phase:2,deny,status:422,nolog,t:none"',message='parse')
    add('K-intro-object','{"user":"Ada","enabled":true,"count":1.2300,"optional":null}',[p('user','Ada'),p('enabled','true'),p('count','1.2300'),p('optional','')])
    add('K-rule-value','{"x":"a\\nb"}',[p('x','a\nb')],rule='SecRule ARGS:json.x "@streq 610a62" "id:4,phase:2,pass,nolog,t:none,t:hexEncode,setvar:tx.value_seen=1"',value_seen='1')
    for lim in [6,7,8]:
        add('K-body-reject-'+str(lim),'{"x":1}',[p('x','1')] if lim>=7 else [],config='SecRequestBodyLimit '+str(lim)+'\nSecRequestBodyLimitAction Reject',inbound='1' if lim<7 else '0',status=403 if lim<7 else 200)
        add('K-body-partial-'+str(lim),'{"x":1}',[p('x','1')],config='SecRequestBodyLimit '+str(lim)+'\nSecRequestBodyLimitAction ProcessPartial',inbound='1' if lim<7 else '0',error='1' if lim<7 else '0',message='parse' if lim<7 else '')
    add('K-body-detection-reject','{"x":1}',config='SecRequestBodyLimit 6\nSecRequestBodyLimitAction Reject',engine='DetectionOnly',inbound='1')
    add('K-no-files','{"x":1}',config='SecRequestBodyNoFilesLimit 6',inbound='1',error='1',processor_error='',phase='1',message='nofiles')
    add('K-long-key-value','{"'+'k'*256+'":"'+'v'*4096+'"}',[p('k'*256,'v'*4096)])
    add('K-large-array','['+','.join(str(i) for i in range(128))+']',[pair('json.array_'+str(i),str(i)) for i in range(128)])
    add('K-connector-chunks','{"x":"é\\n"}',[p('x','é\n')],chunks='1 6 1 1')
    add('K-isolation','{"a":1,',[p('a','1')],mode='isolation',error='1',message='parse')
    add('K-repeat-error','{"a":1,',[p('a','1')],mode='repeat',error='1',message='parse')
    for id,b,args,split,ok,complete in [
        ('token',b'{"n":1E+02}',[p('n','1E+02')],'6 1 1 1',[1,1,1,1,1],1),
        ('utf8','{"x":"é€😀"}'.encode(),[p('x','é€😀')],'7 1 1 1 1 1 1',[1]*8,1),
        ('escape',b'{"x":"\\ud83d\\ude00"}',[p('x','😀')],'7 3 3 2',[1]*5,1),
        ('incomplete',b'{"a":1,',[p('a','1')],'3 4',[1,1],0),
        ('trailing',b'{"a":1}x',[p('a','1')],'7 1',[1,0],0),
        ('empty',b'',[],'0',[1],0)]:
        add('L-parser-'+id,b,args,mode='parser',chunks=split,error='',inbound='',phase='',complete=complete,chunk_results=ok,message='parser' if complete==0 else '')
    add('L-parser-root-number',b'1',[pair('json','1')],mode='parser',chunks='1',error='',inbound='',phase='',pre_args=[])
    return c

def config(c):
    s='SecRuleEngine '+c.engine+'\nSecRequestBodyAccess '+c.access+'\nSecAuditEngine Off\n'
    if c.select:s+='SecAction "id:1,phase:1,pass,nolog,ctl:requestBodyProcessor=JSON"\n'
    s+='SecAction "id:2,phase:2,pass,nolog,setvar:tx.phase2=1"\n'
    return s+c.config+'\n'+c.rule+'\n'

def observe(probe,c):
    payload='\n'.join([c.mode,config(c).encode().hex(),c.body.hex(),c.uri.encode().hex(),c.content_type.encode().hex(),c.chunks])+'\n'
    r=subprocess.run([probe],input=payload,text=True,capture_output=True,timeout=10)
    if r.returncode:raise RuntimeError(f'probe exit={r.returncode}: {r.stderr}')
    rec={'ARGS':[],'GET':[],'POST':[],'NAMES':[],'PRE_ARGS':[],'CHUNK':[],'STATUSES':[],'TRANSACTIONS':[]}
    for l in r.stdout.splitlines():
        bits=l.split('\t');n=bits[0]
        if n in ['ARGS','GET','POST','NAMES','PRE_ARGS']:rec[n].append(tuple(bytes.fromhex(v) for v in bits[1:]))
        elif n=='CHUNK':rec[n].append(int(bits[1]))
        elif n in ['CONFIG','YAJL_ACTIVE']:rec[n]=int(bits[1])
        elif n=='INTERVENTION':
            rec[n]=int(bits[2]);rec['STATUSES'].append(int(bits[2]));rec['TRANSACTIONS'].append({**{k:rec.get(k) for k in ['ERROR','PROCESSOR_ERROR','PHASE2','PROCESSOR','ERROR_MSG']},**{k:rec[k][rec['OFFSETS'][k]:] for k in ['ARGS','GET','POST','NAMES']}})
        elif n=='APPEND_INTERVENTION':rec['STATUSES'].append(int(bits[1]))
        elif n=='COMPLETE':rec[n]=int(bits[1])
        elif n=='TRANSACTION':rec['OFFSETS']={k:len(rec[k]) for k in ['ARGS','GET','POST','NAMES']}
        elif n in ['APPEND','PROCESS']:pass
        else:rec[n]=bytes.fromhex(bits[1])
    return rec,r.stdout

def check(c,r):
    errors=[]
    def eq(n,a,b):
        if a!=b:errors.append(f'{n}: observed={a!r}, expected={b!r}')
    eq('config',r['CONFIG'],int(c.config_ok))
    if not c.config_ok:return errors
    eq('active library JSON canary',r.get('YAJL_ACTIVE'),1)
    repeats=2 if c.mode=='repeat' else 1
    expected=c.args*repeats
    if c.mode=='isolation':expected=c.args+[p('ok.array_0','1'),p('ok.array_1','2')]
    eq('ARGS byte multiset',Counter(r['ARGS']),Counter(expected))
    eq('ARGS_NAMES',Counter(r['NAMES']),Counter((k,k) for k,v in expected))
    get=[pair('q','v')] if '?q=v' in c.uri else []
    eq('ARGS_GET',Counter(r['GET']),Counter(get*repeats))
    eq('ARGS_POST',r['POST'],[])
    for k,w in [('ERROR',c.error),('PROCESSOR_ERROR',c.processor_error if c.processor_error is not None else c.error),('INBOUND',c.inbound),('PHASE2',c.phase),('VALUE_SEEN',c.value_seen)]:
        eq(k,r[k],('0' if c.mode=='isolation' and k in ['ERROR','PROCESSOR_ERROR'] else w).encode())
    expected_processor='JSON' if c.select and c.engine!='Off' else ''
    eq('processor',r['PROCESSOR'],expected_processor.encode())
    eq('intervention status',max(r['STATUSES']),c.status)
    if c.mode=='isolation':
        eq('per transaction errors',[t['ERROR'] for t in r['TRANSACTIONS']],[b'1',b'0'])
        eq('per transaction processor errors',[t['PROCESSOR_ERROR'] for t in r['TRANSACTIONS']],[b'1',b'0'])
        for i,expected_args in enumerate([c.args,[p('ok.array_0','1'),p('ok.array_1','2')]]):
            t=r['TRANSACTIONS'][i]
            eq('transaction '+str(i)+' ARGS',Counter(t['ARGS']),Counter(expected_args))
            eq('transaction '+str(i)+' NAMES',Counter(t['NAMES']),Counter((k,k) for k,v in expected_args))
            eq('transaction '+str(i)+' POST',t['POST'],[])
            eq('transaction '+str(i)+' phase',t['PHASE2'],b'1')
            eq('transaction '+str(i)+' processor',t['PROCESSOR'],b'JSON')
        eq('first error class',r['TRANSACTIONS'][0]['ERROR_MSG'].startswith(b'JSON parsing error: parse error:'),True)
        eq('second cleared message',r['TRANSACTIONS'][1]['ERROR_MSG'],b'')
    if c.mode=='parser':
        eq('chunk results',r['CHUNK'],c.chunk_results)
        eq('pre-completion ARGS',Counter(r['PRE_ARGS']),Counter(c.args if c.pre_args is None else c.pre_args))
        eq('complete',r['COMPLETE'],c.complete)
        eq('parser error present',bool(r['PARSER_ERROR']),c.complete==0)
    elif c.mode!='isolation':
        msg=r['ERROR_MSG']
        if c.error=='1':
            if c.message=='depth':eq('depth class',b'Parsing depth limit exceeded' in msg,True)
            elif c.message=='cancel':eq('callback cancellation',b'client cancelled' in msg,True)
            elif c.message=='nofiles':eq('nofiles class',b'excluding files' in msg,True)
            else:eq('JSON '+c.message+' error class',msg.startswith(('JSON parsing error: '+c.message+' error:').encode()),True)
        elif c.error=='0':eq('no error message',msg,b'')
    return errors

def main():
    a=argparse.ArgumentParser();a.add_argument('--probe',required=True);a.add_argument('--output');a.add_argument('--filter',default='');a.add_argument('--sensitivity',action='store_true');args=a.parse_args()
    output=[];passed=failed=blocked=0
    for c in cases():
        if args.filter and args.filter not in c.id:continue
        try:
            r,raw=observe(args.probe,c);errors=check(c,r);status='FAIL' if errors else 'PASS'
            if errors:failed+=1
            else:passed+=1
            entry={'id':c.id,'status':status,'body_length':len(c.body),'body_hex':c.body.hex(),'body_sha256':hashlib.sha256(c.body).hexdigest(),'config':config(c),'uri':c.uri,'content_type':c.content_type,'mode':c.mode,'chunks':c.chunks,'expected_args_hex':[[k.hex(),v.hex()] for k,v in c.args],'expected_error':c.error,'expected':{'processor_error':c.processor_error if c.processor_error is not None else c.error,'inbound':c.inbound,'phase2':c.phase,'http_intervention':c.status,'message_class':c.message,'config_accepted':c.config_ok,'complete':c.complete if c.mode=='parser' else None,'chunk_results':c.chunk_results if c.mode=='parser' else None},'note':c.note,'errors':errors,'observed_tsv':raw}
        except Exception as e:
            is_blocked=isinstance(e,FileNotFoundError) or 'probe exit=77:' in str(e)
            status='BLOCKED' if is_blocked else 'FAIL'
            if is_blocked:blocked+=1
            else:failed+=1
            reason=('timeout' if isinstance(e,subprocess.TimeoutExpired) else 'probe or harness failure')
            entry={'id':c.id,'status':status,'errors':[reason+': '+str(e)]}
        output.append(entry);print(status,c.id, '; '.join(entry['errors'])[:700])
    if not output:
        print('BLOCKED: no cases matched filter',file=sys.stderr);return 2
    sensitivity=[]
    if args.sensitivity and not blocked and not failed:
        for id in ['B-nul','H-number-16','I-different','J-incomplete-object']:
            c=next(c for c in cases() if c.id==id);r,_=observe(args.probe,c)
            if id=='J-incomplete-object':c.error='0'
            else:c.args=[pair('deliberately-wrong','wrong')]
            ok=bool(check(c,r));sensitivity.append({'id':id,'status':'PASS' if ok else 'FAIL'})
            print('PASS' if ok else 'FAIL','sensitivity-'+id)
            if ok:passed+=1
            else:failed+=1
    report={'version':'v3','counts':{'PASS':passed,'FAIL':failed,'SKIP':0,'BLOCKED':blocked,'XFAIL':0},'cases':output,'sensitivity':sensitivity}
    if args.output:Path(args.output).write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report['counts']));return int(bool(failed or blocked))
if __name__=='__main__':sys.exit(main())

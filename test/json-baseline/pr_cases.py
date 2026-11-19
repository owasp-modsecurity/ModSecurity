"""Independent v3 tests inspired by jsonbench PRs 4/5; no adapter code copied.

Fixed expectations were established on ModSecurity 8271c9d3 with YAJL 2.1.0.
They are rechecked on the current original named in the runtime report.
Proposed requirements are separate, may FAIL, and do not alter that baseline.
"""
from dataclasses import dataclass, field
from run import Case, p, pair

PR4 = 'https://github.com/digitalwave/jsonbench/pull/4'
PR5 = 'https://github.com/digitalwave/jsonbench/pull/5'
REGRESSION = 'https://github.com/Easton97-Jens/jsonbench/blob/715ae64f718577e72a9f54b3a0a08a3c99fd84f6/tests/regression_tests.py'
API = 'https://github.com/Easton97-Jens/jsonbench/blob/715ae64f718577e72a9f54b3a0a08a3c99fd84f6/tests/yajl_api_tests.cpp'
POLICY = 'https://github.com/Easton97-Jens/jsonbench/blob/715ae64f718577e72a9f54b3a0a08a3c99fd84f6/docs/adapter-stack.md'
FZIPI = 'https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842'
AIRWEEN = 'https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6142615388'

@dataclass
class ExtendedCase(Case):
    area: str = ''
    sources: list = field(default_factory=list)
    contract: str = 'characterization'
    second_body: bytes = b'{"ok":1}'
    second_args: list = field(default_factory=lambda: [p('ok', '1')])
    # A stable classification supplements exact byte and ModSecurity flag checks.
    failure_stage: str = ''


def cases():
    result = []
    def add(id, body, args=(), area='', sources=None, **kw):
        result.append(ExtendedCase(id, body.encode() if isinstance(body, str) else body,
                      list(args), area=area, sources=sources or [PR4, PR5, FZIPI], **kw))
    # Numeric-looking keys and strings must never choose the numeric callback.
    add('PR-N-token-kinds', '{"0":"01","1":1E+02,"01":"1e2","n":-0.0,"s":"18446744073709551616","u":18446744073709551616}',
        [p('0','01'),p('1','1E+02'),p('01','1e2'),p('n','-0.0'),p('s','18446744073709551616'),p('u','18446744073709551616')],
        area='H', sources=[REGRESSION+'#L92', AIRWEEN])
    add('PR-N-array-token-kinds', '["1e2",1e2,"-0",-0,"true",true,"null",null]',
        [pair('json.array_'+str(i),v) for i,v in enumerate(['1e2','1e2','-0','-0','true','true','null',''])],area='D,H')
    add('PR-B-dotted-parent-sibling', '{"a.b":{"c":1},"d":2,"a":{"b.c":3}}',
        [p('a.b.c','1'),p('d','2'),p('a.b.c','3')],area='B,D',sources=[REGRESSION+'#L79',FZIPI])
    add('PR-B-dotted-array-sibling', '{"a.b":[{"c.d":1},[],2],"tail":3}',
        [p('a.b.array_0.c.d','1'),p('a.b.array_2','2'),p('tail','3')],area='B,D')
    add('PR-B-nested-nul', b'{"a\\u0000b":{"c\\u0000d":"e\\u0000f"},"tail":2}',
        [pair(b'json.a\0b.c\0d',b'e\0f'),p('tail','2')],area='B,G',sources=[API+'#L353',FZIPI])
    add('PR-G-literal-backslash-surrogate', b'{"x":"\\\\ud800","\\\\udc00":"\\\\u0000"}',
        [p('x',b'\\ud800'),p('\\udc00',b'\\u0000')],area='B,G')
    for name, text, decoded in [
        ('high-ordinary',b'\\ud800A',b'?A'),
        ('high-high',b'\\ud800\\ud801',b'\xf0\x90\x80\x81'),
        ('low-high',b'\\udc00\\ud800',b'\xed\xb0\x80?'),
        ('high-end',b'\\ud800',b'?'),
        ('low-end',b'\\udc00',b'\xed\xb0\x80')]:
        add('PR-G-'+name,b'{"x":"'+text+b'"}',[p('x',decoded)],area='G',sources=[REGRESSION+'#L113',FZIPI],
            note='Observed YAJL surrogate conversion; characterization, not a desired Unicode requirement.')
        add('PR-G-key-'+name,b'{"'+text+b'":"v"}',[pair(b'json.'+decoded,b'v')],area='B,G',
            note='Length-aware decoded key; surrogate handling is not endorsed.')
    add('PR-G-raw-surrogate-value',b'{"x":"\xed\xa0\x80"}',[p('x',b'\xed\xa0\x80')],area='G',sources=[REGRESSION+'#L113',AIRWEEN],
        note='Ill-formed UTF-8 accepted by YAJL: preserved only as characterization.')
    add('PR-G-raw-surrogate-key',b'{"\xed\xa0\x80":"v"}',[pair(b'json.\xed\xa0\x80',b'v')],area='B,G',sources=[REGRESSION+'#L113',AIRWEEN])
    add('PR-G-ff-key',b'{"\xff":"v"}',error='1',message='lexical',area='B,G',sources=[REGRESSION+'#L113',AIRWEEN])
    add('PR-G-raw-nul-key',b'{"a\x00b":"v"}',error='1',message='lexical',area='B,G',sources=[API+'#L132',FZIPI])
    add('PR-G-raw-nul-value',b'{"x":"a\x00b"}',error='1',message='lexical',area='G',sources=[API+'#L132',FZIPI])
    add('PR-J-trailing-raw-nul',b'{"x":1}\x00',[p('x','1')],error='1',message='parse',area='J',sources=[REGRESSION+'#L113',FZIPI])
    syntax=[('colon',b'{"x" 1}',[], 'parse'),('comma',b'{"x":1 "z":2}',[p('x','1')],'parse'),
            ('equals',b'{"x"=1}',[],'lexical'),('object-comma',b'{"x":1,}',[p('x','1')],'parse'),
            ('array-comma',b'[1,]',[pair('json.array_0','1')],'parse'),
            ('block-comment',b'{"x":/* comment */1}',[],'lexical'),
            ('line-comment',b'{"x":// comment\n1}',[],'lexical')]
    for name,body,args,kind in syntax:
        add('PR-J-syntax-'+name,body,args,error='1',message=kind,area='J',sources=[REGRESSION+'#L113',FZIPI])
    for name,token in [('dot-one','.1'),('exponent-plus','1e+'),('double-minus','--1')]:
        add('PR-H-invalid-'+name,'{"n":'+token+'}',error='1',message='lexical',area='H,J',sources=[REGRESSION+'#L113',FZIPI])
    for name,token in [('decimal30','0.123456789012345678901234567890'),('negative-zero','-0.0'),
                       ('huge-exponent','1e+10000'),('tiny-exponent','1e-10000'),('exponent-zeroes','1e-0003')]:
        add('PR-H-'+name,'{"n":'+token+'}',[p('n',token)],area='H',sources=[REGRESSION+'#L95',AIRWEEN])
    add('PR-H-long-integer','{"n":'+'9'*512+'}',[p('n','9'*512)],area='H',sources=[API+'#L313',AIRWEEN])
    add('PR-H-long-incomplete-exponent',b'{"n":'+b'9'*512+b'e+',error='1',message='lexical',area='H,J',sources=[API+'#L313',FZIPI])
    # Every scalar callback must propagate an argument limit cancellation.
    for name,token,value in [('string','"v"','v'),('number','1E+02','1E+02'),('true','true','true'),('false','false','false'),('null','null','')]:
        for n in [2,3]:
            add('PR-F-array-'+name+'-'+str(n),'['+','.join([token]*n)+']',
                [pair('json.array_'+str(i),value) for i in range(min(n,2))],config='SecArgumentsLimit 2',
                error='1' if n==3 else '0',message='cancel' if n==3 else '',area='D,F',sources=[API+'#L248',FZIPI])
    # Error then success: same engine and configuration, new transaction/parser.
    for name,body,args,conf,kind in [
        ('syntax',b'{"a":1 "b":2}',[p('a','1')],'','parse'),
        ('depth',b'{"a":1,"x":{"y":{"z":2}}}',[p('a','1')],'SecRequestBodyJsonDepthLimit 2','depth'),
        ('arguments',b'{"a":1,"b":null}',[p('a','1')],'SecArgumentsLimit 1','cancel'),
        ('eof',b'{"a":1,',[p('a','1')],'','parse')]:
        add('PR-S-sequence-'+name,body,args,mode='sequence',error='1',message=kind,config=conf,
            area='J,K',sources=[API+'#L254',FZIPI],note='Two transactions use one ModSecurity engine and RulesSet; second body is {"ok":1}.')
    # The benchmark's 8192-byte observer buffer is not a ModSecurity limit.
    for n in [8191,8192,8193,16384]:
        add('PR-L-raw-value-'+str(n),'{"x":"'+'v'*n+'"}',[p('x','v'*n)],area='G,K',sources=[API+'#L313',FZIPI])
    for n in [8191,8192,8193]:
        add('PR-L-raw-key-'+str(n),'{"'+'k'*n+'":"tail"}',[p('k'*n,'tail')],area='B,K',sources=[API+'#L313',FZIPI])
    add('PR-L-escaped-value',b'{"x":"'+b'\\u0061'*8193+b'"}',[p('x','a'*8193)],area='G,K',sources=[API+'#L313',FZIPI])
    add('PR-L-escaped-key',b'{"'+b'\\u006b'*8193+b'":"v"}',[p('k'*8193,'v')],area='B,G,K',sources=[API+'#L313',FZIPI])
    add('PR-L-complete-path','{"'+'p'*8193+'":{"'+'k'*513+'":1},"tail":2}',
        [p('p'*8193+'.'+'k'*513,'1'),p('tail','2')],area='B,K',sources=[API+'#L313',FZIPI])
    long_incomplete=b'{"a":1,"x":"'+b'v'*16385
    add('PR-L-incomplete-string',long_incomplete,[p('a','1')],error='1',message='parse',area='J,K',sources=[API+'#L313',FZIPI])
    add('PR-C-16k-incomplete-string',long_incomplete,[p('a','1')],mode='parser',
        chunks='16384 '+str(len(long_incomplete)-16384),chunk_results=[1,1],complete=0,
        error='',inbound='',phase='',message='parser',area='J,K',sources=[API+'#L313',FZIPI],failure_stage='complete')
    # Actual internal JSON::processChunk calls, not connector append boundaries.
    split_payloads=[
        ('utf8','{"é":"€😀"}'.encode(),[p('é','€😀')],None,1),
        ('escape',b'{"x":"\\ud83d\\ude00\\n"}',[p('x','😀\n')],None,1),
        ('number',b'{"n":-0.123456789012345678901234567890E+002}',[p('n','-0.123456789012345678901234567890E+002')],None,1),
        ('root-number',b'1E+02',[pair('json','1E+02')],[],1),
        ('incomplete-number',b'1e+',[],[],0),
        ('incomplete-string',b'{"x":"abc',[],[],0)]
    for name,body,args,pre,complete in split_payloads:
        for split in range(len(body)+1):
            add('PR-C-'+name+'-split-'+str(split).zfill(3),body,args,mode='parser',
                chunks=str(split)+' '+str(len(body)-split),chunk_results=[1,1],pre_args=pre,
                complete=complete,error='',inbound='',phase='',message='parser' if not complete else '',
                area='G,H,J,K',sources=[API+'#L132',FZIPI],failure_stage='complete' if not complete else '')
        add('PR-C-'+name+'-bytewise',body,args,mode='parser',chunks=' '.join(['1']*len(body)),
            chunk_results=[1]*len(body),pre_args=pre,complete=complete,error='',inbound='',phase='',
            message='parser' if not complete else '',area='G,H,J,K',sources=[API+'#L209',FZIPI],
            failure_stage='complete' if not complete else '')
    for split in range(9):
        add('PR-C-trailing-split-'+str(split),b'{"a":1}x',[p('a','1')],mode='parser',
            chunks=str(split)+' '+str(8-split),chunk_results=[0] if split==8 else [1,0],complete=0,
            error='',inbound='',phase='',message='parser',area='J,K',sources=[API+'#L132',FZIPI],failure_stage='chunk')
    for name,body,args in [
        ('utf8',b'{"x":"'+b'v'*16377+'😀'.encode()+b'"}',[p('x','v'*16377+'😀')]),
        ('escape',b'{"x":"'+b'v'*16377+b'\\ud83d\\ude00"}',[p('x','v'*16377+'😀')]),
        ('number',b'{"p":"'+b'v'*16370+b'","n":1E+002}',[p('p','v'*16370),p('n','1E+002')])]:
        add('PR-C-16k-'+name,body,args,mode='parser',chunks='16384 '+str(len(body)-16384),
            chunk_results=[1,1],error='',inbound='',phase='',area='G,H,K',sources=[API+'#L132',FZIPI])
    return result


def requirements():
    """Reviewable desired policies. Failures remain real failures, never XFAIL."""
    result=[]
    def add(id,body,area='G',contract='required-utf8-validation',note='',sources=None,**kw):
        result.append(ExtendedCase(id,body,area=area,contract=contract,sources=sources or [AIRWEEN,REGRESSION+'#L113'],note=note,**kw))
    malformed=[('continuation',b'\x80'),('overlong',b'\xc0\xaf'),('truncated',b'\xe2\x82'),
               ('above-unicode',b'\xf4\x90\x80\x80'),('surrogate',b'\xed\xa0\x80'),('ff',b'\xff')]
    for name,bad in malformed:
        for where in ['key','value']:
            body=b'{"'+bad+b'":"v"}' if where=='key' else b'{"x":"'+bad+b'"}'
            add('REQ-UTF8-'+where+'-'+name,body,error='1',message='lexical',
                note='Desired rejection of ill-formed UTF-8; RFC 8259 section 8.1 requires UTF-8 for interoperable exchange. Current YAJL byte-shape checks are insufficient.')
    # RFC 8259 ABNF admits these escape sequences; rejecting them is a separate
    # Unicode interoperability policy proposal, not an existing syntax contract.
    for name,bad in [('high',b'\\ud800'),('low',b'\\udc00'),('high-high',b'\\ud800\\ud801')]:
        for where in ['key','value']:
            body=b'{"'+bad+b'":"v"}' if where=='key' else b'{"x":"'+bad+b'"}'
            add('REQ-POLICY-surrogate-'+where+'-'+name,body,error='1',message='lexical',
                contract='proposed-unicode-scalar-policy',
                note='Open policy decision: reject isolated/repeated high surrogate escapes. RFC 8259 section 8.2 interoperability warning; not a mandatory syntax rejection.')
    for name,body,conf,kind in [
        ('syntax',b'{"a":1 "b":2}','','parse'),
        ('eof',b'{"a":1,','','parse'),
        ('depth',b'{"a":1,"x":{"y":{"z":2}}}','SecRequestBodyJsonDepthLimit 2','depth'),
        ('arguments',b'{"a":1,"b":null}','SecArgumentsLimit 1','cancel')]:
        add('REQ-POLICY-rollback-'+name,body,area='E,F,J',error='1',message=kind,config=conf,
            contract='proposed-atomic-extraction-policy',sources=[PR5,POLICY,API+'#L254'],
            note='Open policy proposal from PR5: discard partially extracted JSON ARGS on any parse failure. Current ModSecurity retains them; this is not an accepted compatibility requirement.')
    return result

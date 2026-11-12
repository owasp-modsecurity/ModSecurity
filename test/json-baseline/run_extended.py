#!/usr/bin/env python3
"""Run fixed v3 characterization expectations and separately proposed policies.

This tests ModSecurity transactions or its internal JSON request processor,
never a replacement adapter or a direct YAJL-only reproduction.
"""
import argparse
from collections import Counter
from copy import deepcopy
from dataclasses import replace
import hashlib
import json
from pathlib import Path
import subprocess
import sys

from run import check, config, pair
from pr_cases import cases, requirements

_COLLECTIONS = ('ARGS','GET','POST','NAMES','PRE_ARGS')
_VALUES = ('ERROR','PROCESSOR_ERROR','ERROR_MSG','PROCESSOR_ERROR_MSG',
           'PROCESSOR','INBOUND','REQUEST_BODY','PHASE2','VALUE_SEEN','PARSER_ERROR')

class ProbeFailure(RuntimeError):
    def __init__(self, kind, detail):
        super().__init__(detail)
        self.kind = kind


def observe(probe, case):
    lines=[case.mode,config(case).encode().hex(),case.body.hex(),case.uri.encode().hex(),
           case.content_type.encode().hex(),case.chunks]
    if case.mode=='sequence': lines.append(case.second_body.hex())
    process=subprocess.run([probe],input='\n'.join(lines)+'\n',text=True,
                           capture_output=True,timeout=10)
    if process.returncode:
        kind=('json-support-unavailable' if process.returncode==77 else
              'probe-crash' if process.returncode<0 else 'probe-or-harness-error')
        raise ProbeFailure(kind,f'exit={process.returncode}; stderr={process.stderr[:2000]}')
    globals_={}
    transactions=[]
    current=None
    for line in process.stdout.splitlines():
        bits=line.split('\t')
        name=bits[0]
        if name=='TRANSACTION':
            if int(bits[1])!=len(transactions):
                raise ProbeFailure('malformed-protocol','unexpected transaction index')
            current={key:[] for key in _COLLECTIONS}
            current.update(CHUNK=[],STATUSES=[],APPEND=[],PROCESS=[])
            transactions.append(current)
        elif name in ('CONFIG','YAJL_ACTIVE'):
            globals_[name]=int(bits[1])
        elif name in ('VERSION','CONFIG_ERROR'):
            globals_[name]=bytes.fromhex(bits[1])
        elif current is None:
            raise ProbeFailure('malformed-protocol','transaction data before transaction')
        elif name in _COLLECTIONS:
            if len(bits)!=3:raise ProbeFailure('malformed-protocol','invalid collection record')
            current[name].append(tuple(bytes.fromhex(v) for v in bits[1:]))
        elif name in ('CHUNK','APPEND','PROCESS'):
            current[name].append(int(bits[1]))
        elif name=='APPEND_INTERVENTION':current['STATUSES'].append(int(bits[1]))
        elif name=='INTERVENTION':
            current[name]=int(bits[2]);current['STATUSES'].append(int(bits[2]))
        elif name=='COMPLETE':current[name]=int(bits[1])
        elif name in _VALUES:current[name]=bytes.fromhex(bits[1])
        else:raise ProbeFailure('malformed-protocol','unknown record '+name)
    if globals_.get('YAJL_ACTIVE')!=1:
        raise ProbeFailure('json-support-unavailable','active JSON canary missing')
    if 'CONFIG' not in globals_:raise ProbeFailure('malformed-protocol','configuration result missing')
    if globals_['CONFIG'] and not transactions:
        raise ProbeFailure('malformed-protocol','no transaction executed')
    for tx in transactions:
        tx.update(globals_)
        for key in _VALUES[:-1]:
            if key not in tx:raise ProbeFailure('malformed-protocol','missing '+key)
        if 'INTERVENTION' not in tx:raise ProbeFailure('malformed-protocol','intervention missing')
    return globals_,transactions,process.stdout


def check_extended(case, globals_, transactions):
    errors=[]
    expected_count=2 if case.mode=='sequence' else 1
    if len(transactions)!=expected_count:
        return [f'transaction count: observed={len(transactions)}, expected={expected_count}']
    for i,observed in enumerate(transactions):
        expected=case
        if case.mode=='sequence':
            expected=replace(case,mode='transaction')
            if i==1:
                expected=replace(expected,body=case.second_body,args=case.second_args,error='0',message='')
        errors.extend('transaction '+str(i)+': '+err for err in check(expected,observed))
        if expected.mode=='parser':
            if observed['PROCESS']:
                errors.append('parser-level case unexpectedly called transaction processRequestBody')
            if expected.failure_stage=='complete' and not all(observed['CHUNK']):
                errors.append('expected failure only on complete, but a processChunk call failed')
            if expected.failure_stage=='chunk' and 0 not in observed['CHUNK']:
                errors.append('expected processChunk rejection did not occur')
        elif observed['PROCESS']!=[1]:
            errors.append(f'transaction {i}: processRequestBody call/results {observed["PROCESS"]!r}, expected [1]')
    return errors


def expected_record(case):
    result={
        'args_hex':[[key.hex(),value.hex()] for key,value in case.args],
        'reqbody_error':case.error,
        'processor_error':case.processor_error if case.processor_error is not None else case.error,
        'message_class':case.message,'inbound':case.inbound,'phase2':case.phase,
        'http_intervention':case.status,
        'processor':'JSON' if case.select and case.engine!='Off' else '',
        'args_post':[],
    }
    if case.mode=='parser':
        result.update(complete=case.complete,chunk_results=case.chunk_results,
            pre_args_hex=[[key.hex(),value.hex()] for key,value in
                          (case.args if case.pre_args is None else case.pre_args)],
            failure_stage=case.failure_stage)
    if case.mode=='sequence':
        result['second_transaction']={'body_hex':case.second_body.hex(),
            'args_hex':[[key.hex(),value.hex()] for key,value in case.second_args],
            'reqbody_error':'0','processor_error':'0','error_message':'','phase2':'1'}
    return result


def main(default_suite='characterization'):
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--probe',required=True)
    parser.add_argument('--output')
    parser.add_argument('--filter',default='')
    parser.add_argument('--suite',choices=['characterization','requirements'],default=default_suite)
    parser.add_argument('--sensitivity',action='store_true')
    opts=parser.parse_args()
    available=cases() if opts.suite=='characterization' else requirements()
    selected=[case for case in available if opts.filter in case.id]
    if not selected:
        print('BLOCKED: no cases matched filter',file=sys.stderr)
        return 2
    output=[]
    counts={key:0 for key in ['PASS','FAIL','SKIP','BLOCKED','XFAIL']}
    for case in selected:
        entry={'id':case.id,'area':case.area,'contract':case.contract,'sources':case.sources,
            'note':case.note,'level':'internal-ModSecurity-JSON-processor' if case.mode=='parser' else 'ModSecurity-transaction',
            'body_length':len(case.body),'body_hex':case.body.hex(),
            'body_sha256':hashlib.sha256(case.body).hexdigest(),'uri':case.uri,
            'content_type':case.content_type,'config':config(case),'mode':case.mode,
            'chunks':[int(n) for n in case.chunks.split()], 'expected':expected_record(case)}
        try:
            globals_,transactions,raw=observe(opts.probe,case)
            errors=check_extended(case,globals_,transactions)
            entry.update(status='FAIL' if errors else 'PASS',errors=errors,
                         failure_kind='assertion' if errors else '',observed_tsv=raw)
        except (FileNotFoundError,ProbeFailure,subprocess.TimeoutExpired,ValueError,KeyError) as exc:
            blocked=isinstance(exc,FileNotFoundError) or (isinstance(exc,ProbeFailure) and exc.kind=='json-support-unavailable')
            kind='missing-probe' if isinstance(exc,FileNotFoundError) else 'timeout' if isinstance(exc,subprocess.TimeoutExpired) else getattr(exc,'kind','malformed-protocol')
            entry.update(status='BLOCKED' if blocked else 'FAIL',failure_kind=kind,errors=[str(exc)])
        counts[entry['status']]+=1
        output.append(entry)
        print(entry['status'],case.id,'; '.join(entry['errors'])[:300])
    sensitivity=[]
    if opts.sensitivity:
        if opts.suite!='characterization':
            print('--sensitivity is only supported for characterization',file=sys.stderr)
            return 2
        for id in ['PR-N-token-kinds','PR-B-nested-nul','PR-C-root-number-split-000','PR-J-syntax-colon']:
            source=next(case for case in available if case.id==id)
            bad=deepcopy(source)
            if id=='PR-J-syntax-colon':bad.error='0'
            elif id=='PR-C-root-number-split-000':bad.complete=0
            else:bad.args=[pair('deliberately-wrong','wrong')]
            try:
                globals_,transactions,_=observe(opts.probe,source)
                rejected=bool(check_extended(bad,globals_,transactions))
                status='PASS' if rejected else 'FAIL'
                errors=[] if rejected else ['deliberately false expectation was not rejected']
            except Exception as exc:
                status='FAIL';errors=['control did not reach an assertion: '+str(exc)]
            sensitivity.append({'id':id,'status':status,'errors':errors,
                'purpose':'The correct observation must reject one deliberately wrong expectation.'})
            counts[status]+=1
            print(status,'sensitivity-'+id)
    report={'version':'v3','production_commit':'dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd',
        'suite':opts.suite,'counts':counts,'cases':output,'sensitivity':sensitivity,
        'interpretation': 'Policy proposals remain FAIL if unmet; they are neither XFAIL nor adopted compatibility requirements.' if opts.suite=='requirements' else 'Current behavior characterization; a PASS does not endorse questionable behavior.'}
    if opts.output:Path(opts.output).write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(counts))
    return int(bool(counts['FAIL'] or counts['BLOCKED']))

if __name__=='__main__':sys.exit(main())

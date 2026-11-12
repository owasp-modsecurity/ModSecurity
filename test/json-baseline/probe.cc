/* Test-only byte-preserving observer for the unchanged libModSecurity API. */
#include "modsecurity/modsecurity.h"
#include "modsecurity/rules_set.h"
#include "modsecurity/transaction.h"
#include "src/request_body_processor/json.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::string unhex(const std::string& s) {
    if (s.size() % 2) throw std::runtime_error("odd hex input");
    std::string r;
    for (size_t i=0;i<s.size();i+=2) {
        size_t n=0;
        auto v=std::stoul(s.substr(i,2), &n,16);
        if (n!=2) throw std::runtime_error("invalid hex input");
        r.push_back(static_cast<char>(v));
    }
    return r;
}
std::string hex(const std::string& s) {
    static const char *d="0123456789abcdef";
    std::string r;
    for (unsigned char c:s) { r+=d[c>>4];r+=d[c&15]; }
    return r;
}
std::string line() { std::string s; if(!std::getline(std::cin,s)) throw std::runtime_error("missing protocol line"); return s; }
void value(const char *n, const std::string& s) { std::cout<<n<<"\t"<<hex(s)<<"\n"; }
template<class Variable>
void dump(const char *n, Variable& v) {
    std::vector<const modsecurity::VariableValue *> vals;
    v.resolve(&vals);
    for (const auto *p: vals) {
        std::cout<<n<<"\t"<<hex(p->getKey())<<"\t"<<hex(p->getValue())<<"\n";
        delete p;
    }
}
int main() {
    try {
        const std::string mode=line(), rulesText=unhex(line()), firstBody=unhex(line());
        const std::string uri=unhex(line()), contentType=unhex(line()), chunkLine=line();
        // Optional seventh line only for the new sequence mode. Legacy inputs
        // remain six lines; both bodies share one engine and one RulesSet.
        const std::string secondBody=(mode=="sequence")?unhex(line()):"{\"ok\":[1,2]}";
        modsecurity::ModSecurity engine;
        std::cout<<"VERSION\t"<<hex(engine.whoAmI())<<"\n";
#ifndef WITH_YAJL
        std::cerr<<"BLOCKED: JSON support was not enabled\n";
        return 77;
#else
        modsecurity::RulesSet canaryRules;
        if(canaryRules.load("SecRuleEngine On\nSecRequestBodyAccess On\nSecAction \"id:99,phase:1,pass,nolog,ctl:requestBodyProcessor=JSON\"\n")<0)
            throw std::runtime_error("JSON canary rules rejected");
        {
            modsecurity::Transaction canary(&engine,&canaryRules,nullptr);
            canary.processConnection("127.0.0.1",1,"127.0.0.1",80);
            canary.processURI("/","POST","1.1");
            canary.addRequestHeader("Content-Type","application/json");
            canary.processRequestHeaders();
            const std::string sentinel="{\"canary\":\"active\"}";
            canary.appendRequestBody(reinterpret_cast<const unsigned char*>(sentinel.data()),sentinel.size());
            canary.processRequestBody();
            auto a=canary.m_variableArgs.resolveFirst("json.canary");
            if(!a || *a!="active" || canary.m_variableReqbodyError.m_value!="0") {
                std::cerr<<"BLOCKED: active library JSON canary failed\n";return 77;
            }
        }
        std::cout<<"YAJL_ACTIVE\t1\n";
        modsecurity::RulesSet rules;
        int loaded=rules.load(rulesText.c_str());
        std::cout<<"CONFIG\t"<<(loaded>=0)<<"\n";
        if(loaded<0) { value("CONFIG_ERROR",rules.getParserError());return 0; }
        // Two sequential transactions can deliberately share the engine/rules.
        for (int repeat=0;repeat<((mode=="repeat" || mode=="isolation" || mode=="sequence")?2:1);++repeat) {
            const std::string body=((mode=="isolation" || mode=="sequence") && repeat==1)?secondBody:firstBody;
            modsecurity::Transaction tx(&engine,&rules,nullptr);
            tx.processConnection("127.0.0.1",12345,"127.0.0.1",80);
            tx.processURI(uri.c_str(),"POST","1.1");
            tx.addRequestHeader("Host","baseline.invalid");
            if(!contentType.empty()) tx.addRequestHeader("Content-Type",contentType);
            tx.addRequestHeader("Content-Length",std::to_string(body.size()));
            tx.processRequestHeaders();
            std::cout<<"TRANSACTION\t"<<repeat<<"\n";
            if(mode=="parser") {
                modsecurity::RequestBodyProcessor::JSON parser(&tx);
                if(rules.m_requestBodyJsonDepthLimit.m_set)
                    parser.setMaxDepth(rules.m_requestBodyJsonDepthLimit.m_value);
                std::string err; bool ok=true;
                size_t pos=0; std::istringstream split(chunkLine); size_t n;
                while(split>>n) {
                    if(n>body.size()-pos) throw std::runtime_error("oversized chunk");
                    bool step=parser.processChunk(body.data()+pos,n,&err);
                    std::cout<<"CHUNK\t"<<step<<"\n"; pos+=n;ok=ok&&step;
                    if(!step)break;
                }
                if(ok && pos<body.size()) {
                    bool step=parser.processChunk(body.data()+pos,body.size()-pos,&err);
                    std::cout<<"CHUNK\t"<<step<<"\n";
                }
                dump("PRE_ARGS",tx.m_variableArgs);
                std::cout<<"COMPLETE\t"<<parser.complete(&err)<<"\n";
                value("PARSER_ERROR",err);
            } else {
                if(mode!="absent") {
                    size_t pos=0,n; std::istringstream split(chunkLine);
                    while(split>>n) {
                        if(n>body.size()-pos) throw std::runtime_error("oversized chunk");
                        std::cout<<"APPEND\t"<<tx.appendRequestBody(reinterpret_cast<const unsigned char*>(body.data()+pos),n)<<"\n";
                        pos+=n;
                    }
                    if(pos<body.size() || body.empty())
                        std::cout<<"APPEND\t"<<tx.appendRequestBody(reinterpret_cast<const unsigned char*>(body.data()+pos),body.size()-pos)<<"\n";
                }
                modsecurity::ModSecurityIntervention appendIt{}; appendIt.status=200;
                tx.intervention(&appendIt);
                std::cout<<"APPEND_INTERVENTION\t"<<appendIt.status<<"\n";
                free(appendIt.url);free(appendIt.log);
                std::cout<<"PROCESS\t"<<tx.processRequestBody()<<"\n";
            }
            dump("NAMES",tx.m_variableArgsNames);
            dump("ARGS",tx.m_variableArgs);dump("GET",tx.m_variableArgsGet);dump("POST",tx.m_variableArgsPost);
            value("ERROR",tx.m_variableReqbodyError.m_value);
            value("PROCESSOR_ERROR",tx.m_variableReqbodyProcessorError.m_value);
            value("ERROR_MSG",tx.m_variableReqbodyErrorMsg.m_value);
            value("PROCESSOR_ERROR_MSG",tx.m_variableReqbodyProcessorErrorMsg.m_value);
            value("PROCESSOR",tx.m_variableReqbodyProcessor.m_value);
            value("INBOUND",tx.m_variableInboundDataError.m_value);
            value("REQUEST_BODY",tx.m_variableRequestBody.m_value);
            auto phase=tx.m_collections.m_tx_collection->resolveFirst("phase2");
            value("PHASE2",phase?*phase:"");
            auto matched=tx.m_collections.m_tx_collection->resolveFirst("value_seen");
            value("VALUE_SEEN",matched?*matched:"");
            modsecurity::ModSecurityIntervention it{};it.status=200;
            bool intervention=tx.intervention(&it);
            std::cout<<"INTERVENTION\t"<<intervention<<"\t"<<it.status<<"\n";
            free(it.url);free(it.log);
        }
#endif
        return 0;
    } catch(const std::exception& e) { std::cerr<<"HARNESS ERROR: "<<e.what()<<"\n";return 2; }
}

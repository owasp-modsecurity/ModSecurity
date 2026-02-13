#include <iostream>

#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include <modsecurity/rules_set.h>
#include <modsecurity/rule_message.h>
#include "rules_cc/cc/runfiles/runfiles.h"


void ModsecurityTest(const char* argv0);

static const std::string GetTestConfPath(const char* argv0) {
  std::string error;
  std::unique_ptr<rules_cc::cc::runfiles::Runfiles> runfiles(
      rules_cc::cc::runfiles::Runfiles::Create(argv0, BAZEL_CURRENT_REPOSITORY,
                                               &error));

  if (!runfiles) {
    std::cerr << "Failed to create Runfiles: " << error << "\n";
    return "";
  }
  std::string repo_part = BAZEL_CURRENT_REPOSITORY;
  if (repo_part.empty()) {
    repo_part = "_main";
  }

  std::string logical_path =
      repo_part + "/examples/bazel/basic_rules.conf";
  std::string physical_path = runfiles->Rlocation(logical_path);

  if (physical_path.empty()) {
    return "";
  }

  return physical_path;
}


int main(int argc, char* argv[])
{

    std::cout << "Hello world!" << std::endl;
    std::cout << "Test project for modsec windows is here!\n";
    try
    {
        ModsecurityTest(argv[0]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return 0;
}

void ModsecurityTest(const char* argv0)
{
    std::cout << "Starting ModSecurity test..." << std::endl;
    std::cout << "About to create ModSecurity instance..." << std::endl;
    std::cout.flush();
    auto modsec = std::make_unique<modsecurity::ModSecurity>();
    std::cout << "ModSecurity instance created." << std::endl;
    std::cout << "ModSecurity whoAmI: " << modsec->whoAmI() << std::endl;

    std::cout << "About to create RulesSet..." << std::endl;
    std::cout.flush();
    auto rules = std::make_unique<modsecurity::RulesSet>();
    std::cout << "RulesSet created." << std::endl;
    
    std::string confPath = GetTestConfPath(argv0);
    std::cout << "Config path: " << confPath << std::endl;
    std::cout.flush();
    
    if (rules->loadFromUri(confPath.c_str()) < 0)
    {
        std::cout<< "Failed to load rules from: " << confPath << std::endl;
        std::cerr << "Problems loading the rules..." << std::endl;
        std::cerr << rules->m_parserError.str() << std::endl;
        return;
    }

    std::cout<< "Rules loaded successfully from: " << confPath << std::endl;
    auto modsecTransaction = std::make_unique<modsecurity::Transaction>(modsec.get(), rules.get(), nullptr);
    modsecTransaction->processConnection("127.0.0.1", 12345, "127.0.0.1", 80);
    modsecTransaction->processURI(
        "https://www.modsecurity.org/test?foo=herewego",
        "GET", "1.1");

    modsecTransaction->addRequestHeader("User-Agent",
                                        "Basic ModSecurity example");
    modsecTransaction->addRequestHeader("Jacob",
                                        "matched-header");
    modsecTransaction->processRequestHeaders();
    modsecTransaction->processRequestBody();

    modsecTransaction->addResponseHeader("HTTP/1.1",
                                         "200 OK");
    modsecTransaction->processResponseHeaders(200, "HTTP 1.2");
    modsecTransaction->processResponseBody();

    modsecTransaction->processLogging();

    for (const auto &x : modsecTransaction->m_rulesMessages)
    {
        std::cout << std::to_string(x.m_rule.m_ruleId) << " " << x.m_message << std::endl;
    }
}
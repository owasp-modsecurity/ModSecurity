# Introduction

libmodsecurity is a library which provides the functionalities of ModSecurity
to be used on your application. It is capable to load/interpret rules on the
ModSecurity SecRules format and apply them on the HTTP content informed by your
application.

Currently the libmodsecurity is used by the ModSecurity nginx connector
(ModSecurity-nginx).

Notice: This project is under development and it is not ready to be placed in
production yet. It is not supporting all the operators and/or actions of the
SecRules languages yet.


### What is the difference between this project and the old ModSecurity (v2.x.x)?

* All Apache dependences were removed
* Higher performance
* New features

ModSecurity project started as just an Apache module, and as the project had
been extending to be used by other platforms (nginx, IIS and others) it became
necessary to remove the Apache dependencies, making the project more platform
independent.

libmodsecurity is no longer dependent on the Apache web server (both
compilation and runtime), and it also performs a way better. This project adds
also some new features, for instance, further it will be expended to support
audit logs into a JSON format.


### It is no longer an Apache module.

This branch does not contain the ModSecurity module for Apache, nginx or IIS,
instead it contains a library which can be consumed by connectors that can
interface with your web server, for instance the ModSecurity-nginx
(https://github.com/SpiderLabs/ModSecurity-nginx) project.

The connectors are separated projects with different release cycles, issues and
development trees.

# Compilation

Before starting the compilation process, make sure that you have all the
dependencies in place. Read the subsection “Dependencies”  for further
information.

After the compilation make sure that there are no issues on your
build/platform. We strongly recommend the utilization of the unit tests and
regression tests. The test utilities are located under the subfolder ‘tests’.

As a dynamic library, don’t forget that it has to be installed in a folder
where your O.S. will look for dynamic libraries in it.

### Unix (Linux, MacOS, FreeBSD, …)

On unix the project uses autotools to help the compilation process.

```shell
$ ./build.sh
$ ./configure
$ make
$ sudo make install
```

### Windows

Windows build is not ready yet.


## Dependencies

This library is written in C++ using the C++11 standards. It also uses the Flex
and Yacc to produce the “Sec Rules Language” parser. Another dependency that is
mandatory is the YAJL, as ModSecurity uses JSON for logging, and testing
framework. 

Others dependencies are operators related and may not be mandatory. For
instance: libinjection is needed for the operator @detectXSS, also curl is
needed for the SecRemoteRules. If those libraries are missing ModSecurity will
be compiled without the support for the operator @detectXSS and the
configuration directive SecRemoteRules. 

# Library documentation

The library documentation is written within the code in Doxygen format. Use
doxygen utility with the configuration file “doxygen.cfg” inside the "doc/"
subfolder to generate HTML output of the documentation, including examples.

# Library utilization

The library provides a C++ and C interface. Some resources are currently only
available to the C++, for instance: the capability to create custom logging
mechanism (see the regression test to check how those logging mechanism works).
The objective is to have both APIs (C, C++) providing the same functionalities,
if you are missing something, please report.

Inside the subfolder examples, there are simple examples on how to use the API.
Below some are illustrated:

###  Simple example using C++

```c++
using ModSecurity::ModSecurity;
using ModSecurity::Rules;
using ModSecurity::Assay;

ModSecurity *modsec;
ModSecurity::Rules *rules;

modsec = new ModSecurity();

rules = new Rules();

rules->loadFromUri(rules_file);

Assay *modsecAssay = new Assay(modsec, rules);

modsecAssay->processConnection("127.0.0.1");
if (modsecAssay->intervention()) {
   std::cout << "There is an intervention" << std::endl;
}
```

### Simple example using C

```c
#include "modsecurity/modsecurity.h"
#include "modsecurity/assay.h"


char main_rule_uri[] = "basic_rules.conf";

int main (int argc, char **argv)
{
    ModSecurity *modsec = NULL;
    Assay *assay = NULL;
    Rules *rules = NULL;

    modsec = msc_init();

    rules = msc_create_rules_set();
    msc_rules_add_file(rules, main_rule_uri);

    assay = msc_new_assay(modsec, rules);

    msc_process_connection(assay, "127.0.0.1");
    msc_process_uri(assay, "http://www.modsecurity.org/test?key1=value1&key2=value2&key3=value3&test=args&test=test");
    msc_process_request_headers(assay);
    msc_process_request_body(assay);
    msc_process_response_headers(assay);
    msc_process_response_body(assay);

    return 0;
}

```

# Contributing

You are more than welcome to contribute with the growth of our community. New
functionalities, fixes, bug report, support beginners users, anything that you
are willing to help.

## Providing patches

We prefer to have your patch within the GtiHub infrastructure to facilitate our
review work, and our Q.A. integration. GitHub provides an excellent
documentation on how to perform “Pull Requests”, more information available
here: https://help.github.com/articles/using-pull-requests/

Please respect the coding style. Pull requests can include various commits, so
provide one fix or one functionality per commit. Do not change anything outside
the scope of our target work (e.g. coding style in a function that you have
passed by). For further information about the coding style used in this
project, please check: https://www.chromium.org/blink/coding-style

Provides explanative commit messages. First line gives the highlight of your
patch, 3rd and on give a more detailed explanation/technical details about your
patch. Patch explanation is valuable during the review process.

### Don’t know where to start?

Within our code there are various items marked as TODO or FIXME that may need
your attention. Check the list of items by performing a grep:

```
$ cd /path/to/modsecurity-nginx
$ egrep -Rin "TODO|FIXME" -R *
```

A TODO list is also available as part of the Doxygen documentation. 

### Testing your patch

Along with the manual testing, we strongly recommend you to use the our
regression tests and unit tests. If you have implemented an operator, don’t
forget to create unit tests for it. Anything else, it is recommended to have a
regression test for it.

The regression test and unit test utilities are native and do not demand any
external tool or script, although you need to fetch the test cases from other
repositories as they are shared with other versions of ModSecurity, those
others repositories git submodules. To fetch the submodules repository and run
the utilities, follow the commands listed below:

```shell
$ cd /path/to/your/ModSecurity
$ git submodule foreach git pull
$ cd tests
$ ./regression-tests
$ ./unit-tests
 ```

### Debugging 


Before start the debugging process, make sure where your bug is. The problem
could be on your connector or in libmodsecurity. In order to identify where the
bug is, it is recommended the creation of a regression test that mimic the
scenario where the bug is happening. If the bug is reproducible with the
regression-test utility, then it will be a way simple to debug. On Linux it is
recommended the utilization of gdb and/or valgrind.

During the configuration/compilation time, you may want to disable the compiler
optimization making your “back traces” populated with readable data. Use the
CFLAGS to disable the compilation optimization parameters:

```shell
$ export CFLAGS=”-g -02”
$ ./build.sh
$ ./configure
$ make
$ sudo make install
```


## Reporting Issues

If you are facing a configuration issue or something is not working as you
expected to be, please use ModSecurity user’s mailing list. Issues on GitHub
are also welcomed, but prefer to have user questions on the mailing list first,
where you can reach an entire community. Also don’t forget to look for an
existent issue before open a new one.

If you are willing to open an issue on GitHub, don’t forget to tell us the
version of your libmodsecurity and the version of a specific connector if there
is one.


### Security issue

Please do not make public any security issue. Contact us at:
security@modsecurity.org reporting the issue. Once the problem is fixed your
credits will be given.

## Feature request

It will be a pleasure to discuss any feature request with the community, also,
feel free to open GitHub issues requesting for new features. Before opening a
new issue, please check if there is one already opened with the same subject.

## Packing

Having our packages in distros on time is a desire that we have, so let us know
if there is anything we can do to facilitate your work as a packager.





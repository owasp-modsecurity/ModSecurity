# Building ModSecurity

## Branches
Two branches currently used for ModSecurity development and sustaining are `waf_nginx` (the default branch) and `waf_iis`. 
They follow the following rules for code changes:

 - All Nginx-specific fixes go to `waf_nginx` and so pull requests should be opened for this branch (happens by default)
 - All IIS-specific fixes go to `waf_iis` and so pull requests should be opened for this branch. Make sure to change your branch for your PR is your changes apply to ModSecurity IIS only.
 - All common fixes that apply to both Nginx and IIS should be merged to both branches. For these, you can create your PR against either of `waf_nginx` and `waf_iis` (typically, the one that you have worked on and tested with) and then cherry-picked into another one.

NB: the `v2/master` and `iis_maintenance` branches that have been used for development before are now effectively deprecated. 

## Building ModSecurity For IIS

Open a `PowerShell` command line to follow the steps below.

 1. Clone ModSecurity repository along with all registered submodules:
    ```bash
    git clone --recursive https://github.com/Microsoft/ModSecurity
    ```
 2. Switch to `waf_iis` branch:
     ```bash
    git checkout --recurse-submodules waf_iis
    ```
 3.  Change to the `iis` subdirectory:
     ```bash
     cd iis
     ```
 4. Download dependencies using helper PowerShell script:
     ```bash
     .\getModSecurityPkgs.ps1 https://modsecurity.blob.core.windows.net/windows-artifacts
     ```
 5. Open Visual Studio 2015 x64 Native Tools Command Prompt, change to `iis` subdirectory and start the build:
     ```bash
     cd iis
     .\build_release_amd64.bat
     ```
 
 This will produce the binaries and place them into `ModSecurity\iis\release\amd64` directory.

If you only make changes to ModSecurity and not to any of its dependencies, you can later only rebuild it with `build_modsecurity.bat`. You still have to use `build_release_amd64` for the first time to get all the dependencies built.

## Building ModSecurity For Nginx
The steps below are for Ubuntu 16.04 LTS.

 1. Install required dependencies:
	 ```
	 apt-get update && apt-get install -y  \
    vim \
    curl \
    apt-transport-https \
    build-essential \
    wget \
    automake\
    unzip\
    m4 \
    git \
    libtool \
    cmake \
    apache2-dev \
    g++-4.8 \
    libaprutil1 \
    libazurepal-dev \
    libazurepal \
    libcurl4-openssl-dev \
    libexpat-dev\
    libgd2-xpm-dev \
    liblua5.1-dev \
    libluajit-5.1-dev \
    libpcre3-dev \
    libperl-dev \
    libunwind8 \
    libyajl-dev \
    libxml2\
    libxml2-dev\
    metricsext \
    unzip \
    uuid-dev \
    openssh-server \
    unzip \
    uuid-runtime \
    gdb
    # Install jsoncons 0.122
    mkdir -p /tmp/jsoncons/
    curl -o /tmp/jsoncons/jsoncons-0.122.0.zip -OL https://github.com/danielaparker/jsoncons/archive/v0.122.0.zip
    unzip /tmp/jsoncons/jsoncons-0.122.0.zip -d /tmp/jsoncons
    cp -r /tmp/jsoncons/jsoncons-0.122.0/include/jsoncons /usr/include/
	```

 3. Clone ModSecurity repository along with all registered submodules:
    ```bash
    git clone --recursive https://github.com/Microsoft/ModSecurity
    ```
 4. Make sure you're in `waf_nginx` branch:
     ```bash
    git status
    ```
 5.  Configure and build ModSecurity:
	 ```bash
	 ./autogen.sh
	 ./configure LDFLAGS=-g  CXXFLAGS="-std=c++14 " CFLAGS="-fPIC " --enable-standalone-module --disable-mlogc --enable-waf_json_logging
	 make
	 ```

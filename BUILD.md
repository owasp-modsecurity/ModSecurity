# Building ModSecurity

## Branches
Two branches currently used for ModSecurity development and sustaining are `v2/master` and `iis_maintenance`. 
All new fixes go to `v2/master` and so pull requests should be opened for this branch. 
General changes and changes that are specific to ModSecurity for IIS are merged from `v2/master` to `iis_maintenance`. Fixes specific to ModSecurity for Nginx are not propagated from `v2/master`.

## Building ModSecurity For IIS

Open a `PowerShell` command line to follow the steps below.

 1. Clone ModSecurity repository along with all registered submodules:
    ```bash
    git clone --recursive https://github.com/Microsoft/ModSecurity
    ```
 2. Switch to `iis_maintenance` branch:
     ```bash
    git checkout --recurse-submodules iis_maintenance
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
 4. Make sure you're in `v2/master` branch:
     ```bash
    git status
    ```
 5.  Configure and build ModSecurity:
	 ```bash
	 ./autogen.sh
	 ./configure LDFLAGS=-g  CXXFLAGS="-std=c++11 " CFLAGS="-fPIC " --enable-standalone-module --disable-mlogc --enable-waf_json_logging
	 make
	 ```

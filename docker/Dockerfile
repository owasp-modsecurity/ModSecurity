FROM  ubuntu:16.04

RUN apt-get update
RUN apt-get install -y  \
    automake\
    build-essential \
    curl \
    unzip\
    m4 \
    libtool

# Install jsoncons 
RUN mkdir -p /tmp/jsoncons/
RUN curl -o /tmp/jsoncons/jsoncons-0.122.0.zip -OL https://github.com/danielaparker/jsoncons/archive/v0.122.0.zip
RUN unzip /tmp/jsoncons/jsoncons-0.122.0.zip -d /tmp/jsoncons
RUN cp -r /tmp/jsoncons/jsoncons-0.122.0/include/jsoncons /usr/include/

RUN apt-get install -y \
        apache2-dev \
        g++-4.8 \
        libaprutil1 \
        libcurl4-openssl-dev \
        liblua5.1-dev \
        libluajit-5.1-dev \
        libpcre3-dev \
        libperl-dev \
        libunwind8 \
        libyajl-dev \
        libxml2\
        libxml2-dev\
        unzip

RUN apt clean \
    && rm -rf /var/lib/apt/lists/*

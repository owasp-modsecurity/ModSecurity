FROM  ubuntu:16.04

RUN apt-get update
RUN apt-get install -y  \
    automake\
    build-essential \
    curl \
    unzip\
    m4 \
    libtool

# Install google protobuf 3.5.1
RUN mkdir -p /tmp/protobuf/
RUN curl -o /tmp/protobuf/protobuf-3.5.1.zip -OL https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.zip
RUN unzip /tmp/protobuf/protobuf-3.5.1.zip -d /tmp/protobuf/
WORKDIR /tmp/protobuf/protobuf-3.5.1
RUN ./autogen.sh
RUN ./configure --prefix=/usr
RUN  make && make install

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

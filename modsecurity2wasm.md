# Build ModSecurity WASM Library

This article will guide you to build your own ModSecurity WASM library using [Emscripten](https://emscripten.org/) toolchain.

## Pre-requirements

### Install Emscripten

You can refer to the following steps to install the latest `Emscripten`.

```shell
# Get the emsdk repo
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory
cd emsdk

# Fetch the latest version of the emsdk (not needed the first time you clone)
git pull

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
```



### `wasi-sdk` setup

- Download 

  ```shell
  wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz
  ```

- Export it to `/opt/wasi-sdk`

- Configure

  ```shell
  export WASI_SDK_PATH="/opt/wasi-sdk"
  ```



### Build PCRE WASM library

```shell
# Get the pcre library source code
git clone https://github.com/maxfierke/libpcre.git -b mf-wasm32-wasi-cross-compile

cd libpcre
# This should compile successfully and place the compiled .a static library in targets/wasm32-wasi
Run ./build_for_crystal.sh. 

# Copy the wams library to target directory
cp targets/wasm32-wasi/*.a /usr/local/pcre
```



## Configure and build ModSecurity

```shell
# This is version for WASM ModSecurity
git clone https://github.com/leyao-daily/ModSecurity.git

cd ModSecurity
# Build the configuration script
./build.sh

# Download the submodule
git submodule init
git submodule update

# Configure ModSecurity with core functions
emconfigure ./configure --without-yajl --without-geoip --without-libxml --without-curl --disable-shared --disable-examples --disable-libtool-lock --disable-debug-logs --with-pcre=./pcre-config

# Build the library
emmake make -j <num_cpus>

# Install the library
emmake make install

```



## Build your own wasm application

```sehll
emcc test.cc -L/usr/local/modsecurity/lib/ -lmodsecurity -L/usr/local/pcre/ -lpcre -o test.wasm -I/usr/local/modsecurity/include/
```


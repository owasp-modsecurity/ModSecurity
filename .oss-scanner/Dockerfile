# Build environment for Anthropic's OSS Scanner (https://github.com/anthropics/oss-scanner).
# The scanner builds this image with the repository checkout as the build context, then
# analyses it with no network access, so everything the build and the tests need is fetched
# here. Keep the package list aligned with the build-linux job in .github/workflows/ci_new.yml.
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates git build-essential clang autoconf automake libtool pkg-config \
      bison flex python3 gdb valgrind \
      libyajl-dev libcurl4-openssl-dev liblmdb-dev liblua5.4-dev libmaxminddb-dev \
      libpcre2-dev pcre2-utils libpcre3-dev libxml2-dev libfuzzy-dev \
    && rm -rf /var/lib/apt/lists/*

# scanner contract: the checkout lives inside the image, at /src
COPY . /src
WORKDIR /src

# others/libinjection, others/mbedtls and the secrules-language-tests corpus are submodules
# and are required; this is a no-op if the checkout already has them
RUN git config --global --add safe.directory '*' \
    && git submodule update --init --recursive

# The OWASP CRS v4 (latest release), for checking a finding against a default CRS deployment;
# see threat_model.md
RUN tag=$(git ls-remote --tags --refs https://github.com/coreruleset/coreruleset 'v4.*' \
          | sed 's#.*refs/tags/##' | grep -E '^v4\.[0-9]+\.[0-9]+$' | sort -V | tail -n 1) \
    && git clone --depth 1 --branch "$tag" https://github.com/coreruleset/coreruleset /opt/coreruleset

# Build as CI does (assertions on, debug logs on by default), with debug info and frame
# pointers. A failing step fails the build.
RUN ./build.sh \
    && ./configure --enable-assertions=yes --with-lmdb \
         CFLAGS="-g -O1 -fno-omit-frame-pointer" CXXFLAGS="-g -O1 -fno-omit-frame-pointer" \
    && make -j"$(nproc)"

# Run the test suite so the image is known to work, but do not let a failing test abort the build.
RUN make check -j"$(nproc)" || echo "WARNING: make check reported failures"

# Build environment for Anthropic's OSS Scanner (https://github.com/anthropics/oss-scanner).
# The scanner builds this image with the repository checkout as the build context, then
# analyses it with no network access, so everything the build and the tests need is fetched
# here. Keep the package list aligned with the test jobs in .github/workflows/ci.yml.
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates git build-essential clang autoconf automake libtool pkg-config \
      perl libwww-perl curl gdb valgrind \
      apache2 apache2-bin apache2-data apache2-dev \
      libxml2-dev liblua5.4-dev libcurl4-gnutls-dev libpcre2-dev libyajl-dev \
      ssdeep libfuzzy-dev \
    && rm -rf /var/lib/apt/lists/*

# scanner contract: the checkout lives inside the image, at /src
COPY . /src
WORKDIR /src

# apache2/others/libinjection is a submodule and is required; this is a no-op if the checkout
# already has it
RUN git config --global --add safe.directory '*' \
    && git submodule update --init --recursive

# The OWASP CRS v4 (latest release), for checking a finding against a default CRS deployment;
# see threat_model.md
RUN tag=$(git ls-remote --tags --refs https://github.com/coreruleset/coreruleset 'v4.*' \
          | sed 's#.*refs/tags/##' | grep -E '^v4\.[0-9]+\.[0-9]+$' | sort -V | tail -n 1) \
    && git clone --depth 1 --branch "$tag" https://github.com/coreruleset/coreruleset /opt/coreruleset

# Build as CI does, with debug info and frame pointers, and install mod_security2.so into the
# system Apache. A failing step fails the build.
RUN ./autogen.sh \
    && ./configure --enable-assertions --with-lua \
         CFLAGS="-g -O1 -fno-omit-frame-pointer -Werror=format-security" \
    && make -j"$(nproc)" \
    && make install

# Run the unit tests and the regression tests (the latter start their own httpd on port 8088,
# as an unprivileged user, as in CI), but do not let a failing test abort the build.
RUN make test || echo "WARNING: make test reported failures"
RUN useradd --create-home builder \
    && chown -R builder /src/tests \
    && (runuser -u builder -- make test-regression || echo "WARNING: make test-regression reported failures")

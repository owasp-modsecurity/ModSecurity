#!/bin/sh

set -e  # bei Fehler abbrechen

rm -rf autom4te.cache
rm -f aclocal.m4

##
## 1. headers.mk erzeugen (wie bisher)
##
cd src
rm -f headers.mk
echo "noinst_HEADERS = \\" > headers.mk
ls -1 \
    actions/*.h \
    actions/ctl/*.h \
    actions/data/*.h \
    actions/disruptive/*.h \
    actions/transformations/*.h \
    debug_log/*.h \
    audit_log/writer/*.h \
    collection/backend/*.h \
    operators/*.h \
    parser/*.h \
    request_body_processor/*.h \
    utils/*.h \
    variables/*.h \
    engine/*.h \
    *.h | tr "\012" " " >> headers.mk
cd ..

##
## 2. Vendored Mbed TLS bauen – MIT PROGRAMMEN UND TESTS
##
if [ -d "others/mbedtls" ]; then
  echo "==> Building vendored Mbed TLS (mit Programmen und Tests)..."
  (
    cd others/mbedtls

    mkdir -p build

    # Laut offizieller CMake-Options:
    # - ENABLE_PROGRAMS=ON  -> Tools/Beispiele bauen
    # - ENABLE_TESTING=ON   -> Test-Binaries + CTest
    cmake -S . -B build \
      -DENABLE_PROGRAMS=ON \
      -DENABLE_TESTING=ON

    cmake --build build --config Release

    echo "==> Running Mbed TLS tests..."
    cd build
    ctest --output-on-failure
  )
else
  echo "WARNUNG: others/mbedtls nicht gefunden – Mbed TLS wird NICHT gebaut/getestet!"
fi

##
## 3. Autotools für ModSecurity initialisieren
##
case `uname` in
  Darwin*) glibtoolize --force --copy ;;
  *)       libtoolize  --force --copy ;;
esac

autoreconf --install
autoheader
automake --add-missing --foreign --copy --force-missing
autoconf --force
rm -rf autom4te.cache

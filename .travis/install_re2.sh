#!/bin/sh

# Ubuntu 14.04 doesn't have RE2 package altogether,
# and Ubuntu 16.04 RE2 package is too old.
#
# Ubuntu 18.04 RE2 package might work, but this Ubuntu verison it's not
# supported by Travis yet.

RELEASE=2019-01-01

set -ex

cd /tmp
wget https://github.com/google/re2/archive/$RELEASE.tar.gz -O re2.tar.gz
tar -xf re2.tar.gz
(cd re2-$RELEASE && make && sudo make install)

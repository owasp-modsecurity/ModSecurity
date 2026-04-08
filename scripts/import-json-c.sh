#!/usr/bin/env bash
set -euo pipefail
cat <<MSG
Use your distro package or vendor a json-c release into third_party/json-c.
Recommended upstream: https://github.com/json-c/json-c (>=0.16).
This repository does not auto-vendor json-c sources yet because json-c requires
multiple generated/build artifacts depending on your toolchain.
MSG

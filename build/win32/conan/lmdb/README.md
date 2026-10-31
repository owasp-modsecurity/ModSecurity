# Local LMDB recipe for Windows builds

`vcbuild.bat` exports this recipe as `lmdb/0.9.32@modsecurity/ci`, and the Windows consumer selects that reference. The library version remains 0.9.32. The separate reference leaves the ConanCenter recipe in the user's cache untouched.

## Why this copy is needed

The ConanCenter source archive at `git.openldap.org` returns HTTP 429 during Windows dependency builds, including after ten retries with a 30-second delay. This recipe tries that original URL and its original checksum first, retaining Conan's configured download retries. Only after an HTTP 429 failure does it download the same source revision from the official [LMDB GitHub mirror](https://github.com/LMDB/lmdb). Checksum, extraction and other network errors remain fatal.

The fallback URL pins commit `e96d8dfa3951c06062a6fffbe1d7a2f3c9c1ff76`, which is the target of the annotated `LMDB_0.9.32` tag in both the [LMDB mirror](https://github.com/LMDB/lmdb/releases/tag/LMDB_0.9.32) and the [OpenLDAP mirror](https://github.com/openldap/openldap/releases/tag/LMDB_0.9.32). Each archive has its own SHA-256 checksum because its metadata and root directory differ.

Conan 2.33 reports HTTP 429 through a `ConanException` message without a structured status code. The recipe matches that exact error for the original URL. An unrecognized error is re-raised. The regression tests run against the pinned Conan version with a local HTTP server, including exhausted retries and rejected checksums:

```sh
python -m unittest discover -s build/win32/conan/lmdb/tests -v
```

Install Conan in the same Python environment before running the tests. The Windows CI runs them for the full configuration on both runners.

## Provenance and removal

The recipe and `CMakeLists.txt` come from [ConanCenter revision 4c4ac13f1ee7ce578941859ae3d2feff7189796d](https://github.com/conan-io/conan-center-index/tree/4c4ac13f1ee7ce578941859ae3d2feff7189796d/recipes/lmdb/all). `CMakeLists.txt` is unchanged; `conanfile.py` adds only the guarded source fallback. `conandata.yml` retains the original 0.9.32 URL and checksum, with a separate fallback URL and checksum.

Once ConanCenter provides a reliable source for the same supported version, switch the consumer back to its ConanCenter reference and remove this directory and its export from `vcbuild.bat`. Validate both Windows CI runners and the x86/x64 Release/Debug release-packaging matrix before removing the override.

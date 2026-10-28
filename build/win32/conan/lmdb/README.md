# Local LMDB recipe for Windows builds

`vcbuild.bat` exports this recipe as `lmdb/0.9.32@modsecurity/ci`, and the Windows consumer selects that reference. The library version remains 0.9.32. The separate reference leaves the ConanCenter recipe in the user's cache untouched.

## Why this copy is needed

The ConanCenter source archive at `git.openldap.org` returns HTTP 429 during Windows dependency builds, including after ten retries with a 30-second delay. This copy downloads the same source revision from the official [LMDB GitHub mirror](https://github.com/LMDB/lmdb).

The URL pins commit `e96d8dfa3951c06062a6fffbe1d7a2f3c9c1ff76`, which is the target of the annotated `LMDB_0.9.32` tag in both the [LMDB mirror](https://github.com/LMDB/lmdb/releases/tag/LMDB_0.9.32) and the [OpenLDAP mirror](https://github.com/openldap/openldap/releases/tag/LMDB_0.9.32). The archive is verified with SHA-256. Its checksum differs from the GitLab archive because the archive metadata and root directory differ.

## Provenance and removal

`conanfile.py` and `CMakeLists.txt` are copied without changes from [ConanCenter revision 4c4ac13f1ee7ce578941859ae3d2feff7189796d](https://github.com/conan-io/conan-center-index/tree/4c4ac13f1ee7ce578941859ae3d2feff7189796d/recipes/lmdb/all). `conandata.yml` retains only 0.9.32 and changes its source URL and checksum.

Once ConanCenter provides a reliable source for the same supported version, switch the consumer back to its ConanCenter reference and remove this directory and its export from `vcbuild.bat`. Validate both Windows CI runners and the x86/x64 Release/Debug release-packaging matrix before removing the override.

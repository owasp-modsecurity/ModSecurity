from conan import ConanFile
from conan.tools.cmake import cmake_layout

required_conan_version = ">=2.0 <3"


class ModSecurityConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = (
        "yajl/2.1.0@modsecurity/ci",
        "pcre2/10.44",
        "libxml2/2.15.2",
        "lua/5.5.0",
        "libcurl/8.19.0",
        "lmdb/0.9.32@modsecurity/ci",
        "libmaxminddb/1.12.2",
        "dirent/1.24",
        "poco/1.14.2",
    )

    def layout(self):
        cmake_layout(self)
        # Preserve the paths used by vcbuild.bat and the Windows CI tests.
        self.folders.build = "build"
        self.folders.generators = "build/generators"

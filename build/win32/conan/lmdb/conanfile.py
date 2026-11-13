from conan import ConanFile
from conan.errors import ConanException
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get
from conan.tools.scm import Version
import os

required_conan_version = ">=2.4"


class lmdbConan(ConanFile):
    name = "lmdb"
    license = "OLDAP-2.8"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://symas.com/lmdb/"
    description = "Fast and compat memory-mapped key-value database"
    topics = ("database", "key-value", "memory-mapped")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_robust_mutex": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_robust_mutex": True,
    }

    exports_sources = ["CMakeLists.txt"]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
        if self.settings.os in ("Linux", "FreeBSD"):
            self.options.enable_robust_mutex = True
        else:
            self.options.enable_robust_mutex = False

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.settings.rm_safe("compiler.libcxx")
        self.settings.rm_safe("compiler.cppstd")

    def layout(self):
        cmake_layout(self, src_folder="src")

    def source(self):
        """Prefer upstream and use the mirror only after exhausted HTTP 429 retries."""
        original = self.conan_data["sources"][self.version]
        try:
            get(self, **original, destination=self.source_folder, strip_root=True)
        except ConanException as error:
            # Conan 2.33 exposes HTTP 429 only through this exact download error.
            # Do not hide checksum, extraction or unrelated network failures.
            if str(error) != f"Error 429 downloading file {original['url']}":
                raise
            self.output.warning("LMDB upstream download is rate limited; using the official GitHub mirror")
            mirror = self.conan_data["fallback_sources"][self.version]
            get(self, **mirror, destination=self.source_folder, strip_root=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["LMDB_SRC_DIR"] = os.path.join(self.source_folder, "libraries", "liblmdb").replace("\\", "/")
        # MDB_USE_ROBUST is only consulted where LMDB uses POSIX mutexes. Leave it
        # undefined elsewhere: since 1.0 the mere presence of the macro suppresses
        # the POSIX semaphore default on Apple/BSD and falls back to SysV semaphores.
        # Emscripten also uses POSIX mutexes, but does not provide robust mutexes.
        if self.settings.os in ("Linux", "FreeBSD", "Emscripten"):
            tc.variables["LMDB_ENABLE_ROBUST_MUTEX"] = self.options.enable_robust_mutex
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=os.path.join(self.source_folder, os.pardir))
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=os.path.join(self.source_folder, "libraries", "liblmdb"), dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("pkg_config_name", "lmdb")
        self.cpp_info.libs = ["lmdb"]

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["pthread"]
            # Since 1.0 the crypto module loader (module.c) requires dlopen()
            if Version(self.version) >= "1.0.0":
                self.cpp_info.system_libs.append("dl")

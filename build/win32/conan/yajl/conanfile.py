from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import apply_conandata_patches, copy, export_conandata_patches, get, rm, rmdir
import os

# Based on ConanCenter's YAJL recipe at 4c4ac13f1ee7ce578941859ae3d2feff7189796d.
required_conan_version = ">=2.0 <3"


class YAJLConan(ConanFile):
    name = "yajl"
    version = "2.1.0"
    description = "A fast streaming JSON parsing library in C"
    license = "ISC"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://github.com/lloyd/yajl"
    topics = ("json", "encoding", "decoding", "manipulation")
    package_type = "library"
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def export_sources(self):
        export_conandata_patches(self)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.settings.rm_safe("compiler.libcxx")
        self.settings.rm_safe("compiler.cppstd")

    def layout(self):
        cmake_layout(self, src_folder="src")

    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        apply_conandata_patches(self)
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, pattern="COPYING", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "share"))
        # Keep either shared or static lib depending on shared option
        if self.options.shared:
            rm(self, "*yajl_s.*", os.path.join(self.package_folder, "lib"))
        else:
            rm(self, "*yajl.*", os.path.join(self.package_folder, "bin"))
            rm(self, "*yajl.*", os.path.join(self.package_folder, "lib"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "yajl")
        self.cpp_info.set_property("cmake_target_name", "yajl::yajl")
        self.cpp_info.libs = ["yajl"] if self.options.shared else ["yajl_s"]
        if self.options.shared:
            self.cpp_info.defines.append("YAJL_SHARED")

        # https://github.com/lloyd/yajl/blob/5e3a7856e643b4d6410ddc3f84bc2f38174f2872/src/CMakeLists.txt#L64
        self.cpp_info.set_property("pkg_config_name", "yajl")

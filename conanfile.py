from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import load
from conan.tools.apple import is_apple_os
import os
import re

class ViamCppSdkRecipe(ConanFile):
    name = "viam-cpp-sdk"

    license = "Apache-2.0"
    url = "https://github.com/viamrobotics/viam-cpp-sdk/"

    package_type = "library"

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "offline_proto_generation": [True, False],
        "shared": [True, False]
    }

    default_options = {
        "offline_proto_generation": True,
        "shared": True
    }

    exports_sources = "CMakeLists.txt", "LICENSE", "src/*"

    def set_version(self):
        content = load(self, "CMakeLists.txt")
        self.version = re.search("set\(CMAKE_PROJECT_VERSION (.+)\)", content).group(1).strip()

    def configure(self):
        # If we're building static then build the world as static, otherwise
        # stuff will probably break.
        # If you want your shared build to also build the world as shared, you
        # can invoke conan with -o "&:shared=False" -o "*:shared=False",
        # possibly with --build=missing or --build=cascade as desired,
        # but this is probably not necessary.
        if not self.options.shared:
            self.options["*"].shared = False

    def requirements(self):
        self.requires('boost/[>=1.74.0]', transitive_headers=True)

        # The SDK supports older grpc and protobuf, but these are the oldest
        # maintained conan packages.
        self.requires('grpc/[>=1.48.4]', transitive_headers=True)
        self.requires('protobuf/[>=3.17.1]', transitive_headers=True)

        self.requires('xtensor/[>=0.24.3]', transitive_headers=True)
        self.requires('abseil/[>=20230125.3]')

    def build_requirements(self):
        if self.options.offline_proto_generation:
            self.tool_requires('grpc/[>=1.48.4]')
            self.tool_requires('protobuf/[>=3.17.1]')

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)

        tc.cache_variables["VIAMCPPSDK_OFFLINE_PROTO_GENERATION"] = self.options.offline_proto_generation
        tc.cache_variables["VIAMCPPSDK_USE_DYNAMIC_PROTOS"] = True

        tc.cache_variables["VIAMCPPSDK_BUILD_TESTS"] = False
        tc.cache_variables["VIAMCPPSDK_BUILD_EXAMPLES"] = False

        tc.generate()

        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        CMake(self).install()

    def package_info(self):
        self.cpp_info.components["viam_rust_utils"].libs = ["viam_rust_utils"]
        
        self.cpp_info.components["viamsdk"].libs = ["viamsdk"]

        for component in ["viamsdk", "viamapi"]:
           self.cpp_info.components[component].set_property("cmake_target_name", "viam-cpp-sdk::{}".format(component))
           self.cpp_info.components[component].set_property("pkg_config_name", "viam-cpp-sdk-lib{}".format(component))
           self.cpp_info.components[component].requires = ["grpc::grpc++", "protobuf::libprotobuf"]
           if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.components[component].system_libs = ["pthread"]

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.components["viamsdk"].system_libs.extend(["dl", "rt"])

        self.cpp_info.components["viamapi"].includedirs.append("include/viam/api")

        if self.options.shared:
            self.cpp_info.components["viamapi"].libs = ["viamapi"]
        else:
            lib_folder = os.path.join(self.package_folder, "lib")
            lib_fullpath = os.path.join(lib_folder, "libviamapi.a")
            if is_apple_os(self):
                whole_archive = f"-Wl,-force_load,{lib_fullpath}"
            else:
                whole_archive = f"-Wl,--push-state,--whole-archive,{lib_fullpath},--pop-state"
            self.cpp_info.components["viamapi"].exelinkflags.append(whole_archive)
            self.cpp_info.components["viamapi"].sharedlinkflags.append(whole_archive)

        self.cpp_info.components["viamsdk"].requires.extend([
            "viamapi",
            "boost::headers",
            "boost::log",
            "xtensor::xtensor",

            "viam_rust_utils",
            "abseil::absl_strings",
            "grpc::grpc++_reflection"
        ])

        self.cpp_info.components["viamsdk"].frameworks = ["Security"]

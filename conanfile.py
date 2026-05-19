from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.build import valid_max_cppstd
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
        "opentelemetry_tracing": [True, False],
        "shared": [True, False]
    }

    default_options = {
        "offline_proto_generation": True,
        "opentelemetry_tracing": True,
        "shared": False
    }

    exports_sources = "CMakeLists.txt", "LICENSE", "src/*", "buf.lock"

    def set_version(self):
        content = load(self, "CMakeLists.txt")
        self.version = re.search("set\(CMAKE_PROJECT_VERSION (.+)\)", content).group(1).strip()

    def configure(self):
        # Workaround an unfortunately long-standing boost/conan issue which breaks C++20 builds
        self.options["boost"].without_cobalt = True

        if self.options.opentelemetry_tracing:
            self.options["opentelemetry-cpp"].with_otlp_grpc = True
            # Disable every HTTP-based exporter so libcurl never enters the
            # dependency graph; we only use OTLP/gRPC.
            self.options["opentelemetry-cpp"].with_otlp_http = False
            self.options["opentelemetry-cpp"].with_zipkin = False
            self.options["opentelemetry-cpp"].with_elasticsearch = False

        if self.options.shared:
            # See https://github.com/conan-io/conan-center-index/issues/25107
            self.options["grpc"].secure = True

            # From some experiments it seems that the shared-ness of these packages
            # should match that of the SDK recipe. Failure to do so can cause linker
            # errors while compiling, or static initialization errors at runtime for modules.
            for lib in ["grpc", "protobuf", "abseil"]:
                self.options[lib].shared = True

    def validate(self):
        if self.options.opentelemetry_tracing:
            if not self.dependencies["opentelemetry-cpp"].options.with_otlp_grpc:
                raise ConanInvalidConfiguration("opentelemetry_tracing option requires opentelemetry-cpp/*:with_otlp_grpc")



    def _xtensor_requires(self):
        if valid_max_cppstd(self, 14, False):
            return 'xtensor/[>=0.24.3 <0.26.0]'

        return 'xtensor/[>=0.24.3]'

    def _grpc_requires(self):
        if valid_max_cppstd(self, 14, False):
            return 'grpc/[>=1.48.4 <1.70.0]'

        return 'grpc/[>=1.48.4]'

    def requirements(self):
        if self.settings.os == "Windows":
            # We exclude only version 1.88.0 which had an MSVC preprocessor bug affecting some headers
            self.requires('boost/[>=1.74.0 <1.88.0 || >=1.89.0]', transitive_headers=True)
        else:
            self.requires('boost/[>=1.74.0]', transitive_headers=True)

        # The SDK supports older grpc and protobuf, but these are the oldest
        # maintained conan packages.
        self.requires(self._grpc_requires())
        self.requires('protobuf/[>=3.17.1 <6.30.0]')
        self.requires(self._xtensor_requires(), transitive_headers=True)

        if self.options.opentelemetry_tracing:
            # Oldest maintained conan package and first version with proper CMake support
            if self.settings.os == "Windows":
                # v1.25+ builds opentelemetry_proto as a DLL on Windows without
                # exporting its protobuf-generated globals, breaking consumer link.
                self.requires('opentelemetry-cpp/[>=1.21.0 <1.25.0]')
            else:
                self.requires('opentelemetry-cpp/[>=1.21.0]')

    def build_requirements(self):
        if self.options.offline_proto_generation:
            self.tool_requires(self._grpc_requires())
            self.tool_requires('protobuf/<host_version>')

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)

        tc.cache_variables["VIAMCPPSDK_OFFLINE_PROTO_GENERATION"] = self.options.offline_proto_generation
        tc.cache_variables["VIAMCPPSDK_OPENTELEMETRY_TRACING"] = self.options.opentelemetry_tracing
        tc.cache_variables["VIAMCPPSDK_USE_DYNAMIC_PROTOS"] = True

        # We don't want to constrain these for conan builds because we
        # don't know the context where we might be being built. We
        # should permit the build if it works. Also, even the C++ SDK
        # is warnings clean on the modern compilers we use in CI, it,
        # or headers from its dependencies, might throw warnings with
        # older compilers, and we should still allow a build there.
        tc.cache_variables["VIAMCPPSDK_ENFORCE_COMPILER_MINIMA"] = False
        tc.cache_variables["VIAMCPPSDK_USE_WALL_WERROR"] = False

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
            self.cpp_info.components["viam_rust_utils"].system_libs.append("rt")
        elif self.settings.os == "Windows":
            self.cpp_info.components["viamsdk"].system_libs.extend(["ncrypt", "secur32", "ntdll", "userenv"])

        self.cpp_info.components["viamapi"].includedirs.append("include/viam/api")

        if self.options.opentelemetry_tracing:
            self.cpp_info.components["viamapi"].requires.append(
                "opentelemetry-cpp::opentelemetry_proto"
            )

        if self.options.shared:
            self.cpp_info.components["viamapi"].libs = ["viamapi"]
        else:
            lib_folder = os.path.join(self.package_folder, "lib")
            lib_fullpath = os.path.join(lib_folder, "libviamapi.a")
            if self.settings.os == "Windows":
                lib_fullpath = os.path.join(lib_folder, "viamapi.lib")

            if is_apple_os(self):
                whole_archive = f"-Wl,-force_load,{lib_fullpath}"
            elif self.settings.os == "Windows":
                whole_archive = f"/WHOLEARCHIVE:{lib_fullpath}"
            else:
                whole_archive = f"-Wl,--push-state,--whole-archive,{lib_fullpath},--pop-state"

            self.cpp_info.components["viamapi"].exelinkflags.append(whole_archive)
            self.cpp_info.components["viamapi"].sharedlinkflags.append(whole_archive)

        self.cpp_info.components["viamsdk"].requires.extend([
            "boost::headers",
            "boost::log",
            "grpc::grpc++_reflection",
            "xtensor::xtensor",
            "viamapi",
        ])

        if self.options.opentelemetry_tracing:
            self.cpp_info.components["viamsdk"].requires.extend([
                "opentelemetry-cpp::opentelemetry_trace",
                "opentelemetry-cpp::opentelemetry_exporter_otlp_grpc",
            ])

        self.cpp_info.components["viamsdk"].requires.extend([
            "viam_rust_utils"
        ])

        self.cpp_info.components["viamsdk"].frameworks = ["Security"]

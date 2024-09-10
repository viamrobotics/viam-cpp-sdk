from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class ViamCppSdkRecipe(ConanFile):
    name = "viam-cpp-sdk"
    version = "0.0.10"

    license = "Apache-2.0"
    url = "https://github.com/viamrobotics/viam-cpp-sdk/"

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "offline_proto_generation": [True, False],
        "use_dynamic_protos": [True, False]
    }

    default_options = {
        "offline_proto_generation": False,
        "use_dynamic_protos": True
    }

    exports_sources = "CMakeLists.txt", "LICENSE", "src/*"

    def requirements(self):
        self.requires('boost/[>=1.74.0]')

        # The SDK supports older grpc and protobuf, but these are the oldest
        # maintained conan packages.
        self.requires('grpc/[>=1.48.4]')
        self.requires('protobuf/[>=3.17.1]')

        self.requires('xtensor/[>=0.24.3]')
        self.requires('abseil/[>=20230125.3]')

    def build_requirements(self):
        if not self.options.offline_proto_generation:
            self.tool_requires('grpc/[>=1.48.4]')
            self.tool_requires('protobuf/[>=3.17.1]')

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        if self.options.offline_proto_generation:
            tc.variable["VIAMCPPSDK_OFFLINE_PROTO_GENERATION"] = True

        if not self.options.use_dynamic_protos:
            tc.variable["VIAMCPPSDK_USE_DYNAMIC_PROTOS"] = False

        tc.generate()

        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        CMake(self).install()

    def package_info(self):
        # Todo: consider
        # https://docs.conan.io/2/examples/tools/cmake/cmake_toolchain/use_package_config_cmake.html
        # and the best practices at the bottom here:
        # https://docs.conan.io/2/reference/conanfile/methods/package_info.html
        # This would work if we are fine with conan only supporting CMake builds

        self.cpp_info.components["viam_rust_utils"].libs = ["viam_rust_utils"]

        for component in ["viamsdk", "viamapi"]:
           self.cpp_info.components[component].libs = [component]
           self.cpp_info.components[component].set_property("cmake_target_name", "viam-cpp-sdk::{}".format(component))
           self.cpp_info.components[component].set_property("pkg_config_name", "viam-cpp-sdk-lib{}".format(component))
           self.cpp_info.components[component].requires = ["grpc::grpc++", "protobuf::libprotobuf"]
           self.cpp_info.components[component].system_libs = ["pthread"]

        self.cpp_info.components["viamapi"].includedirs.append("include/viam/api")

        self.cpp_info.components["viamsdk"].requires.extend([
            "viamapi",
            "boost::headers",
            "boost::log",
            "xtensor::xtensor",

            "viam_rust_utils",
            "abseil::absl_strings",
            "grpc::grpc++_reflection"
        ])

        self.cpp_info.components[component].frameworks = ["Security"]

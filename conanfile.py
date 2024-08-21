from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain

class ViamSdkRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    options = {
        "offline_proto_generation": [True, False],
        "use_dynamic_protos": [True, False]
    }

    default_options = {
        "offline_proto_generation": False,
        "use_dynamic_protos": True
    }

    def requirements(self):
        self.requires('boost/[>=1.74.0]')

        # The SDK supports older grpc and protobuf, but these are the oldest
        # maintained conan packages.
        self.requires('grpc/[>=1.48.4]')
        self.requires('protobuf/[>=3.17.1]')

        self.requires('xtensor/[>=0.24.3]')
        self.requires('abseil/[>=20230125.3]')

    def build_requirements(self):
        self.tool_requires('grpc/[>=1.48.4]')
        self.tool_requires('protobuf/[>=3.17.1]')

    def generate(self):
        tc = CMakeToolchain(self)
        if self.options.offline_proto_generation:
            tc.variable["VIAMCPPSDK_OFFLINE_PROTO_GENERATION"] = True

        if not self.options.use_dynamic_protos:
            tc.variable["VIAMCPPSDK_USE_DYNAMIC_PROTOS"] = False

        tc.generate()

        CMakeDeps(self).generate()

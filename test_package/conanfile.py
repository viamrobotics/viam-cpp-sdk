import os
from io import StringIO

from conan import ConanFile
from conan.errors import ConanException
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run

def run_helper(conanfile, args, env, out):
    conanfile.run(args, env=env, stdout=out)

class viamCppSdkTest(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            output = StringIO()
            sock = "fake-socket-path"

            self.run(os.path.join(self.cpp.build.bindir, f"example_module {sock}"), stdout=output, env="conanrun")

            print(output.getvalue())

            if f"Module listening on {sock}" not in output.getvalue():
                raise ConanException(f"Simple example failed to start module listening")

import os
from io import StringIO

from conan import ConanFile
from conan.errors import ConanException
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run

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
            cmd = os.path.join(self.cpp.build.bindir, "example_module")
            stderr = StringIO()
            self.run(cmd, env='conanrun', stderr=stderr, ignore_errors=True)
            if "main failed with exception:" not in stderr.getvalue():
                raise ConanException("Unexpected error output from test")

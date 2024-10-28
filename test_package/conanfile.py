import subprocess
import os

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
        cmake_layout(self, src_folder="../src/viam/examples/project/cmake")

    def test(self):
        if can_run(self):
            sock = "fake-socket-path"

            cmd = os.path.join(self.cpp.build.bindir, "example_module")

            # the ConanFile run method is a wrapper around Popen, but it only returns the retcode.
            # A properly intialized module waits indefinitely on a signal, so we have to use Popen manually.
            proc = subprocess.Popen([cmd, sock], stdout=subprocess.PIPE, text=True)

            out = None

            try:
                out = proc.communicate(timeout=2)[0]
            except subprocess.TimeoutExpired:
                proc.terminate()
                out = proc.communicate()[0]
                pass

            if f"Module listening on {sock}" not in out:
                raise ConanException(f"Simple example failed to start module listening")

# Toolchain-of-record for published darwin binaries. compiler.version and
# os.version are part of the package_id; pin them so consumers get cache hits
# regardless of which Xcode the runner image defaults to. Keep in sync with
# the runner/Xcode pin in conan.yml and conan-publish.yml.
include(default)

# macOS 14 Sonoma
[settings]
os=Macos
os.version=14.0
arch=armv8
compiler=apple-clang
compiler.version=21
compiler.libcxx=libc++
compiler.cppstd=17
build_type=Release

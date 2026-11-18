import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy


class DbSQLiteAdapterTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

        dep_dst = os.path.join(self.build_folder, "bin", str(self.settings.build_type))
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", src=bindir, dst=dep_dst)
            for libdir in dep.cpp_info.libdirs:
                copy(self, "*.dll", src=libdir, dst=dep_dst)
                copy(self, "*.dylib*", src=libdir, dst=dep_dst)
                copy(self, "*.so*", src=libdir, dst=dep_dst)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        cmake = CMake(self)
        cmake.test()

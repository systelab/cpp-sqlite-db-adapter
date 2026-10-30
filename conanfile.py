import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy, collect_libs

required_conan_version = ">=2.0.0"

class DbSQLiteAdapterConan(ConanFile):
	name = "db-sqlite-adapter"
	description = "Implementation of C++ DB adapter based on SQLite"
	url = "https://github.com/systelab/cpp-sqlite-db-adapter"
	homepage = "https://github.com/systelab/cpp-sqlite-db-adapter"
	author = "CSW <csw@werfen.com>"
	topics = ("conan", "db", "sql", "sqlite", "adapter")
	license = "MIT"
	settings = "os", "compiler", "build_type", "arch"
	exports_sources = "*", "!build*", "!*.yml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore", "!LICENSE"

	def requirements(self):
		self.requires("db-adapter-interface/2.1.1")
		self.requires("sqlite3mc/2.3.0")

	def build_requirements(self):
		self.test_requires("gtest/1.14.0")
		self.test_requires("db-adapter-test-utilities/2.1.1")

	def generate(self):
		deps = CMakeDeps(self)
		deps.generate()
		tc = CMakeToolchain(self)
		tc.generate()

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()
		cmake.test()

	def package(self):
		copy(self, "*.h", src=os.path.join(self.source_folder, "src", "DbSQLiteAdapter"), dst=os.path.join(self.package_folder, "include", "DbSQLiteAdapter"))
		copy(self, "*DbSQLiteAdapter.lib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
		copy(self, "*DbSQLiteAdapter.pdb", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path=False)

	def package_info(self):
		self.cpp_info.set_property("cmake_file_name", "DbSQLiteAdapter")
		self.cpp_info.set_property("cmake_target_name", "DbSQLiteAdapter::DbSQLiteAdapter")
		self.cpp_info.libs = collect_libs(self)

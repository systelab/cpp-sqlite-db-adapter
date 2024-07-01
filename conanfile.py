from conans import ConanFile, CMake, tools

class DbSQLiteAdapterConan(ConanFile):
	name = "DbSQLiteAdapter"
	description = "Implementation of C++ DB adapter based on SQLite"
	url = "https://github.com/systelab/cpp-sqlite-db-adapter"
	homepage = "https://github.com/systelab/cpp-sqlite-db-adapter"
	author = "CSW <csw@werfen.com>"
	topics = ("conan", "db", "sql", "sqlite", "adapter")
	license = "MIT"
	generators = "cmake_find_package"
	settings = "os", "compiler", "build_type", "arch"
	exports_sources = "*", "!build*", "!*.yml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore", "!LICENSE"

	def requirements(self):
		self.requires("DbAdapterInterface/2.0.1@systelab/stable")
		self.requires("sqleet/3.31.0@systelab/stable")

		self.requires("gtest/1.14##4372c5aed2b4018ed9f9da3e218d18b3", private=True)
		self.requires("DbAdapterTestUtilities/2.0.1@systelab/stable", private=True)

	def build(self):
		cmake = CMake(self)
		cmake.configure(source_folder=".")
		cmake.build()

	def package(self):
		self.copy("*.h", dst="include/DbSQLiteAdapter", src="src/DbSQLiteAdapter")
		self.copy("*DbSQLiteAdapter.lib", dst="lib", keep_path=False)
		self.copy("*DbSQLiteAdapter.pdb", dst="bin", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = tools.collect_libs(self)

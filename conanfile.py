from conans import ConanFile, tools

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
    options = {"boost": ["1.66.0", "1.67.0"], "gtest": ["1.7.0", "1.8.1"]}
    default_options = {"boost":"1.67.0", "gtest":"1.8.1"}

    def configure(self):
        self.options["DbAdapterInterface"].boost = self.options.boost
        self.options["DbAdapterTestUtilities"].gtest = self.options.gtest
        self.options["DbAdapterTestUtilities"].boost = self.options.boost

    def requirements(self):
        self.requires("DbAdapterInterface/1.1.5@systelab/stable")

    def build_requirements(self):
        self.build_requires("DbAdapterTestUtilities/1.1.5@systelab/stable")
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        else:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.so*", dst=("bin/%s" % self.settings.build_type), src="lib")

    def package(self):
        self.copy("Connection.h", dst="include/DbSQLiteAdapter", src="src/DbSQLiteAdapter")
        self.copy("ConnectionConfiguration.h", dst="include/DbSQLiteAdapter", src="src/DbSQLiteAdapter")
        self.copy("*DbSQLiteAdapter.lib", dst="lib", keep_path=False)
        self.copy("*DbSQLiteAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*DbSQLiteAdapter.a", dst="lib", keep_path=False)
        self.copy("*sqleet.lib", dst="lib", keep_path=False)
        self.copy("*sqleet.pdb", dst="lib", keep_path=False)
        self.copy("*sqleet.a", dst="lib", keep_path=False)
        self.copy("*sqlite3.exe", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

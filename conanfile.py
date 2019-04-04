from conans import ConanFile, tools

class DbSQLiteAdapterConan(ConanFile):
    name = "DbSQLiteAdapter"
    description = "Implementation of C++ DB adapter based on SQLite"
    url = "https://github.com/systelab/cpp-sqlite-db-adapter"
    homepage = "https://github.com/systelab/cpp-sqlite-db-adapter"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "db", "sql", "sqlite", "adapter")
    license = "MIT"
    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    default_options = "sqlite3:shared=True"

    def requirements(self):
        self.requires("DbAdapter/1.0.0@systelab/stable")
        self.requires("sqlite3/3.27.1@bincrafters/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("Connection.h", dst="include/DbSQLiteAdapter", src="src/DbSQLiteAdapter")
        self.copy("ConnectionConfiguration.h", dst="include/DbSQLiteAdapter", src="src/DbSQLiteAdapter")
        self.copy("*DbSQLiteAdapter.lib", dst="lib", keep_path=False)
        self.copy("*DbSQLiteAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*DbSQLiteAdapter.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
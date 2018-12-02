[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8249c022a30d4843a819d604eec5ec3f)](https://app.codacy.com/app/joaquimvila/cpp-sqlite-db-adapter?utm_source=github.com&utm_medium=referral&utm_content=joaquimvila/cpp-sqlite-db-adapter&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/joaquimvila/cpp-sqlite-db-adapter.svg?branch=master)](https://travis-ci.org/joaquimvila/cpp-sqlite-db-adapter)

# C++ SQL Lite DB Adapter

# Download #

Remember to clone the repository with "--recursive" flag, otherwise the 'cpp-db-adapter' will not be downloaded.

# Build #

The easiest way to deal with the cpp-sqlite-db-adapter is just open the "CMakeLists.txt" with your QtCreator. However, since it is a CMake project, you don't really need an "IDE" to work with. 

You can build the project from source directly from command line following these steps:  

(In cpp-sqlite-db-adapter folder)  

	$mkdir build  
	$cd build  
	$cmake ..

Windows:  

	-	CMake will create a Visual Studio Solution from where you can easily modify and compile your project.  

POSIX:  

	-	CMake will create a Makefile from where you can build your binaries by simply calling "$make" command.  

NOTES: Take into account that the first time the build is made can take some time because it has to compile some dependencies such as boost, googletest, etc. To do so, maybe you need to add execution rights to "include/boost_1_67_0/bootstrap.sh" and "include/boost_1_67_0/tools/build/src/engine/build.sh" files.

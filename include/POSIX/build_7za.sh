#!/bin/bash
cd p7zip/CPP/7zip
rm -rf P7ZIP.Unix
mkdir P7ZIP.Unix
cd P7ZIP.Unix
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../CMAKE/
make -j4

# Copy bin directory to POSIX/p7zip
cp -r bin ../../../
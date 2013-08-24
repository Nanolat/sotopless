EXT_LIB_HOME=`pwd`/..

git clone https://github.com/Nanolat/nldb.git
cd nldb
# Checkout with the commit hash representing the version of nldb compatible with SoTopless.
# Commit : Fixed makefile of gtest to solve the build break issue.
git checkout d1210cabcf82f76f39af555cf5588d42d3aea258
./build.sh

cp -r ./src/include/nldb ${EXT_LIB_HOME}/include
cp ./src/nldb/libnldb.so ${EXT_LIB_HOME}/lib


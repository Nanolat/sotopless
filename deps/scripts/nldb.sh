EXT_LIB_HOME=`pwd`/..

rm -rf nldb
git clone https://github.com/Nanolat/nldb.git
cd nldb
# Checkout with the commit hash representing the version of nldb compatible with SoTopless.
# Commit : tested prepare-build.sh working
git checkout b03a44ba3d320dc3a2e17260db40ef71e88df036

make prepare-build
make clean
make

cp -r ./src/include/nldb ${EXT_LIB_HOME}/include
cp ./src/nldb/libnldb.so ${EXT_LIB_HOME}/lib


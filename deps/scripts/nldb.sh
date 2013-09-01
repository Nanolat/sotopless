EXT_LIB_HOME=`pwd`/..

rm -rf nldb
git clone https://github.com/Nanolat/nldb.git
cd nldb
# Checkout with the commit hash representing the version of nldb compatible with SoTopless.
# Commit : changed readme
#git checkout 5713d93cf13a75a17bd15c045467861269708272

make prepare-build
make clean
make

cp -r ./src/include/nldb ${EXT_LIB_HOME}/include
cp ./src/nldb/libnldb.so ${EXT_LIB_HOME}/lib


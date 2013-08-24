EXT_LIB_HOME=`pwd`/..

git clone git://github.com/v8/v8.git
cd v8
git checkout tags/3.21.0

make dependencies

OS_NAME=`uname`
if [ $OS_NAME = "Darwin" ]
then
    patch -p1 < ${EXT_LIB_HOME}/patches/v8-3.21.0-osx.patch
fi

make x64.release

cp ./out/x64.release/* ${EXT_LIB_HOME}/bin
rm ${EXT_LIB_HOME}/bin/*.a
cp ./out/x64.release/*.a ${EXT_LIB_HOME}/lib
cp ./include/* ${EXT_LIB_HOME}/include


# The path where following directories exist.
# archives - contains tarballs or zip files of open source projects on that Nanolat Database depends.
# lib - contains libraries built.
# include - contains header files of libraries.

echo "Build started at `date`"

EXT_LIB_HOME=`pwd`

rm -rf include lib bin build
mkdir include lib bin build

cd build

#########################################################
# build thrift
#########################################################
pushd .
tar xvfz ${EXT_LIB_HOME}/archives/thrift-0.9.1.tar.gz
cd thrift-0.9.1
./configure
make clean
make
make install DESTDIR=`pwd`/tdest

rm -rf ${EXT_LIB_HOME}/include/thrift
cp -r ./tdest/usr/local/include/* ${EXT_LIB_HOME}/include/
cp -r ./tdest/usr/local/lib/* ${EXT_LIB_HOME}/lib/
cp -r ./tdest/usr/local/bin/* ${EXT_LIB_HOME}/bin/
popd

#########################################################
# build nldb
#########################################################
pushd .
git clone https://github.com/Nanolat/nldb.git
cd nldb
# Checkout with the commit hash representing the version of nldb compatible with SoTopless.
# Commit : Fixed makefile of gtest to solve the build break issue.
git checkout d1210cabcf82f76f39af555cf5588d42d3aea258
./build.sh

cp -r ./src/include/nldb ${EXT_LIB_HOME}/include
cp ./src/nldb/libnldb.so ${EXT_LIB_HOME}/lib

popd

#########################################################
# build Google test framework 
#########################################################
pushd .
unzip ${EXT_LIB_HOME}/archives/gtest-1.6.0.zip
cd gtest-1.6.0
./configure
make clean
make
cp -r include/* ${EXT_LIB_HOME}/include
cp lib/.libs/* ${EXT_LIB_HOME}/lib
popd

#########################################################
# build xUnit++
#########################################################
pushd .
tar xvfz ${EXT_LIB_HOME}/archives/xunit-1.3.2.tar.gz

cd xunit-1.3.2/xUnit++

cp -r ../xUnit++/xUnit++ ${EXT_LIB_HOME}/include

# build debug version
pushd .
cd src
make clean
make debug

cp ./libxUnit++.Debug.a ${EXT_LIB_HOME}/lib

cd ../../xUnit++.console
make clean
make debug

cp ./xUnit.debug ${EXT_LIB_HOME}/bin
popd

# build release version
pushd .
cd src
make clean
make release

cp ./libxUnit++.a ${EXT_LIB_HOME}/lib

cd ../../xUnit++.console
make clean
make release

cp ./xUnit ${EXT_LIB_HOME}/bin
popd


popd

#########################################################
# build Google v8 engine
#########################################################
pushd .
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

popd

#########################################################
# build boost
#########################################################

pushd .
# Boost is too big > 50MB to put into our repository. Download it from internet.
wget http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz
mv ./boost_1_54_0.tar.gz ${EXT_LIB_HOME}/archives/boost_1_54_0.tar.gz
tar xvfz ${EXT_LIB_HOME}/archives/boost_1_54_0.tar.gz
cd boost_1_54_0 
./bootstrap.sh

./b2 install --prefix=${EXT_LIB_HOME}/boost_install

mv ${EXT_LIB_HOME}/boost_install/include/* ${EXT_LIB_HOME}/include
mv ${EXT_LIB_HOME}/boost_install/lib/* ${EXT_LIB_HOME}/lib

rm -rf ${EXT_LIB_HOME}/boost_install
popd

echo "Build ended at `date`"

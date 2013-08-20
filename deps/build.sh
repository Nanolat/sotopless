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
# build protobuf
#########################################################
pushd .
tar xvfz ${EXT_LIB_HOME}/archives/protobuf-2.5.0.tar.gz
cd protobuf-2.5.0
make clean
./configure --prefix=/proto-dest
make
make install DESTDIR=`pwd`
cd proto-dest

rm -rf ${EXT_LIB_HOME}/include/google
cp -r include/* ${EXT_LIB_HOME}/include/
cp -r lib/* ${EXT_LIB_HOME}/lib/
cp -r bin/* ${EXT_LIB_HOME}/bin/
popd


#########################################################
# build myagora
#########################################################
#pushd .
#tar xvfz ${EXT_LIB_HOME}/archives/pushframework.tar.gz
#cd pushframework
#make clean
#make
#rm -rf ${EXT_LIB_HOME}/include/myagora
#cp -r include ${EXT_LIB_HOME}/include/myagora
#cp ../output/libpushframework.so ${EXT_LIB_HOME}/lib/libmyagora.so
#popd

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

cd src
make

cp ./libxUnit++.Debug.a ${EXT_LIB_HOME}/lib

cd ../../xUnit++.console
make

cp ./xUnit ${EXT_LIB_HOME}/bin

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

# The path where following directories exist.
# archives - contains tarballs or zip files of open source projects on that Nanolat Database depends.
# lib - contains libraries built.
# include - contains header files of libraries.
EXT_LIB_HOME=`pwd`

rm -rf include lib bin build
mkdir include lib bin build

cd build

#########################################################
# build gtest
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



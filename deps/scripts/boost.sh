EXT_LIB_HOME=`pwd`/..

# Boost is too big > 50MB to put into our repository. Download it from internet.
wget http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz
mv ./boost_1_54_0.tar.gz ${EXT_LIB_HOME}/archives/boost_1_54_0.tar.gz
tar xvfz ${EXT_LIB_HOME}/archives/boost_1_54_0.tar.gz
cd boost_1_54_0
./bootstrap.sh

./b2 install --prefix=${EXT_LIB_HOME}/build/boost_install

mv ${EXT_LIB_HOME}/build/boost_install/include/* ${EXT_LIB_HOME}/include
mv ${EXT_LIB_HOME}/build/boost_install/lib/* ${EXT_LIB_HOME}/lib

rm -rf ${EXT_LIB_HOME}/build/boost_install


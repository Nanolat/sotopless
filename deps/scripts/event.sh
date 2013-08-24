EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/libevent-2.0.21-stable.tar.gz
cd libevent-2.0.21-stable

./configure
make clean
make
make install DESTDIR=`pwd`/tdest

cp -r ./tdest/usr/local/include/* ${EXT_LIB_HOME}/include/
cp -r ./tdest/usr/local/lib/* ${EXT_LIB_HOME}/lib/
cp -r ./tdest/usr/local/bin/* ${EXT_LIB_HOME}/bin/



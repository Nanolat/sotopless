EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/poco-1.4.6p2-all.tar.gz
cd poco-1.4.6p2-all
./configure --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --prefix=`pwd`/../poco_install

make

make install

rm -rf ${EXT_LIB_HOME}/include/Poco
rm -rf ${EXT_LIB_HOME}/lib/libPoco*

mv ${EXT_LIB_HOME}/build/poco_install/include/* ${EXT_LIB_HOME}/include
mv ${EXT_LIB_HOME}/build/poco_install/lib/* ${EXT_LIB_HOME}/lib
mv ${EXT_LIB_HOME}/build/poco_install/bin/* ${EXT_LIB_HOME}/bin

rm -rf ${EXT_LIB_HOME}/build/poco_install


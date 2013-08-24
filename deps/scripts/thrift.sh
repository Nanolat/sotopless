EXT_LIB_HOME=`pwd`/..

tar xvfz ${EXT_LIB_HOME}/archives/thrift-0.9.1.tar.gz
cd thrift-0.9.1

mkdir -p ${EXT_LIB_HOME}/share
cat << EOF > ${EXT_LIB_HOME}/share/config.site
CPPFLAGS=-I${EXT_LIB_HOME}/include
LDFLAGS=-L${EXT_LIB_HOME}/lib
EOF

THRIFT_PREFIX=${EXT_LIB_HOME}

./configure --prefix=${EXT_LIB_HOME} --without-ruby
make clean
make
make install DESTDIR=`pwd`/tdest

rm -rf ${EXT_LIB_HOME}/include/thrift
cp -r ./tdest/${THRIFT_PREFIX}/include/* ${EXT_LIB_HOME}/include/
cp -r ./tdest/${THRIFT_PREFIX}/bin/* ${EXT_LIB_HOME}/bin/
cp -r ./tdest/${THRIFT_PREFIX}/lib/* ${EXT_LIB_HOME}/lib/
# jar files are installed here. 
cp -r ./tdest/usr/local/lib/* ${EXT_LIB_HOME}/lib/



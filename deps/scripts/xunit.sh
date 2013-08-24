EXT_LIB_HOME=`pwd`/..

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


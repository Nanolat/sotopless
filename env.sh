OS_NAME=`uname`
ADD_LIBS=`pwd`/deps/lib:`pwd`/lib

if [ $OS_NAME = "Darwin" ] 
then
   echo "OSX Detected."
   export DYLD_LIBRARY_PATH=$ADD_LIBS:$DYLD_LIBRARY_PATH
   echo "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH"
else
   export LD_LIBRARY_PATH=$ADD_LIBS:$LD_LIBRARY_PATH
   echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
fi

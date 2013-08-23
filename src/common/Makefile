THRIFT=../../deps/bin/thrift

all: convert 

convert: 
	rm -rf gen-cpp; \
	$(THRIFT) --gen cpp --gen cpp DatabaseService.thrift; \
	rm gen-cpp/*.skeleton.cpp 


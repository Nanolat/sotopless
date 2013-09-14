THRIFT=../../../../deps/bin/thrift

all: convert 

convert: 
	rm -rf gen-cocoa; \
	$(THRIFT) --gen cocoa ../../../common/DatabaseService.thrift; \


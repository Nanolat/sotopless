MAKE=make
MAKE_ARG=
SUBDIRS_TEMPLATE= \
src/server/BUILD_FOLDER \
src/client/BUILD_FOLDER \
src/main/BUILD_FOLDER \
test/unit_client/BUILD_FOLDER \
test/unit_server/BUILD_FOLDER \
test/xunit_client/BUILD_FOLDER 

SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Release)
all: release

# By default, build in release mode. No MAKE_ARG is required for the release mode.
release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Release)
release: compile
OBJ = $(SRC:.cpp=.o)

# Run the debug build
debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Debug)
debug: compile

clean: clean-release

# Clean the release build
clean-release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Release) 
clean-release: MAKE_ARG=clean
clean-release: compile

# Clean the debug build
clean-debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Debug)
clean-debug: MAKE_ARG=clean
clean-debug: compile

compile: 
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done


MAKE=make
MAKE_ARG=
SUBDIRS_TEMPLATE=                \
src/server/BUILD_FOLDER          \
src/nanolat_client/BUILD_FOLDER  \
src/leaderboard/BUILD_FOLDER     \
src/nanolat/BUILD_FOLDER         \
samples/cpp_async/BUILD_FOLDER   \
samples/cpp_sync/BUILD_FOLDER    \
test/ts_client/BUILD_FOLDER      \
test/ts_server/BUILD_FOLDER      \
test/ts_leaderboard/BUILD_FOLDER 

all: release

# By default, build in release mode. No MAKE_ARG is required for the release mode.
release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Release)
release: compile-release

# Run the debug build
debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Debug)
debug: compile-debug

clean: clean-release clean-debug

# Clean the release build
clean-release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Release) 
clean-release: MAKE_ARG=clean
clean-release: compile-release

# Clean the debug build
clean-debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=Debug)
clean-debug: MAKE_ARG=clean
clean-debug: compile-debug

# Why having compile-debug and compile-release running the same script?
# I separated these to clean both debug and release build when 'make clean' runs.
# If I merge these into one, make cleans up either debug or release, not both of them.
compile-debug: 
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done

compile-release:
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done


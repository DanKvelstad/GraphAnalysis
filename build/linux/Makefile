CXX       = g++-6
CXXFLAGS  = -Wall -std=c++14
ROOT      = ../..
LIB      := $(ROOT)/libraries/linux
BINARIES := $(ROOT)/binaries/linux
	
# Build programs, libraries, documentation, etc. (same as make).
all: libraries support.so test_suite | $(BINARIES)

# Install what needs to be installed, copying the files from the package’s tree to system-wide directories.
install:

# Same as make install, then strip debugging symbols. Some users like to trade space for useful bug reports...
install-strip:

# The opposite of make install: erase the installed files. (This needs to be run from the same build tree that was installed.)
uninstall:

# Erase from the build tree the files built by make all.
clean:

# Additionally erase anything ./configure created.
distclean:

# Run the test suite, if any.
check:

# Check the installed programs or libraries, if supported.
installcheck:

# Recreate package-version.tar.gz from all the source files.
dist:

$(BINARIES):
	mkdir -p $(BINARIES)

include libraries.mk
include support.mk
include test_suite.mk

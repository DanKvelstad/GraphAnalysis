CXX       = g++-6
CXXFLAGS  = -Wall -std=c++14
ROOT      = ../..
LIB      := $(ROOT)/libraries/linux
BINARIES := $(ROOT)/binaries/linux
	
all: libraries support.so test_suite | $(BINARIES)

$(BINARIES):
	mkdir -p $(BINARIES)

include libraries.mk
include support.mk
include test_suite.mk
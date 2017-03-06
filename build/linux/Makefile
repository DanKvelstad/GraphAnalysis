CXX            = g++-6
CXXFLAGS       = -Wall -std=c++14 -MMD
ROOT           = ../..
SRC            = $(ROOT)/sources/libraries
INC            = -I$(SRC) -I$(ROOT)/libraries/googletest/googletest/include
INTERMEDIATE   = $(ROOT)/intermediate/Linux
BINARIES       = $(ROOT)/binaries/Linux
OUTPUT_OPTION  = -o $(INTERMEDIATE)/$@
CPP_FILES     := $(wildcard $(SRC)/*.cpp)
OBJ_FILES     := $(addprefix $(INTERMEDIATE)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: libraries

libraries: $(OBJ_FILES) | $(BINARIES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARIES)/libraries $(OBJ_FILES) -lstdc++fs

$(INTERMEDIATE)/%.o : $(SRC)/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_FILES): | $(INTERMEDIATE)

$(INTERMEDIATE):
	mkdir $(INTERMEDIATE)
	
$(BINARIES):
	mkdir $(BINARIES)

#testing: $(ROOT)/sources/testcases/test_moore.o
#	$(CXX) $(CXXFLAGS) $(INC) $(ROOT)/sources/testcases/test_moore.cpp -o $(INTERMEDIATE)/test_moore.o
#	$(CXX) $(INTERMEDIATE)/test_moore.o -o $(BINARIES)/testsuite -lgtest -lpthread
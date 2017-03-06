SRC            = $(ROOT)/sources/testcases
INC            = -I$(SRC) -I$(ROOT)/libraries/googletest/googletest/include -I$(ROOT)/sources
INTERMEDIATE   = $(ROOT)/intermediate/Linux
BINARIES       = $(ROOT)/binaries/Linux
OUTPUT_OPTION  = -o $(INTERMEDIATE)/$@
CPP_FILES     := $(wildcard $(SRC)/*.cpp)
OBJ_FILES     := $(addprefix $(INTERMEDIATE)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: $(OBJ_FILES) | $(BINARIES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARIES)/test_suite $(BINARIES)/gtest-all.o $(BINARIES)/gtest_main.o $(OBJ_FILES) -lstdc++fs -L$(BINARIES) -l:support.so -pthread

$(INTERMEDIATE)/%.o : $(SRC)/%.cpp
	$(COMPILE.cpp) $(INC) $(OUTPUT_OPTION) $<

$(OBJ_FILES): | $(INTERMEDIATE)

$(INTERMEDIATE):
	mkdir $(INTERMEDIATE)
	
$(BINARIES):
	mkdir $(BINARIES)
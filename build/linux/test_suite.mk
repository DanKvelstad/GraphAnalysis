SRC_TEST_SUITE := $(ROOT)/sources/testcases
INC_TEST_SUITE := -I$(SRC_TEST_SUITE) -I$(ROOT)/libraries/include -I$(ROOT)/sources
INT_TEST_SUITE := $(ROOT)/intermediate/test_suite/linux
CPP_TEST_SUITE := $(wildcard $(SRC_TEST_SUITE)/*.cpp)
OBJ_TEST_SUITE := $(addprefix $(INT_TEST_SUITE)/,$(notdir $(CPP_TEST_SUITE:.cpp=.o)))

test_suite: $(OBJ_TEST_SUITE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARIES)/test_suite $(LIB)/gtest-all.o $(LIB)/gtest_main.o $(OBJ_TEST_SUITE) -lstdc++fs -L$(BINARIES) -l:support.so -pthread

$(INT_TEST_SUITE)/%.o : $(SRC_TEST_SUITE)/%.cpp
	$(COMPILE.cpp) $(INC_TEST_SUITE) -o $@ $<

$(OBJ_TEST_SUITE): | $(INT_TEST_SUITE)

$(INT_TEST_SUITE):
	mkdir -p $(INT_TEST_SUITE)
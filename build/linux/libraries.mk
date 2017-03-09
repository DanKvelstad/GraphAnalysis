SRC_LIBRARIES := $(ROOT)/sources/libraries
INC_LIBRARIES := -I$(SRC_LIBRARIES)
INT_LIBRARIES := $(ROOT)/intermediate/libraries/linux
BIN_LIBRARIES := $(INT_LIBRARIES)/binaries
IGN_LIBRARIES := $(SRC_LIBRARIES)/files_windows.cpp $(SRC_LIBRARIES)/library_googletest_windows.cpp
CPP_LIBRARIES := $(filter-out $(IGN_LIBRARIES),$(wildcard $(SRC_LIBRARIES)/*.cpp))
OBJ_LIBRARIES := $(addprefix $(INT_LIBRARIES)/,$(notdir $(CPP_LIBRARIES:.cpp=.o)))

libraries: $(OBJ_LIBRARIES) | $(BIN_LIBRARIES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BIN_LIBRARIES)/libraries $(OBJ_LIBRARIES) -lstdc++fs
	$(BIN_LIBRARIES)/libraries
	
$(INT_LIBRARIES)/%.o : $(SRC_LIBRARIES)/%.cpp
	$(COMPILE.cpp) -o $@ $<

$(OBJ_LIBRARIES): | $(INT_LIBRARIES)

$(INT_LIBRARIES):
	mkdir -p $(INT_LIBRARIES)
	
$(BIN_LIBRARIES):
	mkdir -p $(BIN_LIBRARIES)

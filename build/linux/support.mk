SRC_SUPPORT := $(ROOT)/sources/support
INC_SUPPORT := -I$(SRC_SUPPORT)
INT_SUPPORT := $(ROOT)/intermediate/support/linux
CPP_SUPPORT := $(wildcard $(SRC_SUPPORT)/*.cpp)
OBJ_SUPPORT := $(addprefix $(INT_SUPPORT)/,$(notdir $(CPP_SUPPORT:.cpp=.o)))

support.so: $(OBJ_SUPPORT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -o $(BINARIES)/support.so $(OBJ_SUPPORT)

$(INT_SUPPORT)/%.o : $(SRC_SUPPORT)/%.cpp
	$(COMPILE.cpp) -fPIC $(INC_SUPPORT) -o $@ $<

$(OBJ_SUPPORT): | $(INT_SUPPORT)

$(INT_SUPPORT):
	mkdir -p $(INT_SUPPORT)
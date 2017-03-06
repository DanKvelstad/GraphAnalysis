SRC            = $(ROOT)/sources/libraries
INC            = -I$(SRC)
INTERMEDIATE   = $(ROOT)/intermediate/Linux
BINARIES       = $(ROOT)/binaries/Linux
OUTPUT_OPTION  = -o $(INTERMEDIATE)/$@
CPP_FILES     := $(wildcard $(SRC)/*.cpp)
OBJ_FILES     := $(addprefix $(INTERMEDIATE)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: $(OBJ_FILES) | $(BINARIES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARIES)/libraries $(OBJ_FILES) -lstdc++fs
	$(BINARIES)/libraries
	
$(INTERMEDIATE)/%.o : $(SRC)/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_FILES): | $(INTERMEDIATE)

$(INTERMEDIATE):
	mkdir $(INTERMEDIATE)
	
$(BINARIES):
	mkdir $(BINARIES)
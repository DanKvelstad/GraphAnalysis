CC				= g++
CFLAGS			= -c -Wall
ROOT			= ../..
INTERMEDIATE	= $(ROOT)/intermediate/Linux
BINARIES		= $(ROOT)/binaries/Linux

testing: $(ROOT)/testcases/libraries.cpp makedir
	$(CC) $(CFLAGS) $(ROOT)/testcases/libraries.cpp -o $(INTERMEDIATE)/libraries.o
	$(CC) $(INTERMEDIATE)/libraries.o -o $(BINARIES)/testsuite -lgtest -lpthread
	
makedir:
	mkdir $(INTERMEDIATE)
	mkdir $(BINARIES)
	
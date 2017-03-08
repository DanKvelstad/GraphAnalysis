#include "libraries.h"
#include "files.h"
#include <iostream>

int main()
{

	std::cout << "will now setup the libraries" << std::endl;

	library_googletest();

	// todo either dll or so
	//copy_libraries_to_binaries();

	return 0;

}
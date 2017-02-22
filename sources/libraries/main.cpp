#include "libraries.h"
#include <iostream>

int main()
{

#ifdef _WIN32
	const os current(os::windows);
#else
	const os current(os::linux);
#endif

	std::cout << "will now setup the libraries" << std::endl;

	library_googletest(current);

	return 0;

}
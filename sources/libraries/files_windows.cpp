#include <string>
#include "os.h"

std::string build_directory(void)
{

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = "x86";
		break;
	case platform::x64:
		platform = "x64";
		break;
	default:
		throw std::logic_error("unknown platform");
		break;
	}

	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = "debug";
		break;
	case configuration::release:
		configuration = "release";
		break;
	default:
		throw std::logic_error("unknown configuration");
		break;
	}

	return ("windows " + platform + " " + configuration);

}
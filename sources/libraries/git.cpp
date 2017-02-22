#include "filesystem.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std::experimental;

bool git_prepare(
	std::string				name,
	const filesystem::path&	path, 
	std::string				timestamp,
	std::string				address
)
{

	if(!is_timestamp_expired(timestamp))
	{
		std::cout << name << " was updated and built within the last 24h, leaving it as-is" << std::endl;
		return false;
	}

	std::cout << name << "is older than 24h, updating and rebuilding it" << std::endl;

	if(filesystem::exists(path /".git"))
	{

		std::cout << "found the " << name << " repository" << std::endl;

		std::cout << "now resetting it" << std::endl;
		std::string command_reset(
			"cd " + path.string()	+ "&&" +
			"git reset"				+ "&&" +
			"git checkout ."		+ "&&" +
			"git clean -fdx"
		);
		system(command_reset.c_str());

		std::cout << "now pulling from the master branch" << std::endl;
		std::string command_pull(
			"cd " + path.string() + "&&"
			"git pull"
		);
		system(command_pull.c_str());

	}
	else
	{

		std::cout << "could not find " << name << " repository" << std::endl;

		filesystem::remove_all(path);
		std::string command_clone("git clone " + address + " " + path.string());
		std::cout << command_clone << std::endl;
		system(command_clone.c_str());

	}

	return true;

}

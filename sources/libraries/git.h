#pragma once
#include <filesystem>

using namespace std::experimental;

bool git_prepare(
	std::string				name, 
	const filesystem::path&	path, 
	std::string				timestamp,
	std::string				address
);
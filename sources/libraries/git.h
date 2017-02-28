#pragma once
#include <filesystem>

using namespace std::experimental;

std::chrono::system_clock::time_point git_timestamp(filesystem::path root);

bool git_prepare(
	const filesystem::path&	path, 
	std::string				timestamp,
	std::string				address
);
#pragma once
#include <string>
#include <filesystem>

using namespace std::experimental;

enum class os
{
	windows,
	linux
};

std::string console(const char* cmd);

bool is_timestamp_expired(std::string id);

filesystem::path find_output(void);
filesystem::path find_intermediate(void);
filesystem::path find_libraries(void);
std::string      find_msbuild(void);
std::string      find_devenv(void);

void library_googletest(const os&);
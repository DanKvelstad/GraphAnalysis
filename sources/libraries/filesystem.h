#pragma once
#include <filesystem>

using namespace std::experimental;

std::chrono::system_clock::time_point get_timestamp(filesystem::path file);
bool is_timestamp_expired(std::string id);

filesystem::path find_output(void);
filesystem::path find_intermediate(void);
filesystem::path find_libraries(void);
std::string      find_msbuild(void);
std::string      find_devenv(void);
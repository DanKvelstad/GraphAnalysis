#pragma once
#include <filesystem>

using namespace std::experimental;

std::chrono::system_clock::time_point read_timestamp(filesystem::path file);
void update_timestamp(filesystem::path file);

filesystem::path find_output(void);
filesystem::path find_timestamps(void);
filesystem::path find_intermediate(void);
filesystem::path find_libraries(void);
std::string      find_msbuild(void);
std::string      find_devenv(void);
#pragma once
#include <experimental/filesystem>
#include <string>

using namespace std::experimental;

std::chrono::system_clock::time_point read_timestamp(filesystem::path file);
void update_timestamp(filesystem::path file);

filesystem::path path_to_output(void);
filesystem::path path_to_timestamps(void);
filesystem::path path_to_intermediate(void);
filesystem::path path_to_libraries(void);
std::string      path_to_msbuild(void);
std::string      path_to_devenv(void);
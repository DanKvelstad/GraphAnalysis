#pragma once
#include <filesystem>

using namespace std::experimental;

void msbuild_change_runtime(const filesystem::path& path_msvc);
void msbuild_convert_to_x64(const filesystem::path& path_msvc);
#pragma once
#include <experimental/filesystem>
#include <string>

using namespace std::experimental;

void msbuild_change_runtime(const filesystem::path& path_msvc);
void msbuild_ensure_platform(const filesystem::path& path_msvc);
void msbuild_remove_project(const filesystem::path& path_msvc, std::string projectname);
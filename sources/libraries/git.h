#pragma once
#include <experimental/filesystem>

using namespace std::experimental;

std::chrono::system_clock::time_point git_get_time_of_latest_commit(filesystem::path root);

bool git_ensure_up_to_date(const filesystem::path& path, std::string address);
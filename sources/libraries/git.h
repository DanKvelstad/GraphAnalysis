#pragma once
#include <experimental/filesystem>
#include <string>

using namespace std::experimental;

//std::chrono::system_clock::time_point git_get_time_of_latest_commit(filesystem::path root);

bool git_update(filesystem::path timestamp, filesystem::path local, std::string remote);
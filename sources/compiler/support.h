#pragma once

#include "dllexport.h"
#include "common.h"
#include <vector>
#include <string>
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

DLLEXPORT std::vector<path> search_for_files_with_extension(const path& working_folder, std::vector<std::string> extensions);
DLLEXPORT std::string file_to_string(const path& file);

DLLEXPORT std::vector<definition>  find_variable_definition(std::string content);
DLLEXPORT std::vector<std::string> find_enum_class(const std::string& content, const std::string& type_name);
DLLEXPORT std::vector<transition>  find_transitions(std::string content, const definition& state_machine);
#pragma once

#include "dllexport.h"
#include "common.h"
#include "parsed.h"
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

DLLEXPORT parsed parse(const path& to_parse);
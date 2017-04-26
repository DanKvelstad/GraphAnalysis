#pragma once

#include "dllexport.h"
#include "common.h"
#include "web.h"
#include <vector>
#include <utility>

DLLEXPORT web layouter_brute(
	const std::vector<std::pair<unsigned, unsigned>>& edges
);
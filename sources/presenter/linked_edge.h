#pragma once

#include "dllexport.h"
#include "linked_state.h"
#include <memory>
#include <string>

class linked_edge
{

public:

	DLLEXPORT linked_edge(std::string name, const linked_state& source, const linked_state& target);
	DLLEXPORT ~linked_edge();

	DLLEXPORT void emplace(std::string name, const linked_state& source, const linked_state& target);
	DLLEXPORT linked_edge* next(void);

	void draw(SkCanvas* canvas) const;

private:

	const std::string	name;
	const point			source;
	const point			target;

	std::unique_ptr<linked_edge> linked;

};
#pragma once

#include "dllexport.h"
#include <string>

class SkCanvas;
class linked_state;

class DLLEXPORT linked_edge
{

public:

	linked_edge(void);
	linked_edge(const linked_edge& other);
	~linked_edge(void);

	void emplace(const char* name, unsigned source, unsigned target);
	linked_edge* next(void);

	void draw(SkCanvas* canvas, const linked_state& states) const;

private:

	const char*		name;
	unsigned		source;
	unsigned		target;
	linked_edge*	linked;

};
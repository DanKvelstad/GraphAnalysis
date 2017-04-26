#include "linked_strand.h"
#include "toolbox.h"

linked_strand::linked_strand(unsigned& intersection_counter)
: linked(nullptr)
, knots(nullptr)
, intersection_counter(intersection_counter)
{
}

linked_strand::linked_strand(const linked_strand& old)
: intersection_counter(old.intersection_counter)
{
	if (nullptr != old.knots)
	{
		// this wont work! old.knots are also moving!
		knots = new std::pair<unsigned, unsigned>(*old.knots);
	}
	else
	{
		knots = nullptr;
	}
	if (nullptr != old.linked)
	{
		linked = new linked_strand(*old.linked);
	}
	else
	{
		linked = nullptr;
	}
}

linked_strand & linked_strand::operator=(const linked_strand& old)
{
	intersection_counter	= old.intersection_counter;
	// this wont work! old.knots are also moving!
	knots					= new std::pair<unsigned, unsigned>(*old.knots);
	linked					= new linked_strand(*old.linked);
	return *this;
}

linked_strand::~linked_strand(void)
{
	if (nullptr == knots)
	{
		delete knots;
	}
	if (nullptr != linked)
	{
		delete linked;
	}
}

void linked_strand::emplace(const linked_knot& map, unsigned source, unsigned target)
{
	if (nullptr==knots)
	{
		knots = new std::pair<unsigned, unsigned>(source, target);
	}
	else if (nullptr==linked)
	{
		linked = new linked_strand(intersection_counter);
		linked->emplace(map, source, target);
	}
	else
	{
		if (intersection(
				map.get(knots->first).get(),	map.get(knots->second).get(),
				map.get(source).get(),			map.get(target).get()
			)
		)
		{
			intersection_counter++;
		}
		linked->emplace(map, source, target);
	}
}

unsigned linked_strand::get_source(void) const
{
	return knots->first;
}

unsigned linked_strand::get_target(void) const
{
	return knots->second;
}

const linked_strand* linked_strand::next(void) const
{
	return linked;
}

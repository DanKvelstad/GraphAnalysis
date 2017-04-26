#include "web.h"
#include "toolbox.h"
#include <stdexcept>

web::web()
: intersections(0)
, strands(intersections)
{
}

web::web(const web& old)
: intersections(old.intersections)
, knots(old.knots)
, strands(old.strands)
{
}

web & web::operator=(const web& old)
{
	intersections	= old.intersections;
	knots			= old.knots;
	strands			= old.strands;
	return *this;
}

web::~web()
{
}

void web::emplace_knot(const grid_point & point)
{
	knots.emplace(point);
}

const linked_knot& web::get_knot(unsigned index) const
{
	return knots.get(index);
}

const linked_knot* web::get_knots(void) const
{
	return &knots;
}

void web::emplace_strand(unsigned source, unsigned target)
{
	strands.emplace(knots, source, target);
}

const linked_strand* web::get_strands(void)
{
	return &strands;
}

unsigned web::get_intersection_count(void) const
{
	return intersections;
}

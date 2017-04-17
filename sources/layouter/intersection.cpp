#include "intersection.h"

intersection_status operator||(intersection_status A, intersection_status B)
{

	if (intersection_status::intersects_between_endpoints == A ||
		intersection_status::intersects_between_endpoints == B)
	{
		return intersection_status::intersects_between_endpoints;
	}
	else if (intersection_status::intersects_at_endpoint == A ||
		intersection_status::intersects_at_endpoint == B)
	{
		return intersection_status::intersects_at_endpoint;
	}
	else
	{
		return intersection_status::does_not_intersect;
	}

}

intersection_status operator&&(intersection_status A, intersection_status B)
{

	if (intersection_status::does_not_intersect == A ||
		intersection_status::does_not_intersect == B)
	{
		return intersection_status::does_not_intersect;
	}
	else if (intersection_status::intersects_between_endpoints == A ||
		intersection_status::intersects_between_endpoints == B)
	{
		return intersection_status::intersects_between_endpoints;
	}
	else
	{
		return intersection_status::intersects_at_endpoint;
	}

}
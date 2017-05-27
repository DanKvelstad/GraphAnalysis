#pragma once

typedef int scalar;

struct point
{

	point(void)
	: x(0)
	, y(0)
	{
	}
	
	point(scalar x, scalar y)
	: x(x)
	, y(y)
	{
	}
	
	scalar x;
	scalar y;

};
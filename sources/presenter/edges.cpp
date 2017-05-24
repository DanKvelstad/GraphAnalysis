#include "edges.h"
#include "edge.h"
#include "states.h"
#include <algorithm>

edges::edges(void)
{
}

edges::edges(const edges & other)
: the_edges(other.the_edges)
{
}

edges::~edges(void)
{
}

void edges::emplace(unsigned source, unsigned target, const std::string & name)
{

	auto the_edge(
		std::find_if(
			the_edges.begin(), the_edges.end(),
			[&](const edge& existing) -> bool
			{
				return existing.equals(source, target);
			}
		)	
	);

	if (the_edges.end() == the_edge)
	{
		the_edges.emplace_back(source, target, name);
		// the_edge = the_edges.end()--;
	}
	else
	{
		the_edge->add(name);
	}

}

void edges::update_spacing(states& the_states)
{

	unsigned longest_length = std::numeric_limits<unsigned>::min();

	std::for_each(
		the_edges.begin(), the_edges.end(),
		[&](edge& e)
		{
			auto current_length(e.get_spacing(the_states));
			if (longest_length < current_length)
			{
				longest_length = current_length;
			}
		}
	);

	the_states.set_spacing(longest_length, longest_length);

}

void edges::draw(SkCanvas& canvas, const states& the_states)
{

	std::for_each(
		the_edges.begin(), the_edges.end(),
		[&](edge& this_edge)
		{
		
			auto single_edge(
				the_edges.end() == std::find_if(
					the_edges.begin(), the_edges.end(),
					[&](const edge& other_edge) -> bool
					{
						return this_edge.is_the_opposite_edge(other_edge);
					}
				)
			);
			this_edge.draw(
				canvas,
				the_states,
				single_edge
			);
			
		}
	);
}

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

	auto pre_existing_edge(
		std::find_if(
			the_edges.begin(), the_edges.end(),
			[&](const edge& existing) -> bool
			{
				return existing.equals(source, target);
			}
		)	
	);
	if (the_edges.end() == pre_existing_edge)
	{
		the_edges.emplace_back(source, target, name);
	}
	else
	{
		pre_existing_edge->add(name);
	}

}

std::pair<unsigned, unsigned> edges::get_text_dimensions(void)
{

	auto longest_length(std::make_pair(0u, 0u));

	std::for_each(
		the_edges.begin(), the_edges.end(),
		[&](edge& e)
		{
			auto current_length(e.get_text_dimensions());
			current_length.first += 50;
			if (longest_length.first < current_length.first)
			{
				longest_length = current_length;
			}
		}
	);

	return longest_length;

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

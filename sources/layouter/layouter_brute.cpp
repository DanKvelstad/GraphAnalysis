#include "layouters.h"
#include <algorithm>

web layouter_brute(
	const std::vector<std::pair<unsigned, unsigned>>& edges
)
{

	unsigned count(0);
	std::for_each(
		edges.begin(), edges.end(),
		[&count](const std::pair<unsigned, unsigned>& edge)
		{
			count = std::max(std::max(count, edge.first), edge.second);
		}
	);
	count++;

	grid_point grid_dimensions{
		static_cast<grid_scalar>(std::ceil(count / 2.0)),
		static_cast<grid_scalar>(std::ceil(count / 2.0))
	};

	std::vector<grid_scalar> grid(grid_dimensions.x*grid_dimensions.y);
	for (unsigned j = 0; j<grid.size(); j++)
	{
		grid.at(j) = j;
	}

	web best_web;
	bool force_best(true);
	
	do 
	{

		web web;
		
		for (unsigned i(0); i < count; i++)
		{
			web.emplace_knot(
				grid_point{
					grid.at(i) / grid_dimensions.x,
					grid.at(i) % grid_dimensions.x
				}
			);
		}

		for (auto& edge : edges)
		{
			web.emplace_strand(edge.first, edge.second);
		}
		
		if (web.get_intersection_count() <= best_web.get_intersection_count())
		{
			best_web = std::move(web);
		}
		else if (force_best)
		{
			best_web = std::move(web);
			force_best = false;
		}

	} while (std::next_permutation(grid.begin(), grid.end()));

	return best_web;

}
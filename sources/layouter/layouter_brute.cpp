#include "layouters.h"
#include <algorithm>
#include "area_rectangle.h"
#include <atomic>
#include "line.h"
#include "shape.h"

layout layouter_brute(
	unsigned									state_count,
	std::vector<std::pair<unsigned, unsigned>>	edges,
	grid_point									grid_dimensions
)
{

	std::vector<unsigned> grid(grid_dimensions.x*grid_dimensions.y);
	for (unsigned j = 0; j<grid.size(); j++)
	{
		grid.at(j) = j;
	}

	layout best_layout;
	best_layout.intersections	= static_cast<unsigned>(-1);
	best_layout.workspace_size	= static_cast<unsigned>(-1);

	do 
	{

		std::vector<area_rectangle> areas;
		areas.reserve(state_count);
		unsigned largest_x(0);
		unsigned largest_y(0);
		for (unsigned i(0); i<state_count; i++)
		{
			areas.emplace_back(
				grid_point{
					grid.at(i)%grid_dimensions.x,
					grid.at(i)/grid_dimensions.x
				}
			);
			largest_x = std::max(largest_x, static_cast<unsigned>(areas.back().get_center().x));
			largest_y = std::max(largest_y, static_cast<unsigned>(areas.back().get_center().y));
		}

		std::vector<line> lines;
		lines.reserve(edges.size());
		for (auto& edge : edges)
		{
			lines.emplace_back(
				areas.at(edge.first).get_center(),
				areas.at(edge.second).get_center()
			);
			lines.back().cut_source(areas.at(edge.first));
			lines.back().cut_target(areas.at(edge.second));
		}

		std::atomic<unsigned> intersection_count(0);
		std::for_each(
			lines.begin(), lines.end(),
			[&intersection_count, &lines](const line& a)
			{
				intersection_count += static_cast<unsigned>(
					std::count_if(
						lines.begin(), lines.end(),
						[&a](const line& b)
						{
							if(a==b)
							{
								return false;
							}
							else
							{
								return	intersection_status::does_not_intersect !=
										a.intersect(b).status;
							}
						}
					)
				);
			}
		);
		auto workspace_size(largest_x*largest_y);
		if (intersection_count < best_layout.intersections)
		{
			best_layout.states				= std::move(areas);
			best_layout.edges				= std::move(lines);
			best_layout.intersections		= intersection_count;
			best_layout.workspace_size		= workspace_size;
		}
		else if (intersection_count == best_layout.intersections)
		{
			if (workspace_size<best_layout.workspace_size)
			{
				best_layout.states				= std::move(areas);
				best_layout.edges				= std::move(lines);
				best_layout.intersections		= intersection_count;
				best_layout.workspace_size		= workspace_size;
			}
		}

	} while (std::next_permutation(grid.begin(), grid.end()));

	return best_layout;

}
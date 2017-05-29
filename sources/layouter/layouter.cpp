#include "layouter.h"
#include "common.h"
#include <algorithm>
#include "toolbox.h"
#include <numeric>

layouter::layouter(void)
: largest_index(0)
{
}

layouter::~layouter(void)
{
}

DLLEXPORT void layouter::emplace(unsigned from_index, unsigned to_index)
{
	edges.emplace_back(edge{ from_index, to_index });
	largest_index = std::max(largest_index, std::max(from_index, to_index));
}

DLLEXPORT std::pair<unsigned, unsigned> layouter::layout()
{

	grid_point grid_dimensions{
		static_cast<unsigned>(std::ceil((largest_index+1) / 2.0)),
		static_cast<unsigned>(std::ceil((largest_index+1) / 2.0))
	};

	std::vector<unsigned> grid(grid_dimensions.x*grid_dimensions.y);
	for (unsigned j = 0; j<grid.size(); j++)
	{
		grid.at(j) = j;
	}

	intersection_count = std::numeric_limits<unsigned>::max();
	
	do 
	{

		std::vector<pixel_point> candidate;
		candidate.reserve(largest_index+1);
		for (unsigned i(0); i < largest_index+1; i++)
		{
			candidate.emplace_back(
				pixel_point{
					static_cast<pixel_scalar>(grid.at(i) % grid_dimensions.x),
					static_cast<pixel_scalar>(grid.at(i) / grid_dimensions.x)
				}
			);
		}

		unsigned candidate_intersection_count(0);
		for (auto it(edges.begin()); edges.end()!=it; ++it)
		{
			candidate_intersection_count += std::accumulate(
				it+1, edges.end(), candidate_intersection_count,
				[&it, &candidate](const unsigned& sum, const edge& right) -> unsigned
				{
					if(	intersection(
							candidate.at(it->from_index),
							candidate.at(it->to_index),
							candidate.at(right.from_index),
							candidate.at(right.to_index)
						)
					)
					{
						return sum+1;
					}
					else
					{
						return sum;
					}
				}
			);
		}

		if (candidate_intersection_count == intersection_count)
		{
			bool unique = candidates.end() == std::find_if(
				candidates.begin(), candidates.end(),
				[&candidate](const std::vector<pixel_point>& existing)
				{
				return existing == candidate;
				}
			);
			if (unique)
			{
				candidates.emplace_back(std::move(candidate));
			}
		}
		else if (candidate_intersection_count < intersection_count)
		{
			candidates.clear();
			candidates.emplace_back(std::move(candidate));
			intersection_count = candidate_intersection_count;
		}

	} while (std::next_permutation(grid.begin(), grid.end()));
	
	return std::make_pair(
		static_cast<unsigned>(candidates.size()),
		static_cast<unsigned>(candidates.at(0).size())
	);

}

unsigned layouter::get_intersection_count(void)
{
	return intersection_count;
}

layouter::coordinate layouter::at(unsigned i, unsigned j)
{
	return layouter::coordinate{
		static_cast<unsigned>(candidates.at(i).at(j).x),
		static_cast<unsigned>(candidates.at(i).at(j).y)
	};
}

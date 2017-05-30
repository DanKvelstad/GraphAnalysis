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

	bool accepted = edges.end()==std::find_if(
		edges.begin(), edges.end(),
		[&](const edge& existing) -> bool
		{
			return	( existing.from_index == from_index &&
					  existing.to_index   == to_index   )  ||
					( existing.from_index == to_index	&&
					  existing.to_index   == from_index );
		}
	);
	if (accepted)
	{
		edges.emplace_back(edge{ from_index, to_index });
		largest_index = std::max(largest_index, std::max(from_index, to_index));
	}
	
}

DLLEXPORT std::pair<unsigned, unsigned> layouter::layout()
{

	unsigned state_count(largest_index+1);

	unsigned grid_dimensions(
		static_cast<unsigned>(
			std::ceilf(
				std::sqrtf(
					static_cast<float>(state_count)
				)	
			)
		)
	);

	std::vector<unsigned> grid(grid_dimensions*grid_dimensions);
	for (unsigned j = 0; j<grid.size(); j++)
	{
		grid.at(j) = j;
	}

	intersection_count = std::numeric_limits<unsigned>::max();
	
	do 
	{

		std::vector<pixel_point> candidate;
		candidate.reserve(state_count);
		for (unsigned i(0); i < state_count; i++)
		{
			candidate.emplace_back(
				pixel_point{
					static_cast<pixel_scalar>(grid.at(i) % grid_dimensions),
					static_cast<pixel_scalar>(grid.at(i) / grid_dimensions)
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

	std::sort(
		candidates.begin(), candidates.end(),
		[this](const std::vector<pixel_point>&a, const std::vector<pixel_point>&b) -> bool
		{

			auto a_length = std::accumulate(
				edges.begin(), edges.end(), 0.0f,
				[&a](const float& sum, const edge& e) -> float
				{
					auto p1 = a.at(e.to_index);
					auto p2 = a.at(e.from_index);
					auto dx = p1.x - p2.x;
					auto dy = p1.y - p2.y;
					return sum + sqrt(dx*dx+dy*dy);
				}
			);

			float b_length = std::accumulate(
				edges.begin(), edges.end(), 0.0f,
				[&b](const float& sum, const edge& e) -> float
				{
					auto p1 = b.at(e.to_index);
					auto p2 = b.at(e.from_index);
					auto dx = p1.x - p2.x;
					auto dy = p1.y - p2.y;
					return sum + sqrt(dx*dx+dy*dy);
				}
			);

			if (fabs(a_length - b_length) < 0.0001)
			{
				auto it = std::mismatch(
					a.begin(), a.end(),
					b.begin(), b.end()
				);
				return	sqrt( it.first->x*it.first->x  +  it.first->y*it.first->y ) < 
						sqrt(it.second->x*it.second->x + it.second->y*it.second->y);
			}
			else
			{
				return a_length < b_length;
			}

		}
	);
	
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

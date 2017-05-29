#include "gtest/gtest.h"
#include "layouter/layouter.h"
#include <chrono>
#include "presenter/presenter.h"

TEST(test_presenter, three_by_three)
{

	std::vector<std::pair<unsigned, unsigned>> input;
	
	// square
	input.emplace_back(0, 1);
	//input.emplace_back(1, 0);
	input.emplace_back(1, 2);
	//input.emplace_back(2, 1);
	input.emplace_back(2, 3);
	//input.emplace_back(3, 2);
	input.emplace_back(3, 0);
	//input.emplace_back(0, 3);

	// star
	input.emplace_back(4, 0);
	//input.emplace_back(0, 4);
	input.emplace_back(4, 1);
	//input.emplace_back(1, 4);
	input.emplace_back(4, 2);
	//input.emplace_back(2, 4);
	input.emplace_back(4, 3);
	//input.emplace_back(3, 4);
	
	//do{4

	layouter uut;
	for (const auto& i : input)
	{
		uut.emplace(i.first, i.second);
	}
	
	auto start(std::chrono::high_resolution_clock::now());
	auto result(uut.layout());
	auto benchmark(std::chrono::high_resolution_clock::now()-start);

	//} while (std::next_permutation(input.begin(), input.end()));

	ASSERT_EQ(uut.get_intersection_count(), 0);
	ASSERT_EQ(result.first, 64);
	ASSERT_EQ(result.second, 5);

	auto path(temp_directory_path() / "google_test" / test_info_->name());
	create_directories(path);
	for (unsigned i = 0; i < result.first; i++)
	{

		presenter the_presenter;
		
		for (unsigned j = 0; j < result.second; j++)
		{
			auto coordinate(uut.at(i, j));
			the_presenter.emplace_state("s"+std::to_string(j), coordinate.x, coordinate.y);
		}
		
		for (const auto& in : input)
		{
			the_presenter.emplace_edge(in.first, in.second);
		}

		the_presenter.draw(path/(std::to_string(i)+".png"));

	}
	 
	ASSERT_LT(benchmark, std::chrono::seconds(1)) << "Too slow at " << benchmark.count() << "ns";

}
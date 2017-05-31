#include "gtest/gtest.h"
#include "layouter/layouter.h"
#include <chrono>
#include "presenter/presenter.h"

TEST(test_layouter, duplex)
{

	std::vector<std::pair<unsigned, unsigned>> input;

	input.emplace_back(0, 1);
	input.emplace_back(1, 0);
	
	layouter uut;
	for (const auto& i : input)
	{
		uut.emplace(i.first, i.second);
	}

	auto result(uut.layout());

	if (false)
	{
		auto path(temp_directory_path() / "google_test" / test_info_->name());
		create_directories(path);
		for (unsigned i = 0; i < result.first; i++)
		{

			presenter the_presenter;

			std::vector<layouter::coordinate> current;
			for (unsigned j = 0; j < result.second; j++)
			{
				current.emplace_back(uut.at(i, j));
				the_presenter.emplace_state("s" + std::to_string(j), current.back().x, current.back().y);
			}

			for (const auto& in : input)
			{
				the_presenter.emplace_edge(in.first, in.second);
			}

			the_presenter.create_png(path / (std::to_string(i) + ".png"));

		}
	}

	ASSERT_EQ(uut.get_intersection_count(), 0);
	ASSERT_EQ(result.first, 12);
	ASSERT_EQ(result.second, 2);

	std::vector<layouter::coordinate> expected{
		layouter::coordinate{ 0, 0 },
		layouter::coordinate{ 1, 0 },
	};
	std::vector<layouter::coordinate> actually_best;
	for (unsigned j = 0; j < result.second; j++)
	{
		actually_best.emplace_back(uut.at(0, j));
	}
	ASSERT_TRUE(expected == actually_best);

}

TEST(test_layouter, three_by_three)
{

	std::vector<std::pair<unsigned, unsigned>> input;
	
	// square
	input.emplace_back(0, 1);
	input.emplace_back(1, 2);
	input.emplace_back(2, 3);
	input.emplace_back(3, 0);

	// star
	input.emplace_back(4, 0);
	input.emplace_back(4, 1);
	input.emplace_back(4, 2);
	input.emplace_back(4, 3);

	layouter uut;
	for (const auto& i : input)
	{
		uut.emplace(i.first, i.second);
	}
	
	auto start(std::chrono::high_resolution_clock::now());
	auto result(uut.layout());
	auto benchmark(std::chrono::high_resolution_clock::now()-start);

	if(false)
	{
		auto path(temp_directory_path() / "google_test" / test_info_->name());
		create_directories(path);
		for (unsigned i = 0; i < result.first; i++)
		{

			presenter the_presenter;
			
			std::vector<layouter::coordinate> current;
			for (unsigned j = 0; j < result.second; j++)
			{
				current.emplace_back(uut.at(i, j));
				the_presenter.emplace_state("s"+std::to_string(j), current.back().x, current.back().y);
			}
			
			for (const auto& in : input)
			{
				the_presenter.emplace_edge(in.first, in.second);
			}

			the_presenter.create_png(path/(std::to_string(i)+".png"));

		}
	}

	ASSERT_EQ(uut.get_intersection_count(), 0);
	ASSERT_EQ(result.first, 304);
	ASSERT_EQ(result.second, 5);
	ASSERT_LT(benchmark, std::chrono::milliseconds(600)) << "Too slow at " << benchmark.count() << "ns";

	std::vector<layouter::coordinate> expected{
		layouter::coordinate{ 0, 1 },
		layouter::coordinate{ 1, 0 },
		layouter::coordinate{ 2, 1 },
		layouter::coordinate{ 1, 2 },
		layouter::coordinate{ 1, 1 }
	};
	std::vector<layouter::coordinate> actually_best;
	for (unsigned j = 0; j < result.second; j++)
	{
		actually_best.emplace_back(uut.at(0, j));
	}
	ASSERT_TRUE(expected==actually_best);

}


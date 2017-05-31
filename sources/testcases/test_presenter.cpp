#include "gtest/gtest.h"
#include "presenter/presenter.h"

path output(std::string name)
{
	return temp_directory_path() / "google_test" / (name + ".png");
}

TEST(test_presenter, full_duplex)
{

	presenter the_presenter;

	the_presenter.emplace_state("s0", 1, 1);
	the_presenter.emplace_state("s1", 0, 0);
	the_presenter.emplace_state("s2", 2, 0);
	the_presenter.emplace_state("s3", 2, 2);
	the_presenter.emplace_state("s4", 0, 2);

	// star
	the_presenter.emplace_edge(0, 1, "forward");
	the_presenter.emplace_edge(1, 0, "backward");
	the_presenter.emplace_edge(0, 2, "forward");
	the_presenter.emplace_edge(2, 0, "backward");
	the_presenter.emplace_edge(0, 3, "forward");
	the_presenter.emplace_edge(3, 0, "backward");
	the_presenter.emplace_edge(0, 4, "forward");
	the_presenter.emplace_edge(4, 0, "backward");

	// square
	the_presenter.emplace_edge(1, 2, "forward");
	the_presenter.emplace_edge(2, 1, "backward");
	the_presenter.emplace_edge(2, 3, "forward");
	the_presenter.emplace_edge(3, 2, "backward");
	the_presenter.emplace_edge(3, 4, "forward");
	the_presenter.emplace_edge(4, 3, "backward");
	the_presenter.emplace_edge(4, 1, "forward");
	the_presenter.emplace_edge(1, 4, "backward");

	the_presenter.create_png(output(test_info_->name()));

}

TEST(test_presenter, single_duplex_forward)
{

	presenter the_presenter;

	the_presenter.emplace_state("s0", 1, 1);
	the_presenter.emplace_state("s1", 0, 0);
	the_presenter.emplace_state("s2", 2, 0);
	the_presenter.emplace_state("s3", 2, 2);
	the_presenter.emplace_state("s4", 0, 2);

	// star
	the_presenter.emplace_edge(0, 1, "forward");
	the_presenter.emplace_edge(0, 2, "forward");
	the_presenter.emplace_edge(0, 3, "forward");
	the_presenter.emplace_edge(0, 4, "forward");

	// square
	the_presenter.emplace_edge(1, 2, "forward");
	the_presenter.emplace_edge(2, 3, "forward");
	the_presenter.emplace_edge(3, 4, "forward");
	the_presenter.emplace_edge(4, 1, "forward");

	the_presenter.create_png(output(test_info_->name()));

}

TEST(test_presenter, single_duplex_backward)
{

	presenter the_presenter;

	the_presenter.emplace_state("s0", 1, 1);
	the_presenter.emplace_state("s1", 0, 0);
	the_presenter.emplace_state("s2", 2, 0);
	the_presenter.emplace_state("s3", 2, 2);
	the_presenter.emplace_state("s4", 0, 2);

	// star
	the_presenter.emplace_edge(1, 0, "backward");
	the_presenter.emplace_edge(2, 0, "backward");
	the_presenter.emplace_edge(3, 0, "backward");
	the_presenter.emplace_edge(4, 0, "backward");

	// square
	the_presenter.emplace_edge(2, 1, "backward");
	the_presenter.emplace_edge(3, 2, "backward");
	the_presenter.emplace_edge(4, 3, "backward");
	the_presenter.emplace_edge(1, 4, "backward");

	the_presenter.create_png(output(test_info_->name()));

}

TEST(test_presenter, multiple_events_edge)
{

	presenter the_presenter;

	the_presenter.emplace_state("s0", 0, 0);
	the_presenter.emplace_state("s1", 1, 0);

	the_presenter.emplace_edge(0, 1, "e0");
	the_presenter.emplace_edge(0, 1, "e1");
	the_presenter.emplace_edge(0, 1, "e2");

	the_presenter.create_png(output(test_info_->name()));

}

TEST(test_presenter, long_names)
{

	presenter the_presenter;

	the_presenter.emplace_state("long_state_name", 0, 0);
	the_presenter.emplace_state("s1", 1, 0);

	the_presenter.emplace_edge(0, 1, "long_event_name");

	the_presenter.create_png(output(test_info_->name()));

}
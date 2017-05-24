#include "gtest/gtest.h"
#include "presenter/presenter.h"
#include "presenter/states.h"
#include "presenter/edges.h"
#include "presenter/states.h"

path output(std::string name)
{
	return temp_directory_path() / "google_test" / (name + ".png");
}

TEST(test_presenter, full_duplex)
{

	states states;
	states.emplace("s0", 1, 1);
	states.emplace("s1", 0, 0);
	states.emplace("s2", 2, 0);
	states.emplace("s3", 2, 2);
	states.emplace("s4", 0, 2);

	edges edges;

	// star
	edges.emplace(0, 1, "forward");
	edges.emplace(1, 0, "backward");
	edges.emplace(0, 2, "forward");
	edges.emplace(2, 0, "backward");
	edges.emplace(0, 3, "forward");
	edges.emplace(3, 0, "backward");
	edges.emplace(0, 4, "forward");
	edges.emplace(4, 0, "backward");

	// square
	edges.emplace(1, 2, "forward");
	edges.emplace(2, 1, "backward");
	edges.emplace(2, 3, "forward");
	edges.emplace(3, 2, "backward");
	edges.emplace(3, 4, "forward");
	edges.emplace(4, 3, "backward");
	edges.emplace(4, 1, "forward");
	edges.emplace(1, 4, "backward");

	draw(
		output(test_info_->name()),
		states,
		edges
	);

}

TEST(test_presenter, single_duplex_forward)
{

	states states;
	states.emplace("s0", 1, 1);
	states.emplace("s1", 0, 0);
	states.emplace("s2", 2, 0);
	states.emplace("s3", 2, 2);
	states.emplace("s4", 0, 2);

	edges edges;

	// star
	edges.emplace(0, 1, "forward");
	edges.emplace(0, 2, "forward");
	edges.emplace(0, 3, "forward");
	edges.emplace(0, 4, "forward");

	// square
	edges.emplace(1, 2, "forward");
	edges.emplace(2, 3, "forward");
	edges.emplace(3, 4, "forward");
	edges.emplace(4, 1, "forward");

	draw(
		output(test_info_->name()),
		states,
		edges
	);

}

TEST(test_presenter, single_duplex_backward)
{

	states states;
	states.emplace("s0", 1, 1);
	states.emplace("s1", 0, 0);
	states.emplace("s2", 2, 0);
	states.emplace("s3", 2, 2);
	states.emplace("s4", 0, 2);

	edges edges;

	// star
	edges.emplace(1, 0, "backward");
	edges.emplace(2, 0, "backward");
	edges.emplace(3, 0, "backward");
	edges.emplace(4, 0, "backward");

	// square
	edges.emplace(2, 1, "backward");
	edges.emplace(3, 2, "backward");
	edges.emplace(4, 3, "backward");
	edges.emplace(1, 4, "backward");

	draw(
		output(test_info_->name()),
		states,
		edges
	);

}

TEST(test_presenter, multiple_events_edge)
{

	states states;
	states.emplace("s0", 0, 0);
	states.emplace("s1", 1, 0);

	edges edges;
	edges.emplace(0, 1, "e0");
	edges.emplace(0, 1, "e1");
	edges.emplace(0, 1, "e2");

	draw(
		output(test_info_->name()),
		states,
		edges
	);

}

TEST(test_presenter, long_names)
{

	states states;
	states.emplace("long_state_name", 0, 0);
	states.emplace("s1", 1, 0);

	edges edges;
	edges.emplace(0, 1, "long_event_name");

	draw(
		output(test_info_->name()),
		states,
		edges
	);

}

TEST(test_presenter, circle)
{

	states states;
	states.emplace("s0", 1, 1);
	states.emplace("s1", 1, 0);
	states.emplace("s2", 0, 1);

	edges edges;
	edges.emplace(0, 0, "e0");
	edges.emplace(1, 0, "e1");
	edges.emplace(2, 0, "e2");
	edges.emplace(1, 2, "e3");

	//draw(
	//	output(test_info_->name()),
	//	states,
	//	edges
	//);

}
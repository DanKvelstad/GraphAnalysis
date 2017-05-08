//#include "gtest/gtest.h"
//#include "compiler/support.h"
//
//TEST(test_compiler, simple_variable_definition)
//{
//	definition input{"state_machine", "my_states", "my_edges"};
//	auto output = find_variable_definition(
//		"moore<" + input.states + ", " + input.edges + "> " + input.variable
//	);
//	ASSERT_EQ(input.variable,	output.at(0).variable);
//	ASSERT_EQ(input.states,		output.at(0).states);
//	ASSERT_EQ(input.edges,		output.at(0).edges);
//}
//
//// ToDo: moore<my_states, my_edges>*					state_machine
//// ToDo: std::shared_ptr<moore<my_states, my_edges>>	state_machine
//
//TEST(test_compiler, simple_enum_class)
//{
//	std::vector<std::string> input{ "s0", "s1", "s2" };
//	auto output = find_enum_class(
//		"enum class states{"+input.at(0)+","+input.at(1)+" ,\t"+input.at(2)+"}",
//		"states"
//	);
//	ASSERT_EQ(input.at(0), output.at(0));
//	ASSERT_EQ(input.at(1), output.at(1));
//	ASSERT_EQ(input.at(2), output.at(2));
//}
//
//TEST(test_compiler, enum_class_formated)
//{
//	std::vector<std::string> input{ "s0", "s1", "s2" };
//	auto output = find_enum_class(
//		"enum class states\n{" + input.at(0) + ",\n" + input.at(1) + ",\n" + input.at(2) + "\n}",
//		"states"
//	);
//	ASSERT_EQ(input.at(0), output.at(0));
//	ASSERT_EQ(input.at(1), output.at(1));
//	ASSERT_EQ(input.at(2), output.at(2));
//}
//
//TEST(test_compiler, simple_transition_without_event)
//{
//	std::vector<std::string> input{ "s0", "s1" };
//	auto output = find_transitions(
//		"state_machine.add_transition(my_states::" + input.at(0) +", my_states::" + input.at(1) +");",
//		definition{"state_machine", "my_states", "my_edges"}
//	);
//	ASSERT_EQ(input.at(0), output.at(0).from_state);
//	ASSERT_EQ(input.at(1), output.at(0).to_state);
//}
//
//TEST(test_compiler, simple_transition_with_event)
//{
//	std::vector<std::string> input{ "s0", "e0", "s1" };
//	auto output = find_transitions(
//		"state_machine.add_transition(my_states::" + input.at(0) + ", my_events::" + input.at(1) + ", my_states::" + input.at(2) + ");",
//		definition{ "state_machine", "my_states", "my_edges" }
//	);
//	ASSERT_EQ(input.at(0), output.at(0).from_state);
//	ASSERT_EQ(input.at(1), output.at(0).event_name);
//	ASSERT_EQ(input.at(2), output.at(0).to_state);
//}
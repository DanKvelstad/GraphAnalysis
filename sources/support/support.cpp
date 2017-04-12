#include "support.h"
#include <regex>

std::vector<definition> find_variable_definition(std::string content)
{

	std::vector<definition> result;

	std::smatch smatch;
	std::regex  regex("moore<\\s*(\\w+)\\s*,\\s*(\\w+)\\s*>\\s*(\\w+)");
	while(std::regex_search(content, smatch, regex))
	{
		auto& variable(smatch[3]);
		auto& states(smatch[1]);
		auto& edges(smatch[2]);
		result.emplace_back(definition{smatch[3], smatch[1], smatch[2]});
		content = smatch.suffix();
	}

	return result;

}

std::vector<std::string> find_enum_class(const std::string& content, const std::string& type_name)
{

	std::vector<std::string> result;

	std::smatch smatch;
	std::regex  regex("enum\\s+class\\s+" + type_name + "\\s*\\{([\\w\\W]*?)\\}");
	if (std::regex_search(content, smatch, regex))
	{
		std::string content2(smatch[1].str());
		std::smatch smatch2;
		std::regex  regex2("(\\w+)");
		while (std::regex_search(content2, smatch2, regex2))
		{
			result.emplace_back(smatch2[1].str());
			content2 = smatch2.suffix();
		}
	}

	return result;

}

std::vector<transition> find_transitions(std::string content, const definition& state_machine)
{

	std::vector<transition> result;

	std::smatch smatch;
	std::regex  regex(state_machine.variable+"[.->]add_transition\\((.*?)\\)");
	while (std::regex_search(content, smatch, regex))
	{
		std::string content2(smatch[1].str());
		std::smatch smatch2;
		std::regex  regex2("\\w+::(\\w+)");
		std::vector<std::string> tmp;
		while (std::regex_search(content2, smatch2, regex2))
		{
			tmp.push_back(smatch2[1].str());
			content2 = smatch2.suffix();
		}
		if (2 == tmp.size())
		{
			result.emplace_back(transition{ tmp.at(0), "", tmp.at(1) });
		}
		else if (3 == tmp.size())
		{
			result.emplace_back(transition{ tmp.at(0), tmp.at(1), tmp.at(2) });
		}
		else
		{
			throw std::exception();
		}
		content = smatch.suffix();
	}

	return result;

}
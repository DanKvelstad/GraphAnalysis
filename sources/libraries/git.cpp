#include "filesystem.h"
#include "os.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>
#include <ctime>
#include <string>

using namespace std::experimental;

std::chrono::system_clock::time_point git_get_time_of_latest_commit(filesystem::path root)
{

	auto string(
		console(
			"cd "+root.string()+"&&"+
			"git log -1 --date=raw --pretty=format:%cd"
		)
	);
	std::regex	regex("([0-9]+) ([-+][0-9]+)");
	std::smatch	smatch;
	std::regex_search(string, smatch, regex);
	return std::chrono::system_clock::time_point(
		std::chrono::seconds(std::stoi(smatch[1]))
	);

}

bool git_ensure_up_to_date(const filesystem::path& path, std::string address)
{

	bool updated;

	if(filesystem::exists(path /".git"))
	{

		std::cout << "Now reseting the local workspace... ";

		std::string command_reset(
			"cd " + path.string()	+ "&&" +
			"git reset"				+ "&&" +
			"git checkout ."		+ "&&" +
			"git clean -fdx"
		);
		std::string response_reset(console(command_reset.c_str()));
		std::cout << "done" << std::endl;

		std::cout << "Now pulling from the the repo... ";
		std::string command_pull(
			"cd " + path.string() + "&&"
			"git pull"
		);
		std::string response_update(console(command_pull.c_str()));
		std::cout << "done" << std::endl;
		updated = ("Already up-to-date." == response_update);
		
	}
	else
	{

		std::cout << "Could not find the local workspace, will clone it now" << std::endl;

		filesystem::remove_all(path);
		std::string command_clone("git clone " + address + " " + path.string());
		std::cout << command_clone << "... ";
		auto response(console(command_clone.c_str()));
		std::cout << std::endl;

		updated = true;

	}

	return updated;

}

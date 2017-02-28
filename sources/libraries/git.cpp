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

std::chrono::system_clock::time_point git_timestamp(filesystem::path root)
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

bool git_prepare(
	const filesystem::path&	path, 
	std::string				timestamp,
	std::string				address
)
{

	bool repo_changed;

	if(!is_timestamp_expired(timestamp))
	{
		std::cout << "Git repository was checked within the last 24h, will not check it again" << std::endl;
		repo_changed = false;
	}
	else if(filesystem::exists(path /".git"))
	{

		std::cout << "The local library has not been compared to the git repo in the last 24h" << std::endl;
		std::cout << "Now reseting the local workspace... ";

		std::string command_reset(
			"cd " + path.string()	+ "&&" +
			"git reset"				+ "&&" +
			"git checkout ."		+ "&&" +
			"git clean -fdx"
		);
		system(command_reset.c_str());
		std::cout << "done" << std::endl;

		std::cout << "Now pulling from the the repo... ";
		std::string command_pull(
			"cd " + path.string() + "&&"
			"git pull"
		);
		std::string response(console(command_pull.c_str()));
		std::cout << "done" << std::endl;
		repo_changed = ("Already up-to-date." == response);
		
	}
	else
	{

		std::cout << "Could not find the local workspace, will clone it now" << std::endl;

		filesystem::remove_all(path);
		std::string command_clone("git clone " + address + " " + path.string());
		std::cout << command_clone << "... ";
		auto response(console(command_clone.c_str()));
		std::cout << std::endl;

		repo_changed = true;

	}

	return repo_changed;

}

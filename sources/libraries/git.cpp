#include "files.h"
#include "os.h"
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>
#include <ctime>
#include <string>

using namespace std::experimental;

//std::chrono::system_clock::time_point git_get_time_of_latest_commit(filesystem::path root)
//{
//
//	auto string(
//		console(
//			"cd "+root.string()+"&&"+
//			"git log -1 --date=raw --pretty=format:%cd"
//		)
//	);
//	std::regex	regex("([0-9]+) ([-+][0-9]+)");
//	std::smatch	smatch;
//	std::regex_search(string, smatch, regex);
//	return std::chrono::system_clock::time_point(
//		std::chrono::seconds(std::stoi(smatch[1]))
//	);
//
//}

void git_reset(const filesystem::path& path)
{

	std::cout << "now reseting the local workspace... ";

	std::string command_reset(
		"cd " + path.string() + "&&" +
		"git reset" + "&&" +
		"git checkout ." + "&&" +
		"git clean -fdx"
	);
	std::string response_reset(console(command_reset.c_str()));
	std::cout << "done" << std::endl;

}

bool git_pull(const filesystem::path& path)
{

	std::cout << "now pulling the repo... ";
	std::string command(
		"cd " + path.string() + "&&"
		"git pull"
	);
	std::string response(console(command.c_str()));
	std::cout << "done" << std::endl;
	return ("Already up-to-date." == response);

}

void git_clone(const filesystem::path& path, std::string address)
{

	std::cout << "now cloning the repository... " << std::endl;

	filesystem::remove_all(path);
	std::string command_clone("git clone " + address + " " + path.string());
	auto response(console(command_clone.c_str()));
	std::cout << "done" << std::endl;

}

bool git_update(filesystem::path timestamp, filesystem::path local, std::string remote)
{

	bool updated;

	if(filesystem::exists(local / ".git"))
	{

		std::cout << "found the local workspace" << std::endl;

		auto git_timestamp_delta(
			std::chrono::system_clock::now() -
			read_timestamp(timestamp)
		);
		std::cout << "it was last updated ";
		if(git_timestamp_delta < std::chrono::hours(1))
		{
			std::cout	<< std::chrono::duration_cast<std::chrono::minutes>(
						      git_timestamp_delta
						   ).count() << " minutes ago" << std::endl;
		}
		else
		{
			std::cout	<< std::chrono::duration_cast<std::chrono::hours>(
						      git_timestamp_delta
						   ).count() << " hours ago" << std::endl;
		}

		git_reset(local);
		if(git_timestamp_delta > std::chrono::hours(24))
		{
			updated = git_pull(local);
			update_timestamp(timestamp);
		}
		else
		{
			updated = false;
		}


	}
	else
	{

		std::cout << "could not find the local workspace" << std::endl;

		git_clone(local, remote);
		updated = true;

	}

	return updated;

}
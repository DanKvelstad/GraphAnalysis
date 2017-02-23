#include "msbuild.h"
#include "filesystem.h"
#include "os.h"
#include "git.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <thread>
#include "libraries.h"

using namespace std::experimental;

void build_vs(const filesystem::path& path_root)
{

	auto path_msvc(path_root / "googletest" / "msvc");

	std::cout << "upgrading the solution... ";
	std::string command_upgrade(find_devenv() + " " + (path_msvc / "gtest.sln").string() + " /upgrade");
	system(command_upgrade.c_str());
	std::cout << "done" << std::endl;

	if(platform::x64 == get_platform())
	{
		msbuild_convert_to_x64(path_msvc);
	}

	msbuild_change_runtime(path_msvc);
	
	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = " /p:Configuration=Debug";
		break;
	case configuration::release:
		configuration = " /p:Configuration=Release";
		break;
	default:
		throw std::exception("unknown configuration");
		break;
	}

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = " /p:Platform=x86";
		break;
	case platform::x64:
		platform = " /p:Platform=x64";
		break;
	default:
		throw std::exception("unknown platform");
		break;
	}

	std::string command_build(
		"cd " + (path_root / "googletest" / "msvc").string() + "&&" +
		find_msbuild() + " gtest.sln /t:Build" + configuration+platform
	);

	std::cout << "building with " << configuration << " and " << platform << std::endl;
	system(command_build.c_str());

}

void build_make(const filesystem::path& path_root)
{

}

void move_result(const filesystem::path& path_root)
{

	filesystem::path output(path_root /"googletest"/"msvc"/"gtest");
	filesystem::path gtest;
	filesystem::path gtest_main;

	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		output /= "Debug";
		filesystem::copy_file(
			output/"gtestd.lib",
			find_output()/"gtestd.lib"
		);
		filesystem::copy_file(
			output/"gtest_maind.lib",
			find_output()/"gtest_maind.lib"
		);
		break;
	case configuration::release:
		output /= "Release";
		filesystem::copy_file(
			output / "gtest.lib",
			find_output() / "gtest.lib"
		);
		filesystem::copy_file(
			output / "gtest_main.lib",
			find_output() / "gtest_main.lib"
		);
		break;
	default:
		throw std::exception("unknown configuration");
		break;
	}

}

void library_googletest(void)
{

	filesystem::path googletest(find_libraries()/"googletest");

	if(
		git_prepare(
			"googletest",
			googletest, 
			"timestamp_googletest.txt",
			"https://github.com/google/googletest.git"
		)
	)
	{

		switch(get_os())
		{
		case os::windows:
			std::cout << "rebuilding with msbuild" << std::endl;
			build_vs(googletest);
			break;
		case os::linux:
			std::cout << "rebuilding with make" << std::endl;
			build_make(googletest);
			break;
		default:
			break;
		}

		move_result(googletest);
		
	}

}
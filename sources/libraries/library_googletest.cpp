#include "filesystem.h"
#include "os.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <thread>
#include "libraries.h"

using namespace std::experimental;

bool git_prepare(const filesystem::path& googletest)
{

	if(!is_timestamp_expired("timestamp_googletest.txt"))
	{
		std::cout << "googletest was updated and built within the last 24h, leaving it as-is" << std::endl;
		return false;
	}

	std::cout << "googletest is older than 24h, updating and rebuilding it" << std::endl;

	if(filesystem::exists(googletest/".git"))
	{

		std::cout << "found the googletest repository" << std::endl;

		std::cout << "now resetting it" << std::endl;
		std::string command_reset(
			"cd " + googletest.string()	+ "&&" +
			"git reset"					+ "&&" +
			"git checkout ."			+ "&&" +
			"git clean -fdx"
		);
		system(command_reset.c_str());

		std::cout << "now pulling from the master branch" << std::endl;
		std::string command_pull(
			"cd " + googletest.string() + "&&"
			"git pull"
		);
		system(command_pull.c_str());

	}
	else
	{

		std::cout << "could not find googletest repository" << std::endl;

		filesystem::remove_all(googletest);
		std::string github("https://github.com/google/googletest.git");
		std::string command_clone("git clone " + github + " " + googletest.string());
		std::cout << command_clone << std::endl;
		system(command_clone.c_str());

	}

	return true;

}

void change_runtime(const filesystem::path& path_msvc)
{

	for(auto& file : filesystem::directory_iterator(path_msvc))
	{
		if(".vcxproj" == file.path().extension())
		{

			auto path_old(file.path());
			auto path_new(path_old);
			path_new.replace_extension(".tmp");

			std::ifstream is(path_old.string());
			std::ofstream os(path_new.string());

			std::string buffer(1024, '\0');
			for(
				is.getline(&buffer[0], buffer.size());
				is;
				is.getline(&buffer[0], buffer.size())
			)
			{

				std::string line(buffer.c_str());

				if(std::string::npos != line.find("<RuntimeLibrary>MultiThreaded</RuntimeLibrary>"))
				{
					os << "      <RuntimeLibrary>MultiThreadedDll</RuntimeLibrary>\n";
				}
				else if(std::string::npos != line.find("<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>"))
				{
					os << "      <RuntimeLibrary>MultiThreadedDebugDll</RuntimeLibrary>\n";
				}
				else
				{
					os << line << "\n";
				}
				
			}

			os.close();
			is.close();

			filesystem::copy_file(
				path_new, 
				path_old, 
				filesystem::copy_options::overwrite_existing
			);
			filesystem::remove(path_new);

		}

	}

}

void build_vs(const filesystem::path& path_root)
{

	auto path_msvc(path_root / "googletest" / "msvc");

	std::cout << "upgrading the solution... ";
	std::string command_upgrade(find_devenv() + " " + (path_msvc / "gtest.sln").string() + " /upgrade");
	system(command_upgrade.c_str());
	std::cout << "done" << std::endl;

	change_runtime(path_msvc);
	
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

void library_googletest(void)
{

	filesystem::path googletest(find_libraries()/"googletest");

	if(git_prepare(googletest))
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
	
	}

}
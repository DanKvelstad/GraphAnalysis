#include "msbuild.h"
#include "files.h"
#include "os.h"
#include "git.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <thread>
#include <chrono>
#include "libraries.h"

using namespace std::experimental;

filesystem::path path_to_gtest_all(void)
{
	
	auto library = path_to_libraries() / build_directory();
	if(!filesystem::exists(library))
	{
		filesystem::create_directories(library);
	}
	
	return library / "gtest.lib";

}

filesystem::path path_to_gtest_main(void)
{
	
	auto library = path_to_libraries() / build_directory();
	if(!filesystem::exists(library))
	{
		filesystem::create_directories(library);
	}
	
	return library / "gtest_main.lib";

}

void move_result(const filesystem::path& path_to_workspace)
{

	auto path_to_built_files = path_to_workspace / "googletest" / "msvc" / "gtest";
	
	switch(get_configuration())
	{
	case configuration::debug:
		path_to_built_files		/= "debug";
		filesystem::rename(path_to_built_files / "gtestd.lib",		path_to_gtest_all());
		filesystem::rename(path_to_built_files / "gtest_maind.lib",	path_to_gtest_main());
		break;
	case configuration::release:
		path_to_built_files /= "release";
		filesystem::rename(path_to_built_files / "gtest.lib",		path_to_gtest_all());
		filesystem::rename(path_to_built_files / "gtest_main.lib",	path_to_gtest_main());
		break;
	default:
		throw std::logic_error("unknown configuration");
	}

	auto path_to_library_includes(path_to_library_includes() / "gtest");
	if(filesystem::exists(path_to_library_includes))
	{
		filesystem::remove_all(path_to_library_includes);
	}
	filesystem::rename(path_to_workspace/"googletest"/"include"/"gtest", path_to_library_includes);

}

void build(const filesystem::path& path_root)
{

	auto path_msvc(path_root / "googletest" / "msvc");

	std::ofstream ofs(path_to_intermediate()/"libraries"/"googletest.log");

	std::cout << "upgrading the solution... ";
	std::cout.flush();
	std::string command_upgrade(path_to_devenv() + " " + (path_msvc / "gtest.sln").string() + " /upgrade");
	ofs << command_upgrade << std::endl;
	ofs << console(command_upgrade.c_str());
	std::cout << "done" << std::endl;

	if(platform::x64 == get_platform())
	{
		msbuild_convert_to_x64(path_msvc);
	}

	msbuild_change_runtime(path_msvc);
	msbuild_remove_project(path_msvc, "gtest_unittest");
	msbuild_remove_project(path_msvc, "gtest_prod_test");
	
	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = " /p:Configuration=debug";
		break;
	case configuration::release:
		configuration = " /p:Configuration=release";
		break;
	default:
		throw std::logic_error("unknown configuration");
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
		throw std::logic_error("unknown platform");
	}

	std::cout << "building solution... ";
	std::cout.flush();
	std::string command_build(
		"cd " + (path_root / "googletest" / "msvc").string() + "&&" +
		path_to_msbuild() + " gtest.sln /t:Build" + configuration + platform
	);
	ofs << command_build;
	ofs << console(command_build.c_str());
	std::cout << "done" << std::endl;

	move_result(path_root);

}

bool verify_output(void)
{
	return	filesystem::exists(path_to_gtest_all()) &&
			filesystem::exists(path_to_gtest_main());
}
#include "files.h"
#include "os.h"
#include <regex>
#include <experimental/filesystem>

using namespace std::experimental;

filesystem::path path_to_root(void)
{

	auto path(filesystem::current_path());
	while(!filesystem::exists(path / ".git"))
	{
		path = path.parent_path();
		if(path.empty())
		{
			throw std::logic_error("could not find intermediate folder");
		}
	}
	return path;
	
}

filesystem::path path_to_timestamps(void)
{

	auto path(path_to_root()/"intermediate"/"libraries"/"timestamps");

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_output(void)
{

	auto path(path_to_root()/"binaries"/build_directory());
	
	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_intermediate(void)
{

	auto path(path_to_root()/"intermediate"/"libraries"/build_directory());
	
	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_repositories(void)
{

	auto path(path_to_root() / "intermediate" / "libraries" / "repositories");

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_libraries(void)
{

	auto path(path_to_root()/"libraries");

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_library_includes(void)
{

	auto path(path_to_libraries()/"include");

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

std::string path_to_msbuild(void)
{

	auto path_msbuild_garbled(
		console(
			"reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\14.0\" /v MSBuildToolsPath"
		)
	);
	std::regex regex(".:\\\\.*");
	std::smatch smatch;
	std::regex_search(path_msbuild_garbled, smatch, regex);

	filesystem::path msbuild(smatch[0].str());
	msbuild /= "msbuild.exe";
	
	if(!filesystem::exists(msbuild))
	{
		throw std::logic_error("msbuild not found");
	}

	return "\"" + msbuild.string() + "\"";
	
}

std::string path_to_devenv(void)
{
		
	auto path_msbuild_garbled(
		console(
			"reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\devenv.exe\""
		)
	);
	std::regex regex(".:\\\\.*");
	std::smatch smatch;
	std::regex_search(path_msbuild_garbled, smatch, regex);

	filesystem::path msbuild(smatch[0].str());
	
	if(!filesystem::exists(msbuild))
	{
		throw std::logic_error("devenv not found");
	}

	return "\"" + msbuild.string() + "\"";
	
}
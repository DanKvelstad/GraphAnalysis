#include "filesystem.h"
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
	return path_to_root()/"intermediate";
}

filesystem::path path_to_output(void)
{

	auto path(path_to_root() / "binaries");
	
	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = "Debug";
		break;
	case configuration::release:
		configuration = "Release";
		break;
	default:
		throw std::logic_error("unknown configuration");
		break;
	}

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = "Win32";
		break;
	case platform::x64:
		platform = "x64";
		break;
	default:
		throw std::logic_error("unknown platform");
		break;
	}

	path /= ("Windows " + platform + " " + configuration);

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_intermediate(void)
{

	auto path(path_to_root()/"intermediate");
	
	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = "Debug";
		break;
	case configuration::release:
		configuration = "Release";
		break;
	default:
		throw std::logic_error("unknown configuration");
		break;
	}

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = "Win32";
		break;
	case platform::x64:
		platform = "x64";
		break;
	default:
		throw std::logic_error("unknown platform");
		break;
	}

	path /= ("Windows " + platform + " " + configuration);
	path /= "Libraries";

	if(!filesystem::exists(path))
	{
		filesystem::create_directories(path);
	}

	return path;

}

filesystem::path path_to_libraries(void)
{
	return path_to_root()/"libraries";
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
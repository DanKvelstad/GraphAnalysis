#include "libraries.h"
#include <regex>
#include <filesystem>

using namespace std::experimental;

filesystem::path find_root(void)
{

	auto path(filesystem::current_path());
	while(!filesystem::exists(path / ".git"))
	{
		path = path.parent_path();
		if(path.empty())
		{
			throw std::exception("could not find intermediate folder");
		}
	}
	return path;
	
}

filesystem::path find_output(void)
{

	auto path(find_root() / "binaries");

#ifdef _DEBUG
	std::string configuration("Debug");
#else
	std::string configuration("Release");
#endif

#ifdef WIN32
	std::string platform("Win32");
#else
	std::string platform("x64");
#endif

	path /= ("Windows " + platform + " " + configuration);

	if(!filesystem::exists(path))
	{
		throw std::exception("could not find intermediate folder");
	}

	return path;

}

filesystem::path find_intermediate(void)
{

	auto path(find_root()/"intermediate");

#ifdef _DEBUG
	std::string configuration("Debug");
#else
	std::string configuration("Release");
#endif

#ifdef WIN32
	std::string platform("Win32");
#else
	std::string platform("x64");
#endif

	path /= ("Windows " + platform + " " + configuration);
	path /= "Libraries";

	if(!filesystem::exists(path))
	{
		throw std::exception("could not find intermediate folder");
	}

	return path;

}

filesystem::path find_libraries(void)
{
	return find_root()/"libraries";
}

std::string find_msbuild(void)
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
		throw std::exception("msbuild not found");
	}

	return "\"" + msbuild.string() + "\"";
	
}

std::string find_devenv(void)
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
		throw std::exception("devenv not found");
	}

	return "\"" + msbuild.string() + "\"";
	
}
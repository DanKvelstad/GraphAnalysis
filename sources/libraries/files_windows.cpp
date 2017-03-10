#include <string>
#include <regex>
#include "files.h"
#include "os.h"

std::string build_directory(void)
{

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = "x86";
		break;
	case platform::x64:
		platform = "x64";
		break;
	default:
		throw std::logic_error("unknown platform");
		break;
	}

	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = "debug";
		break;
	case configuration::release:
		configuration = "release";
		break;
	default:
		throw std::logic_error("unknown configuration");
		break;
	}

	return ("windows " + platform + " " + configuration);

}

filesystem::path path_to_devenv(void)
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

	if (filesystem::exists(msbuild))
	{
		return msbuild;
	}
	else
	{
		return filesystem::path();
	}

}

filesystem::path path_to_cmake(void)
{

	auto cmake(filesystem::path("C:/") / "Program Files" / "CMake" / "bin" / "cmake.exe");

	if (filesystem::exists(cmake))
	{
		return cmake;
	}
	else
	{
		return filesystem::path();
	}

}

filesystem::path extract_path(std::string input)
{

	std::regex regex(".:\\\\.*");
	std::smatch smatch;
	std::regex_search(input, smatch, regex);

	return smatch[0].str();

}

filesystem::path path_to_msbuild(void)
{

	auto path_to_msbuild(
		extract_path(
			console(
				"reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\MSBuild\\ToolsVersions\\14.0\" /v MSBuildToolsPath"
			)
		) / "msbuild.exe"
	);

	if (!filesystem::exists(path_to_msbuild))
	{
		path_to_msbuild = extract_path(
			console(
				"reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\14.0\" /v MSBuildToolsPath"
			)
		) / "msbuild.exe";
	}

	if (!filesystem::exists(path_to_msbuild))
	{
		throw std::logic_error("msbuild not found");
	}

	return path_to_msbuild;

}
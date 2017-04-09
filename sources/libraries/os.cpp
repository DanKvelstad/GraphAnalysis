#include "os.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#ifdef _WIN32
#  define POPEN  _popen
#  define PCLOSE _pclose
#else
#  define POPEN  popen
#  define PCLOSE pclose
#endif

os get_os(void)
{
#ifdef _WIN32
	return os::windows;
#else
	return os::linux;
#endif
}

configuration get_configuration(void)
{
#ifdef _DEBUG
	return configuration::debug;
#else
	return configuration::release;
#endif
}

platform get_platform(void)
{
#ifdef WIN32
	return platform::x86;
#else
	return platform::x64;
#endif
}

std::string console(const std::string& cmd)
{

	std::shared_ptr<FILE> pipe(
		POPEN(cmd.c_str(), "rt"), 
		PCLOSE
	);
	if (!pipe)
	{
		throw std::runtime_error("popen failed");
	}

	std::string result;
	for (
		std::array<char, 128> buffer;
		buffer.data()==fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get());
		result += buffer.data()
	);

	if (!feof(pipe.get()))
	{
		throw std::logic_error("failed to read the pipe to the end.");
	}

	return result;

}
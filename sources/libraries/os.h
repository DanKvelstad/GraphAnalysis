#pragma once
#include <string>

enum class os
{
	windows,
	linux
};
os get_os();

enum class configuration
{
	debug,
	release
};
configuration get_configuration();

enum class platform
{
	x86,
	x64
};
platform get_platform();

std::string console(const std::string& cmd);
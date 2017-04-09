#include "files.h"
#include <fstream>
#include <chrono>

using namespace std::chrono_literals;

std::chrono::system_clock::time_point read_timestamp(filesystem::path file)
{

	std::chrono::system_clock::time_point time_point;

	if(filesystem::exists(file))
	{
		std::ifstream stream(file);
		long long count;
		stream >> count;
		time_point = std::chrono::system_clock::time_point(
			std::chrono::system_clock::duration(count)
		);
	}
	else
	{
		time_point = std::chrono::system_clock::time_point(
			std::chrono::system_clock::duration(0)
		);
	}

	return time_point;

}

void update_timestamp(filesystem::path file)
{

	std::ofstream stream(file);
	stream << std::chrono::system_clock::now().time_since_epoch().count();

}
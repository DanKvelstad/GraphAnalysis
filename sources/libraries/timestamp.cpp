#include "libraries.h"
#include <fstream>
#include <chrono>

using namespace std::chrono_literals;

bool is_timestamp_expired(std::string id)
{

	auto timestamp(find_intermediate()/id);

	bool time_to_update;
	if(filesystem::exists(timestamp))
	{
		std::ifstream stream(timestamp);
		long long count;
		stream >> count;
		std::chrono::system_clock::time_point timeout(
			std::chrono::system_clock::duration(count)+24h
		);
		time_to_update = std::chrono::system_clock::now() > timeout;
	}
	else
	{
		time_to_update = true;
	}

	if(time_to_update)
	{
		std::ofstream stream(timestamp.string());
		stream << std::chrono::system_clock::now().time_since_epoch().count();
	}

	return time_to_update;

}
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
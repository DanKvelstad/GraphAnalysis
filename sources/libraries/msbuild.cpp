#include <experimental/filesystem>
#include <fstream>
#include <algorithm>
#include "os.h"

using namespace std::experimental;

void msbuild_change_runtime(const filesystem::path& path_msvc)
{
	
	for(auto& file : filesystem::directory_iterator(path_msvc))
	{
		if(".vcxproj" == file.path().extension())
		{

			auto path_old(file.path());
			auto path_new(path_old);
			path_new.replace_extension(".tmp");

			std::ifstream is(path_old.string());
			std::ofstream os(path_new.string());

			
			for(std::string line; std::getline(is, line); )
			{

				if(std::string::npos != line.find("<RuntimeLibrary>MultiThreaded</RuntimeLibrary>"))
				{
					os << "      <RuntimeLibrary>MultiThreadedDll</RuntimeLibrary>\n";
				}
				else if(std::string::npos != line.find("<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>"))
				{
					os << "      <RuntimeLibrary>MultiThreadedDebugDll</RuntimeLibrary>\n";
				}
				else
				{
					os << line << "\n";
				}
				
			}
			
			os.close();
			is.close();

			filesystem::rename(path_new, path_old);

		}

	}

}

void msbuild_ensure_platform(const filesystem::path& path_msvc)
{

	std::string platform_string;
	switch (get_platform())
	{
	case platform::x86:
		platform_string = "Win32";
		break;
	case platform::x64:
		platform_string = "x64";
		break;
	default:
		throw std::logic_error("unknown platform");
	}

	for(auto& file : filesystem::directory_iterator(path_msvc))
	{

		if(".vcxproj" == file.path().extension() || ".sln" == file.path().extension())
		{

			auto path_old(file.path());
			auto path_new(path_old);
			path_new.replace_extension(".tmp");

			std::ifstream is(path_old.string());
			std::ofstream os(path_new.string());

			for(std::string line; std::getline(is, line); )
			{
				
				std::string::size_type pos;
				if (platform::x64 == get_platform())
				{
					pos = 0u;
					while (std::string::npos != (pos = line.find("X86", pos)))
					{
						line.replace(pos, 3, "X64");
						pos += 3;
					}
				}
				pos = 0u;
				while(std::string::npos  != (pos = line.find("Win32", pos)))
				{
					line.replace(pos, 5, platform_string);
					pos += platform_string.length();
				}
				pos = 0u;
				while(std::string::npos != (pos = line.find("x86", pos)))
				{
					line.replace(pos, 3, platform_string);
					pos += platform_string.length();
				}

				os << line << "\n";

			}

			os.close();
			is.close();

			filesystem::rename(path_new, path_old);

		}

	}

}

void msbuild_remove_project(const filesystem::path& path_msvc, std::string projectname)
{

	for(auto& file : filesystem::directory_iterator(path_msvc))
	{

		if(".sln" == file.path().extension())
		{

			auto path_old(file.path());
			auto path_new(path_old);
			path_new.replace_extension(".tmp");

			std::ifstream is(path_old.string());
			std::ofstream os(path_new.string());

			bool delete_this_line(false);
			for (std::string line; std::getline(is, line); )
			{

				if(delete_this_line)
				{
					delete_this_line = false;
				}
				if(std::string::npos != line.find(projectname))
				{
					delete_this_line = true;
				}
				else
				{
					os << line << "\n";
				}

			}

			os.close();
			is.close();

			filesystem::rename(path_new, path_old);

		}

	}

}
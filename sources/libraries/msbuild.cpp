#include <filesystem>
#include <fstream>
#include <algorithm>

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

			std::string buffer(1024, '\0');
			for(
				is.getline(&buffer[0], buffer.size());
				is;
				is.getline(&buffer[0], buffer.size())
			)
			{

				std::string line(buffer.c_str());

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

			filesystem::copy_file(
				path_new, 
				path_old, 
				filesystem::copy_options::overwrite_existing
			);
			filesystem::remove(path_new);

		}

	}

}

void msbuild_convert_to_x64(const filesystem::path& path_msvc)
{

	for(auto& file : filesystem::directory_iterator(path_msvc))
	{

		if(".vcxproj" == file.path().extension() || ".sln" == file.path().extension())
		{

			auto path_old(file.path());
			auto path_new(path_old);
			path_new.replace_extension(".tmp");

			std::ifstream is(path_old.string());
			std::ofstream os(path_new.string());

			std::string buffer(1024, '\0');
			for(
				is.getline(&buffer[0], buffer.size());
				is;
				is.getline(&buffer[0], buffer.size())
				)
			{

				std::string line(buffer.c_str());

				std::string::size_type pos = 0u;
				while(std::string::npos  != (pos = line.find("Win32", pos)))
				{
					line.replace(pos, 5, "x64");
					pos += 3;
				}
				pos = 0u;
				while(std::string::npos != (pos = line.find("x86", pos)))
				{
					line.replace(pos, 3, "x64");
					pos += 3;
				}

				os << line << "\n";

			}

			os.close();
			is.close();

			filesystem::copy_file(
				path_new,
				path_old,
				filesystem::copy_options::overwrite_existing
			);
			filesystem::remove(path_new);

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
			std::string buffer(1024, '\0');
			for(
				is.getline(&buffer[0], buffer.size());
				is;
				is.getline(&buffer[0], buffer.size())
				)
			{

				std::string line(buffer.c_str());

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

			filesystem::copy_file(
				path_new,
				path_old,
				filesystem::copy_options::overwrite_existing
			);
			filesystem::remove(path_new);

		}

	}

}
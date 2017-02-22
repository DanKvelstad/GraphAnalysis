#include <filesystem>
#include <fstream>

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

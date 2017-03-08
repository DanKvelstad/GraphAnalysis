#include "files.h"

void copy_libraries_to_binaries(void)
{

	for(
		filesystem::directory_iterator it(path_to_libraries()/build_directory()); 
		filesystem::directory_iterator()!=it; 
		++it
	)
	{
		if(filesystem::is_regular_file(it->status()))
		{
			filesystem::copy_file(
				*it,
				path_to_output()/it->path().filename(),
				filesystem::copy_options::overwrite_existing
			);
		}
	}

}
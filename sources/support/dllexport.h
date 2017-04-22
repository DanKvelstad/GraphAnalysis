#ifdef _WIN32 
#	if PROJECT_SUPPORT
#		define DLLEXPORT __declspec(dllexport)
#	else
#		define DLLEXPORT __declspec(dllimport)
#	endif
#else
#	define DLLEXPORT	
#endif
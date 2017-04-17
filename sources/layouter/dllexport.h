#pragma once

#ifdef _WIN32 
#	if PROJECT_LAYOUTER
#		define DLLEXPORT __declspec(dllexport)
#		define TPLEXPORT
#	else
#		define DLLEXPORT __declspec(dllimport)
#		define TPLEXPORT extern
#	endif
#else
#	define DLLEXPORT	
#	define TPLEXPORT
#endif

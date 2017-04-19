#pragma once

#include "dllexport.h"

class SkCanvas;

//void raster(int width, int height,
//	void(*draw)(SkCanvas*),
//	const char* path);

DLLEXPORT void skpdf(int width, int height,
	void(*draw)(SkCanvas*),
	const char* path);

DLLEXPORT void picture(int width, int height,
	void(*draw)(SkCanvas*),
	const char* path);

DLLEXPORT void example(SkCanvas* canvas);
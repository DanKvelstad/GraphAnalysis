#include "presenter.h"

#include <algorithm>
#include <Windows.h>

#include "SkCanvas.h"
#include "SkStream.h"
#include "SkSurface.h"
#include "SkTextBox.h"

// todo: figure out how to do this without windows.h
// stub out openGl dependency, which isn't needed in this case.
extern "C"
{
	PROC WINAPI __imp_wglGetProcAddress(LPCSTR)
	{
		abort();
		return nullptr;
	}

	HGLRC WINAPI  __imp_wglGetCurrentContext()
	{
		abort();
		return nullptr;
	}
}

void draw(
	const path&		output,
	states			the_states,
	edges			the_edges
)
{

	auto spacing(the_edges.get_text_dimensions().first);
	the_states.set_spacing(spacing, spacing);

	auto surface(
		SkSurface::MakeRasterN32Premul(
			the_states.get_workspace_width(),
			the_states.get_workspace_height()
		)
	);
	surface->getCanvas()->clear(SK_ColorWHITE);
	
	the_states.draw(*surface->getCanvas());
	the_edges.draw(*surface->getCanvas(), the_states);

	sk_sp<SkImage> img(surface->makeImageSnapshot());
	if (!img) 
	{ 
		throw std::exception(); 
	}
	
	sk_sp<SkData> png(img->encode());
	if (!png) 
	{
		throw std::exception();
	}

	create_directories(output.parent_path());
	auto output_string(output.string());
	SkFILEWStream out(output_string.c_str());
	(void)out.write(png->data(), png->size());

}
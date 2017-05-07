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
	linked_state	states,
	linked_edge		edges
)
{

	int max_state_length(0);
	int surface_width(0);
	int surface_height(0);
	for (auto it(&states); nullptr!=it; it = it->next())
	{
		if (max_state_length<strlen(it->get().name))
		{
			max_state_length = static_cast<unsigned>(strlen(it->get().name));
		}
		if (surface_width < it->get().right)
		{
			surface_width = it->get().right;
		}
		if (surface_height < it->get().bottom)
		{
			surface_height = it->get().bottom;
		}
	}
	surface_width  += states.get_spacing();
	surface_height += states.get_spacing();

	int max_edge_length(0);
	for (auto it(&edges); nullptr != it; it = it->next())
	{
		if (max_edge_length<strlen(it->get_name()))
		{
			max_edge_length = static_cast<unsigned>(strlen(it->get_name()));
		}
	}

	sk_sp<SkSurface> surface(
		SkSurface::MakeRasterN32Premul(surface_width, surface_height)
	);

	surface->getCanvas()->clear(SK_ColorWHITE);
	states.draw(surface->getCanvas());
	edges.draw(surface->getCanvas(), states);

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
	
	auto output_string(output.string());
	SkFILEWStream out(output_string.c_str());
	(void)out.write(png->data(), png->size());

}
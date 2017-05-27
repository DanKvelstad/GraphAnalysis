#include "presenter.h"

#include <algorithm>
#include <Windows.h>

#include "SkCanvas.h"
#include "SkStream.h"
#include "SkSurface.h"
#include "SkTextBox.h"

#include "states.h"
#include "edges.h"

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

presenter::presenter()
: the_states(std::make_unique<states>())
, the_edges(std::make_unique<edges>())
{
}

presenter::~presenter()
{
}

void presenter::emplace_state(const std::string & name, unsigned x, unsigned y)
{
	the_states->emplace(name, x, y);
}

void presenter::emplace_edge(unsigned source, unsigned target, const std::string & name)
{
	the_edges->emplace(source, target, name);
}

void presenter::draw(const path & output)
{

	the_edges->update_spacing(*the_states);

	auto surface(
		SkSurface::MakeRasterN32Premul(
			the_states->get_workspace_width(),
			the_states->get_workspace_height()
		)
	);
	surface->getCanvas()->clear(SK_ColorWHITE);
	
	the_states->draw(*surface->getCanvas());
	the_edges->draw(*surface->getCanvas(), *the_states);

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

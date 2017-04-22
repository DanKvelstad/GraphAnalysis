#include "presenter.h"

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
	linked_state& states,
	linked_edge&  edges
)
{

	sk_sp<SkSurface> surface(
		SkSurface::MakeRasterN32Premul(350, 150)
	);

	states.draw(surface->getCanvas());
	edges.draw(surface->getCanvas());

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
	
	SkFILEWStream out("state_machine.png");
	(void)out.write(png->data(), png->size());

}
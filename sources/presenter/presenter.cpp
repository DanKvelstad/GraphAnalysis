#include "presenter.h"

#include <Windows.h>

#include "SkCanvas.h"
#include "SkStream.h"
#include "SkSurface.h"

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

void draw_statemachine(void)
{

	sk_sp<SkSurface> surface(
		SkSurface::MakeRasterN32Premul(350, 150)
	);
	SkCanvas* canvas = surface->getCanvas();

	SkPaint paint;
	paint.setStyle(SkPaint::kStroke_Style);
	paint.setAntiAlias(true);
	paint.setStrokeWidth(4);
	paint.setColor(0xff4281A4);

	canvas->drawRect(
		SkRect::MakeXYWH(50, 50, 100, 50), 
		paint
	);

	canvas->drawRect(
		SkRect::MakeXYWH(200, 50, 100, 50),
		paint
	);

	canvas->drawLine(
		150, 75,
		200, 75,
		paint
	);

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
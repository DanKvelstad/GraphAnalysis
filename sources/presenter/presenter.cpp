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

void draw_statemachine(void)
{

	sk_sp<SkSurface> surface(
		SkSurface::MakeRasterN32Premul(350, 150)
	);
	SkCanvas* canvas = surface->getCanvas();

	SkPaint paint_rectangle;
	paint_rectangle.setStyle(SkPaint::kStroke_Style);
	paint_rectangle.setAntiAlias(true);
	paint_rectangle.setStrokeWidth(4);
	paint_rectangle.setColor(SK_ColorBLUE);

	SkPaint paint_line;
	paint_line.setAntiAlias(true);
	paint_line.setStrokeWidth(4);
	paint_line.setColor(SK_ColorRED);

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorWHITE);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	canvas->drawRect(
		SkRect::MakeXYWH(50, 50, 100, 50), 
		paint_rectangle
	);
	SkTextBox tb0;
	tb0.setBox(50, 50, 150, 100);
	tb0.setText("s0", 2, paint_text);
	tb0.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	tb0.draw(canvas);

	canvas->drawRect(
		SkRect::MakeXYWH(200, 50, 100, 50),
		paint_rectangle
	);
	SkTextBox tb1;
	tb1.setBox(200, 50, 300, 100);
	tb1.setText("s1", 2, paint_text);
	tb1.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	tb1.draw(canvas);

	canvas->drawLine(
		150, 75,
		200, 75,
		paint_line
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
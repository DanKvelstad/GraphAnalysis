#include "linked_state.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include <algorithm>

linked_state::linked_state(std::string name, unsigned x, unsigned y)
: width(100)
, height(50)
, spacing(std::min(width, height))
, name(name)
, x(x*(width +spacing)+spacing)
, y(y*(height+spacing)+spacing)
{
}

linked_state::~linked_state()
{
}

void linked_state::emplace(std::string name, unsigned x, unsigned y)
{

	if (linked)
	{
		linked->emplace(name, x, y);
	}
	else
	{
		linked = std::make_unique<linked_state>(name, x, y);
	}

}

linked_state* linked_state::next(void) 
{
	
	return linked.get();

}

void linked_state::draw(SkCanvas* canvas) const
{

	SkPaint paint_rectangle;
	paint_rectangle.setStyle(SkPaint::kStroke_Style);
	paint_rectangle.setAntiAlias(true);
	paint_rectangle.setStrokeWidth(4);
	paint_rectangle.setColor(SK_ColorBLUE);

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorMAGENTA);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	canvas->drawRect(
		SkRect::MakeXYWH(
			static_cast<SkScalar>(x), 
			static_cast<SkScalar>(y),
			static_cast<SkScalar>(width), 
			static_cast<SkScalar>(height)
		),
		paint_rectangle
	);
	SkTextBox tb0;
	tb0.setBox(
		static_cast<SkScalar>(x), 
		static_cast<SkScalar>(y), 
		static_cast<SkScalar>(x+width), 
		static_cast<SkScalar>(y+height)
	);
	tb0.setText(name.c_str(), name.length(), paint_text);
	tb0.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	tb0.draw(canvas);

	if (linked)
	{
		linked->draw(canvas);
	}

}
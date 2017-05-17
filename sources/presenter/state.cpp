#include "state.h"
#include "algorithms.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include "SkRegion.h"

state::state(const std::string& name, unsigned x, unsigned y)
: name(name)
, x(x)
, y(y)
{

	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	paint_shape.setStyle(SkPaint::kStroke_Style);
	paint_shape.setAntiAlias(true);
	paint_shape.setStrokeWidth(4);
	paint_shape.setColor(SK_ColorBLACK);

}

state::state(const state & other)
: name(other.name)
, x(other.x)
, y(other.y)
, paint_shape(other.paint_shape)
, paint_text(other.paint_text)
{
}

state::~state(void)
{
}

unsigned state::get_x(void) const
{
	return x;
}

unsigned state::get_y(void) const
{
	return y;
}

std::pair<unsigned, unsigned> state::get_text_dimensions(void)
{
	SkRect bounds;
	paint_text.measureText(name.c_str(), name.length(), &bounds);
	return std::make_pair(
		static_cast<unsigned>(bounds.width()),
		static_cast<unsigned>(bounds.height())
	);
}

void state::draw(SkCanvas& canvas, const SkRegion& region)
{

	auto bounds(region.getBounds());
	bounds.setLTRB(
		bounds.left()   + 5,
		bounds.top()    + 5,
		bounds.right()  - 5,
		bounds.bottom() - 5
	);
	
	canvas.drawRoundRect(
		SkRect::MakeFromIRect(bounds),
		10,
		10,
		paint_shape
	);
	
	SkTextBox text_box;
	text_box.setBox(SkRect::MakeFromIRect(bounds));
	text_box.setText(name.c_str(), name.length(), paint_text);
	text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	text_box.draw(&canvas);

}
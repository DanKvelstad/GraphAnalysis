#include "linked_edge.h"
#include "SkCanvas.h"
#include "SkTextBox.h"

linked_edge::linked_edge(std::string name, const linked_state& source, const linked_state& target)
: name(name)
, source(source.intersection(target))
, target(target.intersection(source))
{
}

linked_edge::~linked_edge()
{
}

void linked_edge::emplace(std::string name, const linked_state& source, const linked_state& target)
{

	if (linked)
	{
		linked->emplace(name, source, target);
	}
	else
	{
		linked = std::make_unique<linked_edge>(name, source, target);
	}

}

linked_edge* linked_edge::next(void) 
{
	
	return linked.get();

}

void linked_edge::draw(SkCanvas* canvas) const
{

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

	canvas->drawLine(
		static_cast<SkScalar>(source.x), static_cast<SkScalar>(source.y),
		static_cast<SkScalar>(target.x), static_cast<SkScalar>(target.y),
		paint_line
	);

}
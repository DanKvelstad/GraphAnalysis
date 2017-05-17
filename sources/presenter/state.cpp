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

std::vector<point> state::get_single_endpoints(const SkRegion& region) const
{

	std::vector<point> endpoints;
	
	auto margin(region.getBounds());

	// 3X   X4X   X5
	// X           X
	//		    
	// X           X
	// 2           6
	// X           X
	//		    
	// X           X
	// 1X   X8X   X7

	endpoints.emplace_back( margin.left()    , margin.bottom()  );
	endpoints.emplace_back( margin.left()    , margin.centerY() );
	endpoints.emplace_back( margin.left()    , margin.top()     );
	endpoints.emplace_back( margin.centerX() , margin.top()     );
	endpoints.emplace_back( margin.right()   , margin.top()     );
	endpoints.emplace_back( margin.right()   , margin.centerY() );
	endpoints.emplace_back( margin.right()   , margin.bottom()  );
	endpoints.emplace_back( margin.centerX() , margin.bottom()  );

	return endpoints;

}

std::vector<point> state::get_double_endpoints_source(const SkRegion& region) const
{

	std::vector<point> endpoints;
	
	auto margin(region.getBounds());

	// XX   XX4   5X
	// 3           X
	//		    
	// X           6
	// X           X
	// 2           X
	//		    
	// X           7
	// X1   8XX   XX

	endpoints.emplace_back( margin.left()    + 5 , margin.bottom()      );
	endpoints.emplace_back( margin.left()        , margin.centerY() + 5 );
	endpoints.emplace_back( margin.left()        , margin.top()     + 5 );
	endpoints.emplace_back( margin.centerX() + 5 , margin.top()         );
	endpoints.emplace_back( margin.right()   - 5 , margin.top()         );
	endpoints.emplace_back( margin.right()       , margin.centerY() - 5 );
	endpoints.emplace_back( margin.right()       , margin.bottom()  - 5 );
	endpoints.emplace_back( margin.centerX() - 5 , margin.bottom()      );

	return endpoints;

}

std::vector<point> state::get_double_endpoints_target(const SkRegion& region) const
{

	std::vector<point> endpoints;

	auto margin(region.getBounds());

	// X3   4XX   XX
	// X           5
	//
	// 2           X
	// X           X
	// X           6
	//
	// 1           X
	// Xx   XX8   7X
	
	endpoints.emplace_back( margin.left()        , margin.bottom()  - 5 );
	endpoints.emplace_back( margin.left()        , margin.centerY() - 5 );
	endpoints.emplace_back( margin.left()    + 5 , margin.top()         );
	endpoints.emplace_back( margin.centerX() - 5 , margin.top()         );
	endpoints.emplace_back( margin.right()       , margin.top()     + 5 );
	endpoints.emplace_back( margin.right()       , margin.centerY() + 5 );
	endpoints.emplace_back( margin.right()   - 5 , margin.bottom()      );
	endpoints.emplace_back( margin.centerX() + 5 , margin.bottom()      );

	return endpoints;

}

void state::draw(SkCanvas& canvas, const SkRegion& region)
{

	auto margin(region.getBounds());
	auto border(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(margin.left()   + 5),
			static_cast<SkScalar>(margin.top()    + 5),
			static_cast<SkScalar>(margin.right()  - 5),
			static_cast<SkScalar>(margin.bottom() - 5)
		)
	);

	canvas.drawRoundRect(
		border,
		10,
		10,
		paint_shape
	);

	auto padding(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(border.left()   + 5),
			static_cast<SkScalar>(border.top()    + 5),
			static_cast<SkScalar>(border.right()  - 5),
			static_cast<SkScalar>(border.bottom() - 5)
		)
	);
	
	SkTextBox content;
	content.setBox(padding);
	content.setText(name.c_str(), name.length(), paint_text);
	content.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	content.draw(&canvas);

}
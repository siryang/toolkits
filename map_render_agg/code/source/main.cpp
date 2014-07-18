#include "mapr_stdafx.h"
#include "agg_frame.h"
#include "platform/agg_platform_support.h"

class MaprenderFrame: public AggFrame
{
public:
	MaprenderFrame(agg::pix_format_e format, bool flip_y): AggFrame(format, flip_y)
	{
	}

	virtual void on_draw()
	{
		addBuffer(100, 100, L"heart.png");
		addBuffer(-50, 400, L"heart.png");
	}
};


int agg_main(int argc, char* argv[])
{
	enum flip_y_e { flip_y = true };
	MaprenderFrame app(agg::pix_format_rgba32, flip_y);
	app.caption("AGG Example. Lion");

	if(app.init(512, 400, agg::window_resize))
	{
		return app.run();
	}
	return 1;
}
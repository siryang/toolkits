#include "mapr_stdafx.h"
#include "agg_frame.h"
#include "agg_ellipse.h"
#include "platform/agg_platform_support.h"

extern "C"
{
	#include "util/nds.h"
};

using namespace std;

class MaprenderFrame: public AggFrame
{
public:
	
	MaprenderFrame(agg::pix_format_e format, bool flip_y): 
		AggFrame(format, flip_y)
	{
		// init window size
		// init center
		// init zoom level

	}

	~MaprenderFrame()
	{
		
	}

	virtual void on_draw()
	{
		bool draw = true;
		int startRow = 1815;
		int startCol = 5296;
		for (int row = startRow; row <= 1817; row++)
		{
			for (int col = startCol; col <= 5298; col++)
			{
				if (draw)
				{
					GridId gridId = (GridId)NdsGridId_fromRowCol(13, row, col);
					int y = (row - startRow) * 128 + 64;
					int x = (col - startCol) * 128 - 64;
					drawGrid(gridId, x, y);
				}
				draw = !draw;
			}
		}

		pixfmt pixf(rbuf_window());
		renderer_base render(pixf);
		renderer_scanline scanline(render);
		scanline_type sl;
		rasterizer ras;
		agg::ellipse ell;
		ell.init(220, 128, 10, 20);

		scanline.color(agg::rgba(0,1,0));
		ras.reset();
		ras.add_path(ell);
		agg::render_scanlines(ras, sl, scanline);
	}
};


int agg_main(int argc, char* argv[])
{
	argc,argv;
	enum flip_y_e { flip_y = true };
	MaprenderFrame app(agg::pix_format_rgba32, flip_y);
	app.caption("AGG Example. Lion");

	if(app.init(256, 256, agg::window_resize))
	{
		return app.run();
	}
	return 1;
}
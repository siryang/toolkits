#include "mapr_stdafx.h"
#include "agg_frame.h"
#include "platform/agg_platform_support.h"

extern "C"
{
	#include "util/nds.h"
};

using namespace std;

class MaprenderFrame: public AggFrame
{
public:
	MaprenderFrame(agg::pix_format_e format, bool flip_y): AggFrame(format, flip_y)
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
		//vector<GridId> gridsInView;
		//gridsInView.push_back(100);
		//gridsInView.push_back(200);
		//gridsInView.push_back(300);
		//gridsInView.push_back(400);
		//addBuffer(-64, 64, L"heart.png");\

		//drawGrid(557467434, -64, 64);
		//drawGrid(557467434, 192, 64);
		//drawGrid(557467434, 64, 192);
		//drawGrid(557467434, -64, 320);
		//drawGrid(557467434, 192, 320);

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
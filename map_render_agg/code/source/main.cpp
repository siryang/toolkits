#include "mapr_stdafx.h"

using namespace agg;
using namespace std;

enum
{
	frame_width = 320,
	frame_height = 200
	//frame_width = 10,
	//frame_height = 10
};


// Writing the buffer to a .PPM file, assuming it has 
// RGB-structure, one byte per color component
//--------------------------------------------------
bool write_ppm(const unsigned char* buf, 
			   unsigned width, 
			   unsigned height, 
			   const char* file_name)
{
	FILE* fd = fopen(file_name, "wb");
	if(fd)
	{
		fprintf(fd, "P6 %d %d 255 ", width, height);
		fwrite(buf, 1, width * height * sizeof(pixfmt), fd);
		fclose(fd);
		return true;
	}
	return false;
}

// Draw a black frame around the rendering buffer, assuming it has 
// RGB-structure, one byte per color component
//--------------------------------------------------
void draw_black_frame(agg::rendering_buffer& rbuf)
{
	unsigned i;
	for(i = 0; i < rbuf.height(); ++i)
	{
		unsigned char* p = rbuf.row_ptr(i);
		*p++ = 0; *p++ = 0; *p++ = 0; *p++=0; // 3 black in header
		p += (rbuf.width() - 2) * sizeof(pixfmt);
		*p++ = 0; *p++ = 0; *p++ = 0; *p++=0; // 3 black in tail
	}
	memset(rbuf.row_ptr(0), 0, rbuf.width() * sizeof(pixfmt));
	memset(rbuf.row_ptr(rbuf.height() - 1), 0, rbuf.width() * sizeof(pixfmt));
}


class FreamWindow : public platform_support
{
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
public:
	FreamWindow(pix_format_e format, bool flip_y): platform_support(format, flip_y)
	{
		// In the first example we do the following:
		//--------------------
		// Allocate the buffer.
		// Clear the buffer, for now "manually"
		// Create the rendering buffer object
		// Do something simple, draw a diagonal line
		// Write the buffer to agg_test.ppm
		// Free memory

		unsigned char* buffer = new unsigned char[frame_width * frame_height * sizeof(pixfmt)];
		memset(buffer, 255, frame_width * frame_height * sizeof(pixfmt));

		agg::rendering_buffer rbuf(buffer,
			frame_width, 
			frame_height,
			frame_width * sizeof(pixfmt));

		unsigned i;
		pixfmt pixf(rbuf);
		for(i = 0; i < rbuf.height(); ++i)
		{
			pixf.copy_pixel(i, i, agg::rgba8(127, 200, 98));
		}

		draw_black_frame(rbuf);
		common::writePng(buffer, sizeof(pixfmt), frame_width, frame_height, "agg_test.png");

		delete [] buffer;
	}

	virtual void on_resize(int cx, int cy)
	{
		// clear background
		pixfmt pf(rbuf_window());
		renderer_base r(pf);
		r.clear(agg::rgba(1, 1, 1));
		
	}

	virtual void on_draw()
	{
		pixfmt pixelFormat(rbuf_window());
		renderer_base render(pixelFormat);
		render.clear(agg::rgba(1, 1, 1));
		
		size_t width, height;
		vector<char> pngBuffer;
		// load png
		common::readPng("heart.png", &width, &height, pngBuffer);
		// draw png
		agg::rgba8* bitmap = (agg::rgba8*)&pngBuffer[0];
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				pixelFormat.copy_pixel(x, y, bitmap[y * width + x]);
			}
		}
	};
};

int agg_main(int argc, char* argv[])
{
	enum flip_y_e { flip_y = true };
	FreamWindow app(pix_format_bgrABB, flip_y);
	app.caption("AGG Example. Lion");

	if(app.init(512, 400, agg::window_resize))
	{
		return app.run();
	}
	return 1;
}
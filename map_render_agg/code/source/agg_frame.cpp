#include "mapr_stdafx.h"
#include "agg_frame.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "agg_pixfmt_rgba.h"
#include "ctrl/agg_slider_ctrl.h"
#include "common_types.h"

using namespace agg;
using namespace std;
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

AggFrame::AggFrame( pix_format_e format, bool flip_y ) : platform_support(format, flip_y)
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

void AggFrame::on_draw()
{
	pixfmt pixelFormat(rbuf_window());
	renderer_base render(pixelFormat);
	render.clear(agg::rgba(1, 1, 1));
}

void AggFrame::on_resize( int cx, int cy )
{
	// clear background
	pixfmt pf(rbuf_window());
	renderer_base r(pf);
	r.clear(agg::rgba(1, 1, 1));
}

void AggFrame::addBuffer( int left, int top, const char* buffers, size_t width, size_t height )
{
	pixfmt pixelFormat(rbuf_window());
	int frameWidth = pixelFormat.width();
	int frameHegith = pixelFormat.height();

	Rect drawRegion;
	drawRegion.set(max(0, left), min(frameWidth, left + (int)width)
		, max(0, (top - (int)height)), min(frameHegith, top));
	
	agg::rgba8* bitmap = (agg::rgba8*)buffers + (top - drawRegion.bottom) * width + drawRegion.left - left;
	for (int y = drawRegion.bottom - 1; y >= drawRegion.top; y--)
	{
		agg::rgba8* row = bitmap;
		for (int x = drawRegion.left ; x < drawRegion.right; x++)
		{
			pixelFormat.copy_pixel(x, y, *row++);
		}
		bitmap += width;
	}
}

void AggFrame::addBuffer( int left, int top, const wchar_t* pngName )
{
	vector<char> pngBuffer;
	size_t width, height;
	common::readPng("heart.png", &width, &height, pngBuffer);
	addBuffer(left, top, &pngBuffer[0], width, height);
}

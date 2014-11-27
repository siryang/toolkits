// cairo_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include "stdio.h"
#include "cairo/cairo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("cairo version: %s\n", cairo_version_string());

	cairo_surface_t* surface;
	cairo_t* cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 120, 120);
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, 120, 120);
	cairo_move_to(cr, 120, 0);
	cairo_line_to(cr, 0, 120);
	cairo_set_line_width(cr, 24);
	cairo_stroke(cr);

	cairo_rectangle(cr, 0, 0, 60, 60);
	cairo_set_source_rgba(cr, 1, 0, 0, 0.80);
	cairo_fill(cr);

	cairo_rectangle(cr, 0, 60, 60, 60);
	cairo_set_source_rgba(cr, 0, 1, 0, 0.60);
	cairo_fill(cr);

	cairo_rectangle(cr, 60, 0, 60, 60);
	cairo_set_source_rgba(cr, 0, 0, 1, 0.40);
	cairo_fill(cr);

	cairo_destroy(cr);

	cairo_status_t status = cairo_surface_write_to_png(surface, "test.png");
	switch (status)
	{
	case CAIRO_STATUS_SUCCESS:
		printf("CAIRO_STATUS_SUCCESS\n");
		break;
	default:
		printf("ERROR STATUS: %d\n", status);
		break;
	}

	cairo_surface_destroy(surface);
	system("pause");
	return 0;
}


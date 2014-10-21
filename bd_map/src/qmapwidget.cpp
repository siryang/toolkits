#include "stdafx.h"
#include "qmapwidget.h"
#include <vector>

#define ROUTE_PLAN_START_ORIENTATION_RADIUS	20

#define METER_PER_LAT_UNIT 1.11f					 /* How many meters a latitude unit (10^-5 degree) equate to. */
#define LAT_UNIT_PER_METER (1/METER_PER_LAT_UNIT)      

#define ZOOM_COUNT 18

const int32 GRID_DATA_LEVEL[ZOOM_COUNT] = {
	1, 1, 1,    //-3 - -1
	1, 1, 1, 1, // 0 - 3
	3, 3,       // 4 - 5
	5, 5,       // 6 - 7
	7, 7,       // 8 - 9
	9, 9,       // 10 - 11
	11, 11, 11  // 12 - 14
};

#define MIN_SCALE_METER_INT 1
#define MAX_SCALE_METER_INT 1000000

#define PIXELS_PER_UNIT 100

const int32 METER_PER_UNIT[ZOOM_COUNT] = {
	MIN_SCALE_METER_INT,		// 2, at zoom level -3
	MIN_SCALE_METER_INT * 2,	// 5
	MIN_SCALE_METER_INT * 5,	// 10

	MIN_SCALE_METER_INT * 10,	// 20, at zoom level 0
	MIN_SCALE_METER_INT * 25,   // 50
	MIN_SCALE_METER_INT * 50,   // 125
	MIN_SCALE_METER_INT * 100,  // 250

	MIN_SCALE_METER_INT * 400,  // 625
	MIN_SCALE_METER_INT * 1250,  // 1,250

	MIN_SCALE_METER_INT * 2500, // 2,500
	MIN_SCALE_METER_INT * 5000, // 

	MIN_SCALE_METER_INT * 12500, // 12,500
	MIN_SCALE_METER_INT * 25000,// 
	MIN_SCALE_METER_INT * 50000,// 

	MIN_SCALE_METER_INT * 125000, // 125,000
	MIN_SCALE_METER_INT * 250000,// 

	MIN_SCALE_METER_INT * 625000,// 625,000
	MAX_SCALE_METER_INT		     // 1000k
};

uint8* g_buffer = NULL;

QMapWidget::QMapWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	setWindowState(Qt::WindowMaximized);
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);
}

QMapWidget::~QMapWidget()
{
}

void QMapWidget::timerEvent( QTimerEvent * event )
{
	CM_ASSERT(0);
}

void QMapWidget::paintEvent( QPaintEvent *event )
{
	CM_ASSERT(0);
}

void QMapWidget::resizeEvent( QResizeEvent * event )
{

	CM_ASSERT(0);
}

void QMapWidget::update()
{
	CM_ASSERT(0);

}

void QMapWidget::paintGL()
{
	CM_ASSERT(0);

}

#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H
#include "common_types.h"
#include <QGLWidget>

class QMapWidget : public QGLWidget
{
	Q_OBJECT
public:
	QMapWidget(QWidget *parent = 0);
	virtual ~QMapWidget();

public slots:
	void update();

public:
	void setCenter(const Point* center);

protected:
	/////////////////////event method override///////////////////////////////////////
	virtual void timerEvent(QTimerEvent * event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent * event);
	//virtual void resizeGL(int width, int height);
	//virtual void initializeGL();
	virtual void paintGL();
	//virtual void mousePressEvent(QMouseEvent *event);
	//virtual void mouseReleaseEvent(QMouseEvent *event);
	//virtual void mouseMoveEvent(QMouseEvent *event);
	//virtual void wheelEvent (QWheelEvent * event);
	//virtual void showEvent(QShowEvent* event);

	//////////////////////Map Drawing Members/////////////////////////////
private:
	void drawOnSurface();
	void viewToWorld(int w, int h, Rect *roundBox);
	void QMapWidget::resize(int w, int h);
};

#endif // QMAPWIDGET_H

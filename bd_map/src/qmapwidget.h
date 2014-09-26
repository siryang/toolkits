#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QGLWidget>
#include "tile_service.h"

class QMapWidget : public QGLWidget
{
	Q_OBJECT
public:
	QMapWidget(QWidget *parent = 0);
	virtual ~QMapWidget();

public slots:
	void update();

public:
	enum MapState {
		MapState_map,				// 拖动地图
		MapState_route,				// 拖动route起点和终点
		MapState_associateGroup,
		MapState_hintAngle,
		MapState_hintPos,
		MapState_routePoint,
	};

	enum NetState
	{
		NetState_online,
		NetState_offline,
		NetState_auto,
	};

	void setCenter(const Point* center);

protected:
	/////////////////////event method override///////////////////////////////////////
	virtual void timerEvent(QTimerEvent * event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void resizeGL(int width, int height);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent (QWheelEvent * event);
	virtual void showEvent(QShowEvent* event);

	//////////////////////Map Drawing Members/////////////////////////////
	MapState m_mapState;			// mapwidget的工作模式

private:
	void drawOnSurface();
	void viewToWorld(int w, int h, Rect *roundBox);
	void QMapWidget::resize(int w, int h);

private:

	GDI* m_gdi;
	Handle m_surf;
	int m_zoomLevel;
	Point m_worldCenter;

	bool m_mouseDown;					
	QPoint m_mouseDownPos;				// 鼠标按下坐标
	QPoint m_oldWorldCenter;			// 鼠标按下时的世界中心
	QPoint m_currentPos;				// 拖动鼠标时的当前坐标
	Point m_destCandidate;				// 鼠标释放时世界坐标
	bool m_needRefreshScreen;
	int m_refreshTimerId;
	//////////////////////////////////////////////////////////////////////////
	bool m_drawTmcGroup;				// 显示group点
	bool m_drawTmcHint;					// 显示hint点
	bool m_drawAssociation;				// 绘制group间的关联箭头
	bool m_ctrlPressDown;				// ctrl键是否已经按下

	// routePlan起点终点拖动相关变量
	int m_dragRoutePointIndex;
	Point m_oldDestPos;

	int m_groupId;
	NetState m_netState;				// 绘图功能的联网状态
};

#endif // QMAPWIDGET_H

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
		MapState_map,				// �϶���ͼ
		MapState_route,				// �϶�route�����յ�
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
	MapState m_mapState;			// mapwidget�Ĺ���ģʽ

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
	QPoint m_mouseDownPos;				// ��갴������
	QPoint m_oldWorldCenter;			// ��갴��ʱ����������
	QPoint m_currentPos;				// �϶����ʱ�ĵ�ǰ����
	Point m_destCandidate;				// ����ͷ�ʱ��������
	bool m_needRefreshScreen;
	int m_refreshTimerId;
	//////////////////////////////////////////////////////////////////////////
	bool m_drawTmcGroup;				// ��ʾgroup��
	bool m_drawTmcHint;					// ��ʾhint��
	bool m_drawAssociation;				// ����group��Ĺ�����ͷ
	bool m_ctrlPressDown;				// ctrl���Ƿ��Ѿ�����

	// routePlan����յ��϶���ر���
	int m_dragRoutePointIndex;
	Point m_oldDestPos;

	int m_groupId;
	NetState m_netState;				// ��ͼ���ܵ�����״̬
};

#endif // QMAPWIDGET_H

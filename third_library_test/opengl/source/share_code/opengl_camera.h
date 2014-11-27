#pragma once
#include "ass_type.h"

class CameraInfo
{
public:
	Rect m_viewPort;
	Point3F m_eye, m_center, m_up;

	float m_near, m_far;
	RectF m_nearView;
};

class Camera
	: protected CameraInfo
{
public:
	Camera()
	{
		m_eye.x = m_eye.y = m_eye.z = m_center.x = m_center.y = m_center.z = m_up.x = m_up.y = m_up.z = 0;
		m_near = m_far = 0;

		m_viewPort.set(0, 800, 0, 600);
		m_nearView.set(-80, 80, -60, 60);
		m_up.y = 1;
		m_center.z = -1;
		m_near = 100;
		m_far = 5000;
	}

	void changeViewPort(int x, int y, int width, int height)
	{
		m_viewPort.set(x, x + width, y, y + height);
		glViewport(m_viewPort.left, m_viewPort.top, m_viewPort.width(), m_viewPort.height());\

		float aspectRatio = m_viewPort.height() / (float)m_viewPort.width();

		m_nearView.set(-100, 100, 100 * aspectRatio, -100 * aspectRatio);
	}

	void flush();
	void zoomIn();
	void zoomOut();
	void step(Dir3D dir);
};


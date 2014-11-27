#include "opengl_stdafx.h"
#include "opengl_camera.h"
#include <iostream>

using namespace std;

//void Camera::initCamera()
//{
//	x = y = z = 0.0f;
//	centerX = centerY = centerZ = 0.0f;
//	upX = upY = upZ = 0.0f;
//	zNear = zFar = 0.0f;
//
//	x = 0.0f;
//	//z = 5.0f;
//	centerZ = -5.0f;
//	upY = 1.0f;
//
//	zNear = 0.0f;
//	zFar = 180.0f;
//
//	enableCamera();
//}
//
//void Camera::enableCamera()
//{
//	glMatrixMode(GL_PROJECTION);    //���þ���ģʽΪͶӰ�任����
//	glLoadIdentity();                //��Ϊ��λ����
//	gluPerspective(60, (GLfloat)viewWidth / viewHeight, zNear, zFar);    //����ͶӰ����
//
//	glMatrixMode(GL_MODELVIEW);							//���þ���ģʽΪ��ͼ����(ģ��)
//	glLoadIdentity();                                    // //��Ϊ��λ����
//	gluLookAt(x, y, z
//		, centerX, centerY, centerZ
//		, upX, upY, upZ); 
//}
//
//void Camera::moveEyes( float _x, float _y, float _z )
//{
//	x += _x;
//	y += _y;
//	z += _z;
//	cout << "camera : (" << x << "," << y << "," << z << ")" << endl;
//}
//
//void Camera::moveCenter( float _x, float _y, float _z )
//{
//	centerX += _x;
//	centerY += _y;
//	centerZ += _z;
//	cout << "center : (" << centerX << "," << centerY << "," << centerZ << ")" << endl;
//}

void Camera::flush()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(m_nearView.left, m_nearView.right, m_nearView.bottom, m_nearView.top, m_near, m_far);

	gluLookAt(m_eye.x, m_eye.y, m_eye.z
		, m_center.x, m_center.y, m_center.z
		, m_up.x, m_up.y, m_up.z); 
	glutPostRedisplay();
}

void Camera::zoomIn()
{
	if (m_near < m_far)
		m_near += 10;
	flush();
}

void Camera::zoomOut()
{
	if (m_near > 1)
		m_near -= 10;
	flush();
}

void Camera::step( Dir3D dir )
{
	
}



#include "opengl_stdafx.h"
#include "martrix_transform.h"

void MatrixTransform::modelViewTransTest()
{
	glMatrixMode(GL_MODELVIEW);	// ����ģ�;���
	glPushMatrix();
	glLoadIdentity(); // ת��Ϊ��λ����

	glRotatef(25.0, 0, 0, 1); // ����ת(0, 0, 0)--(0, 0, 1)����ʹ�����ֹ�����ת25��
	glPushMatrix(); // ��¼��ǰλ��(0, 0)

	glTranslated(-50.0f, 0.0f , 0); // ��x���ƶ�ģ�;���, ������(-50, 0)
	glColor3f(1.0f, 0.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glTranslated(100.0f, 0.0f , 0); // ��x���ƶ�ģ�;���������(50, 0)
	glColor3f(1.0f, 1.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glPopMatrix(); // ����֮ǰ��¼��λ��, (0, 0)
	glColor3f(0.0f, 1.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glPopMatrix();
}

void MatrixTransform::projectionTransTest()
{

}

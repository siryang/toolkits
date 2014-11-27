#include "opengl_stdafx.h"
#include "martrix_transform.h"

void MatrixTransform::modelViewTransTest()
{
	glMatrixMode(GL_MODELVIEW);	// 载入模型矩阵
	glPushMatrix();
	glLoadIdentity(); // 转换为单位矩阵

	glRotatef(25.0, 0, 0, 1); // 沿旋转(0, 0, 0)--(0, 0, 1)轴旋使用右手规则旋转25度
	glPushMatrix(); // 记录当前位置(0, 0)

	glTranslated(-50.0f, 0.0f , 0); // 向x轴移动模型矩阵, 中心在(-50, 0)
	glColor3f(1.0f, 0.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glTranslated(100.0f, 0.0f , 0); // 向x轴移动模型矩阵，中心在(50, 0)
	glColor3f(1.0f, 1.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glPopMatrix(); // 返回之前记录的位置, (0, 0)
	glColor3f(0.0f, 1.0f, 0.0f);
	glRecti(-20, -20, 20, 20);

	glPopMatrix();
}

void MatrixTransform::projectionTransTest()
{

}

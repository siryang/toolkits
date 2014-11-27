#include "opengl_stdafx.h"
#include "gl_render.h"

RenderSystem* RenderSystem::m_instance = NULL;

void RenderSystem::draw()
{

}

void RenderSystem::drawstr( GLuint x, GLuint y, char* format, ... )
{
	va_list args;
	char buffer[255], *s;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glRasterPos2i(x, y);
	for (s = buffer; *s; s++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *s);
}

void drawNet()
{
	int LinesX = 30;
	int LinexY = 30;
	int LinesZ = 30;
	GLfloat size = 25.0;

	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
			0.0,
			size / 2.0);
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
			0.0,
			size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
		glVertex3f(	size / -2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
	}
	glEnd();
}

void RenderSystem::drawCube()
{
	static const float vertex_list[][3] = 
	{ 
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		-0.5f, 0.5f, -0.5f, 
		0.5f, 0.5f, -0.5f, 
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f, 
		-0.5f, 0.5f, 0.5f, 
		0.5f, 0.5f, 0.5f, 
	}; 

	// 将要使用的顶点的序号保存到一个数组里面 

	static const GLint index_list[][2] = 
	{ 
		{0, 1},    
		{2, 3},    
		{4, 5},    
		{6, 7},    
		{0, 2},    
		{1, 3},    
		{4, 6},    
		{5, 7},
		{0, 4},
		{1, 5},
		{7, 3},
		{2, 6}
	}; 

	// 绘制立方体

	int i,j;

	glBegin(GL_LINES); 
	for(i=0; i<12; ++i) // 12 条线段

	{
		for(j=0; j<2; ++j) // 每条线段 2个顶点

		{
			glVertex3fv(vertex_list[index_list[i][j]]);     
		}
	}
	glEnd();
}

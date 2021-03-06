#include "opengl_stdafx.h"
#include "martrix_transform.h"

void redisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//MatrixTransform::modelViewTransTest();
	// 修改投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	// 设置平头椎体
	//glOrtho (-100.0, 100.0, -100.0, 100.0, 3.5, 20.0); // 正交
	glFrustum(-2.0, 2.0, -2.0, 2.0, 3.5, 20.0); // 透视


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // 设置摄像机

	//glTranslated(0, 0, -1);
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	//glRecti(-1, -1, 1, 1);
	glScalef(2.0, 2.0, 1.0);
	glutWireCube (1.0);

	glPopMatrix();

	glFlush();
}

///////////////////////////////////////////////////////////
// Set up the rendering state
void SetupRC(void)
{
	// Set clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

///////////////////////////////////////////////////////////
// Called by GLUT library when the window has chanaged size
void changeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
}

void mouseMove(int button, int state, int x, int y)
{
	cout << "button" << button << " state" << state << " x" << x << " y" << y << endl;
}

void mouseWheel(int button, int dir, int x, int y)
{
	cout << "button" << button << " dir" << dir << " x" << x << " y" << y << endl;
}

void keyboardEnter(unsigned char key, int x, int y)
{
	cout << key << x << y<< endl;
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char** argv)
{
	GLuint mainWindow;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	mainWindow = glutCreateWindow("GLCube");
	glutDisplayFunc(redisplay);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseMove);
	glutMouseWheelFunc(mouseWheel);
	glutKeyboardFunc(keyboardEnter);
	SetupRC();
	glutMainLoop();
	return 0;
}


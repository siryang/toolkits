#include "opengl_stdafx.h"

Camera camera;

void redisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//MatrixTransform::modelViewTransTest();


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glClear (GL_COLOR_BUFFER_BIT);
	//glScalef(2.0, 2.0, 2.0);

	glPushMatrix();
	glTranslated(0, 0, -200);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutWireCube (50.0);


	glPopMatrix();
	glPushMatrix();
	glTranslated(100, 0, -400);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutWireCube (50.0);

	glPopMatrix();
	glPushMatrix();
	glTranslated(-100, 0, -400);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutWireCube (50.0);
	glPopMatrix();


	glPopMatrix();

	glFlush();
}

///////////////////////////////////////////////////////////
// Set up the rendering state
void SetupRC(void)
{
	// Set clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	camera.flush();
}

///////////////////////////////////////////////////////////
// Called by GLUT library when the window has chanaged size
void changeSize(GLsizei w, GLsizei h)
{
	camera.changeViewPort(0, 0, w, h);
	camera.flush();
}

void mouseMove(int button, int state, int x, int y)
{
	cout << "button" << button << " state" << state << " x" << x << " y" << y << endl;
}

#define MOUSE_WHEEL_UP 1
#define MOUSE_WHEEL_DOWN -1
void mouseWheel(int button, int dir, int x, int y)
{
	cout << "button" << button << " dir" << dir << " x" << x << " y" << y << endl;
	dir == MOUSE_WHEEL_UP? camera.zoomIn(): camera.zoomOut();
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


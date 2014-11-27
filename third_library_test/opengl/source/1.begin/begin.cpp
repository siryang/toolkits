#include "opengl_stdafx.h"
#include "draw_cube.h"

void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	// Set current drawing color to red
	// R G B
	glColor3f(1.0f, 0.0f, 0.0f);

	//// Draw a filled rectangle with current color
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 20, 0);
	glVertex3f(20, 20, 0);
	glEnd();

	//glRectf(-25.0f, 25.0f, 25.0f, -25.0f);

	glColor3f(0.0f, 1.0f, 1.0f);
	// draw cube
	drawCube();

	// Flush drawing commands
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
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Establish clipping volume (left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho (-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio,
		1.0, -1.0);
	else
		glOrtho (-100.0 * aspectRatio, 100.0 * aspectRatio,
		-100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseMove(int button, int state, int x, int y)
{
	;
}

void keyboardEnter(unsigned char key, int x, int y)
{
	;
}

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("GLRect");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(mouseMove);
	SetupRC();
	glutMainLoop();
	return 0;
}


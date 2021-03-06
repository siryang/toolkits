#include "../share_code/opengl_stdafx.h"
#include "image_indexed.h"

static size_t s_width, s_height;

bool write_ppm(const unsigned char* buf, 
			   unsigned width, 
			   unsigned height, 
			   const char* file_name)
{
	FILE* fd = fopen(file_name, "wb");
	if(fd)
	{
		fprintf(fd, "P6 %d %d 255 ", width, height);
		fwrite(buf, 1, width * height * 4, fd);
		fclose(fd);
		return true;
	}
	return false;
}

void redisplay()
{
	//GLuint fbo, rboColor, rboDepth;

	//// Color renderbuffer.
	//glGenRenderbuffers(1,&rboColor);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboColor);
	//// Set storage for currently bound renderbuffer.
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, s_width, s_height);

	//// Depth renderbuffer
	//glGenRenderbuffers(1,&rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, s_width, s_height);

	//// Framebuffer
	//glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColor);
	//// Set renderbuffers for currently bound framebuffer
	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//// Set to write to the framebuffer.
	//glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glRectd(-30.0, -30.0, 10.0, 10.0);

	glColor3f(1.0f, 1.0f, 0.0f);
	glRectd(0.0, 0.0, 20.0, 20.0);

	// Tell glReadPixels where to read from.
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	GLubyte *buffer = (GLubyte *) malloc(s_width * s_height* 4);
	glReadPixels(0, 0, s_width, s_height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)buffer);
	savePng(L"png.png", buffer, s_width, s_height, s_width);
	write_ppm(buffer, s_width, s_height, "test.ppm");

	free(buffer);
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
	s_width = w;
	s_height = h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Establish clipping volume (left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;
	// 修改投影矩阵
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
	cout << "button" << button << " state" << state << " x" << x << " y" << y << endl;
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
	glutKeyboardFunc(keyboardEnter);
	SetupRC();
	glutMainLoop();
	return 0;
}


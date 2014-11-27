#include "opengl_stdafx.h"

GLuint g_texture = 0;
GLfloat xrot =0;
GLfloat yrot =0;
GLfloat zrot =0;                                    // Keep Going

Camera g_camera;

//����һ��������
int DrawCube(void)
{
	glBindTexture(GL_TEXTURE_2D, g_texture);        //ʹ����ͼ����
	glPushMatrix();        //ѹ��任����

	glRotatef(xrot,1.0f,0.0f,0.0f);            //��ת����������x����ת��
	glRotatef(yrot,0.0f,1.0f,0.0f);            //��ת����������y����ת��
	glRotatef(zrot,0.0f,0.0f,1.0f);            //��z����ת������zrot�ǽǶ��ƵĶ�����

	glBegin(GL_QUADS);  //�����ı��δ�����ģʽ����

	// ����ǰ�棬���￪ʼȷ���������꣬Ȼ����ȷ�����λ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

	// ���ƺ���
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

	// ����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

	//����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	// ����
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

	// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();

	glPopMatrix(); //�����任����

	return 1;

}

void drawHorizontal()
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
		// x
		glVertex3f(-2.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);

		// y
		glVertex3f(0.0f, -2.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, -2.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // �����ɫ���ݺ�������ݣ�������
	//glClearColor(0.2, 0.2, 0.2, 0.0);

	g_camera.enableCamera();

	glMatrixMode(GL_MODELVIEW);							//���þ���ģʽΪ��ͼ����(ģ��)
	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, -5.0f);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -5.0f);
	DrawCube();
	glPopMatrix();

	drawHorizontal();

	glutSwapBuffers();            //��������������ʾͼ��
}

//����һ��.bmp��ʽ����ͼ����
int LoadGLTextures(GLuint& unTexture, const char* chFileName)                
{
	int width, height;
	GLubyte* texture = glmReadPPM(chFileName, &width, &height);
	glGenTextures(1, &unTexture);                    // ����һ������unTexture

	glBindTexture(GL_TEXTURE_2D, unTexture);        //������Ȼ��Ը������������������

	//�����������Ϣ��
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		//�����˲�Ϊ�����˲�
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);		//�����˲�

	if (texture)                //�ͷ���Դ
	{
		free(texture);
	}

	return 1;
}


//��ʼ��
void init (void) 
{
	g_camera.initCamera();
	glClearColor (0.0, 0.0, 0.0, 0.0);            //������ɫ��Ϊ��ɫ����Ҳ����Ϊ�Ǳ�����ɫ��

	glCullFace(GL_BACK);                        //����ü�(���治�ɼ�)
	glEnable(GL_CULL_FACE);                        //���òü�
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	LoadGLTextures(g_texture, "data/checker.ppm");            //����������ͼ
}

//�����ڴ�С�ı�ʱ��������������
void reshape(GLsizei w,GLsizei h)
{
	//����С˵��һ�£�����ģʽ�ǲ�ͬ�ģ����Ǹ�����һ������ͶӰ���
	g_camera.setViewPort(w, h);
	//glViewport(-w / 2, -h / 2, w / 2, h / 2);        //�����ӿ�
	glViewport(0, 0, w, h);        //�����ӿ�
	//glEnable(GL_DEPTH_TEST);
}

//���ú���������ѭ������ʱ�ͻ�����������
void MyIdle(void)
{
	Sleep(10);
	xrot+=0.3f;        //������ת�ĽǶȡ�
	yrot+=0.2f;
	zrot+=0.4f;                                    
	if (xrot > 360.0f)
		xrot -= 360.0f;
	glutPostRedisplay();
}

// x, y �ǵ��������ͼ�����е�λ��
void keyboardPressed(unsigned char key, int x, int y)
{
	cout << key <<  "(" << x << "," << y << ")" << endl;
	if (key == '+')
	{
		g_camera.moveEyes(0,0, -0.05f);
	}
	else if (key == '-')
	{
		g_camera.moveEyes(0,0, 1.05f);
	}
	else if (key == 'r')
		g_camera.initCamera();
}

void mousePressed(int button, int state, int x, int y)
{
	static Point downLocation;
	//cout << "key:" << button << ", state:" << state << "(" << x << "," << y << ")" << endl;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			downLocation.x = x;
			downLocation.y = y;
		}
		else if (state == GLUT_UP)
		{
			g_camera.moveEyes((downLocation.x - x) / (float)g_camera.getWindowWidth(), (downLocation.y - y) / (float)g_camera.getWindowHeight(), 0.0f);
			g_camera.moveCenter((downLocation.x - x) / (float)g_camera.getWindowWidth(), (downLocation.y - y) / (float)g_camera.getWindowHeight(), 0.0f);
			glutPostRedisplay();
		}
	}
}

void motionFunc(int x, int y)
{

}

void passiveMotionFunc(int x, int y)
{

}

void specialFunc(int button, int x, int y)
{
	cout << "key:" << button << "(" << x << "," << y << ")" << endl;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);        //Opnegl��ʼ��
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA); //������ʾģʽΪ˫���壬RGEBA

	glutInitWindowSize (800, 600);            //���ڴ�С
	glutInitWindowPosition (100, 100);        //����λ��
	glutCreateWindow ("hello");                //����һ������Ϊhello�Ĵ���
	glutDisplayFunc(display);                //���ڴ�С�ı�ʱ�Ļص�
	glutReshapeFunc(reshape);                //����ͼ��ʱ�Ļص�
	glutIdleFunc(MyIdle);
	glutKeyboardFunc(keyboardPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutSpecialFunc(specialFunc);
	
	init ();                                //��ʼ����Դ,����һ��Ҫ�ڴ��������Ժ󣬲�Ȼ����Ч��
	glutMainLoop();                            //��ѭ����
	return 0;  
}
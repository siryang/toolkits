#include "opengl_stdafx.h"

GLuint g_texture = 0;
GLfloat xrot =0;
GLfloat yrot =0;
GLfloat zrot =0;                                    // Keep Going

Camera g_camera;

//绘制一个立方体
int DrawCube(void)
{
	glBindTexture(GL_TEXTURE_2D, g_texture);        //使用贴图纹理
	glPushMatrix();        //压入变换矩阵

	glRotatef(xrot,1.0f,0.0f,0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(yrot,0.0f,1.0f,0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(zrot,0.0f,0.0f,1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // 清楚颜色数据和深度数据（清屏）
	//glClearColor(0.2, 0.2, 0.2, 0.0);

	g_camera.enableCamera();

	glMatrixMode(GL_MODELVIEW);							//设置矩阵模式为视图矩阵(模型)
	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, -5.0f);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -5.0f);
	DrawCube();
	glPopMatrix();

	drawHorizontal();

	glutSwapBuffers();            //交换缓冲区。显示图形
}

//载入一个.bmp格式的贴图纹理
int LoadGLTextures(GLuint& unTexture, const char* chFileName)                
{
	int width, height;
	GLubyte* texture = glmReadPPM(chFileName, &width, &height);
	glGenTextures(1, &unTexture);                    // 创建一个纹理，unTexture

	glBindTexture(GL_TEXTURE_2D, unTexture);        //绑定纹理，然后对该纹理区添加纹理数据

	//设置纹理的信息，
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		//设置滤波为线性滤波
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);		//线性滤波

	if (texture)                //释放资源
	{
		free(texture);
	}

	return 1;
}


//初始化
void init (void) 
{
	g_camera.initCamera();
	glClearColor (0.0, 0.0, 0.0, 0.0);            //清理颜色，为黑色，（也可认为是背景颜色）

	glCullFace(GL_BACK);                        //背面裁剪(背面不可见)
	glEnable(GL_CULL_FACE);                        //启用裁剪
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	LoadGLTextures(g_texture, "data/checker.ppm");            //载入纹理贴图
}

//当窗口大小改变时，会调用这个函数
void reshape(GLsizei w,GLsizei h)
{
	//这里小说明一下：矩阵模式是不同的，他们各自有一个矩阵。投影相关
	g_camera.setViewPort(w, h);
	//glViewport(-w / 2, -h / 2, w / 2, h / 2);        //设置视口
	glViewport(0, 0, w, h);        //设置视口
	//glEnable(GL_DEPTH_TEST);
}

//闲置函数，当主循环空闲时就会调用这个函数
void MyIdle(void)
{
	Sleep(10);
	xrot+=0.3f;        //增加旋转的角度。
	yrot+=0.2f;
	zrot+=0.4f;                                    
	if (xrot > 360.0f)
		xrot -= 360.0f;
	glutPostRedisplay();
}

// x, y 是点击点在视图坐标中的位置
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
	glutInit(&argc, argv);        //Opnegl初始化
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA); //设置显示模式为双缓冲，RGEBA

	glutInitWindowSize (800, 600);            //窗口大小
	glutInitWindowPosition (100, 100);        //窗口位置
	glutCreateWindow ("hello");                //创建一个标题为hello的窗口
	glutDisplayFunc(display);                //窗口大小改变时的回调
	glutReshapeFunc(reshape);                //绘制图形时的回调
	glutIdleFunc(MyIdle);
	glutKeyboardFunc(keyboardPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutSpecialFunc(specialFunc);
	
	init ();                                //初始化资源,这里一定要在创建窗口以后，不然会无效。
	glutMainLoop();                            //主循环。
	return 0;  
}
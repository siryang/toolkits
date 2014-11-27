#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include "color_picker.h"

color_picker::color_picker(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	emit init();
}

color_picker::~color_picker()
{

}

void color_picker::mousePressEvent(QMouseEvent *event)
{
	mouseMoveEvent(event);
}

void color_picker::mouseMoveEvent(QMouseEvent *event)
{
	event;
	char buff[128];
	HDC hDC = ::GetDC(NULL); 
	tagPOINT pt;
	GetCursorPos(&pt);	//�õ���ǰ�������λ��
	COLORREF clr = ::GetPixel(hDC, pt.x, pt.y); //��ȡ��ǰ��������ֵs
	sprintf (buff, "%x", clr);
	ui.lineEdit->setText(buff);
	::ReleaseDC(NULL, hDC); //�ͷ���ĻDC
}

void color_picker::inited()
{

}

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
	GetCursorPos(&pt);	//得到当前鼠标所在位置
	COLORREF clr = ::GetPixel(hDC, pt.x, pt.y); //获取当前鼠标点像素值s
	sprintf (buff, "%x", clr);
	ui.lineEdit->setText(buff);
	::ReleaseDC(NULL, hDC); //释放屏幕DC
}

void color_picker::inited()
{

}

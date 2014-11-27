#include "color_picker.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	color_picker w;
	w.show();
	return a.exec();
}

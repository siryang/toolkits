#include "stdafx.h"
#include "bd_map.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BDMap w;
	w.show();
	return a.exec();
}

#include "stdafx.h"
#include "bd_map.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tile_server_demo w;
	w.show();
	return a.exec();
}

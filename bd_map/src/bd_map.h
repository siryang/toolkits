#ifndef TILE_SERVER_DEMO_H
#define TILE_SERVER_DEMO_H

#include <QtGui/QMainWindow>
#include "ui_bd_map.h"

class BDMap : public QMainWindow
{
	Q_OBJECT

public:
	BDMap(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BDMap();

private:
	Ui::BDMapClass ui;
};

#endif // TILE_SERVER_DEMO_H

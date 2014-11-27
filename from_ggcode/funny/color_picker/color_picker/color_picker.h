#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <QtGui/QMainWindow>
#include "ui_color_picker.h"

class color_picker : public QMainWindow
{
	Q_OBJECT

public:
	color_picker(QWidget *parent = 0, Qt::WFlags flags = 0);
	~color_picker();

signals:
	void init();
private slots:
	void inited();

private:
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void mousePressEvent(QMouseEvent *event);
	Ui::color_pickerClass ui;
	
};

#endif // COLOR_PICKER_H

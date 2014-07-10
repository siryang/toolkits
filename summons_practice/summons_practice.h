#ifndef SUMMONS_PRACTICE_H
#define SUMMONS_PRACTICE_H

#include <QtGui/QMainWindow>
#include "ui_summons_practice.h"

class summons_practice : public QMainWindow
{
	Q_OBJECT

public:
	summons_practice(QWidget *parent = 0, Qt::WFlags flags = 0);
	~summons_practice();

private:
	Ui::summons_practiceClass ui;
};

#endif // SUMMONS_PRACTICE_H

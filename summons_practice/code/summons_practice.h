#ifndef SUMMONS_PRACTICE_H
#define SUMMONS_PRACTICE_H

#include <QKeyEvent> 
#include <QtGui/QMainWindow>
#include "ui_summons_practice.h"

class summons_practice : public QMainWindow
{
	Q_OBJECT

public:
	summons_practice(QWidget *parent = 0, Qt::WFlags flags = 0);
	~summons_practice();

	void keyPressEvent(QKeyEvent *event);
private:
	void update();

	Ui::summons_practiceClass ui;
	size_t m_num;
	size_t m_sum;
};

#endif // SUMMONS_PRACTICE_H

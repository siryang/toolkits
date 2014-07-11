#ifndef SUMMONS_PRACTICE_H
#define SUMMONS_PRACTICE_H

#include <QKeyEvent> 
#include <QtGui/QMainWindow>
#include <time.h>
#include "ui_summons_practice.h"

class summons_practice : public QMainWindow
{
	Q_OBJECT

public:
	summons_practice(QWidget *parent = 0, Qt::WFlags flags = 0);
	~summons_practice();

	void keyPressEvent(QKeyEvent *event);
public slots:
	void onCustomIdChanged();

private:
	void update();
	void redo();
	void addNew(float newNum);
	void cancelAll();
	void complete();

	Ui::summons_practiceClass ui;
	QVector<float> m_inputted;
	float m_sum;
	clock_t m_startTime;
};

#endif // SUMMONS_PRACTICE_H

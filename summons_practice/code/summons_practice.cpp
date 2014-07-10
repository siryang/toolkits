#include "summons_practice.h"

summons_practice::summons_practice(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_sum = 100;
	m_num = 0;

	update();
}


summons_practice::~summons_practice()
{

}

void summons_practice::keyPressEvent(QKeyEvent  *event) 
{
	QString customNum = ui.CustomId->text();
	size_t newSum = ui.CustomSum->text().toInt();
	if (m_sum == newSum)
	{
		m_num++;
	}
	else
	{
		m_sum = newSum;
		m_num = 0;
	}
	update();
}

void summons_practice::update()
{
	ui.CustomId->setText("");
	ui.CustomSum->setText(QString::number(m_sum));
	ui.CurrentNum->setText(QString::number(m_num));
}

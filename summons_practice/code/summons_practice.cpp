#include "summons_practice.h"
#include "time.h"
#include <QMessageBox>

summons_practice::summons_practice(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_sum = 0.0f;
	update();
}


summons_practice::~summons_practice()
{

}

void summons_practice::onCustomIdChanged()
{
	// .88 .99
	QString newNum = ui.currentNum->text();
	if (m_inputted.empty())
	{
		m_startTime = clock();
	}

	if (newNum == tr(".88"))
	{
		redo();
	}
	else if (newNum == tr(".99"))
	{
		complete();
	}
	else
	{
		int dotPos = newNum.indexOf('.');
		int length = newNum.length();
		if (dotPos != -1 && length - dotPos == 4)
		{
			QChar* numStr = newNum.data();
			numStr[length - 2] = numStr[length - 1];
			newNum.resize(newNum.length() - 1);
		}
		ui.currentNum->setText(newNum);
	}
	update();
}

void summons_practice::keyPressEvent(QKeyEvent  *event) 
{
	if (event == QKeySequence::InsertParagraphSeparator)
	{
		// press enter
		addNew(ui.currentNum->text().toFloat());
	}
	else if (event == QKeySequence::MoveToPreviousLine)
	{
		// press up
		cancelAll();
	}
	update();
}

void summons_practice::update()
{
	ui.totalInputted->setText(QString::number(m_inputted.size()));
	ui.sum->setText(QString::number(m_sum));
	if (!m_inputted.empty())
	{
		ui.costTime->setText(tr("Cost: %1 s").arg((clock() - m_startTime) / 1000));
	}
}

void summons_practice::redo()
{
	if (!m_inputted.empty())
	{
		float last = m_inputted.back();
		m_inputted.pop_back();
		m_sum -= last;
	}
	ui.currentNum->setText("");
}

void summons_practice::addNew( float newNum )
{
	m_inputted.push_back(newNum);
	m_sum += newNum;
	ui.currentNum->setText("");
}

void summons_practice::cancelAll()
{
	ui.currentNum->setText("");
	m_inputted.clear();
	m_sum = 0.0f;
}

void summons_practice::complete()
{
	// reach goal
	clock_t end = clock();
	double cost = (end - m_startTime) / 1000;
	QMessageBox::information(NULL, "Time Counting", tr("Cost time: %1 second").arg(cost), QMessageBox::Yes, QMessageBox::Yes);
	cancelAll();
}

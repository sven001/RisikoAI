#ifndef DIALOGSTARTAICONTEST_H
#define DIALOGSTARTAICONTEST_H

#include <QDialog>
#include "ui_DialogStartAIContest.h"

using namespace Ui;

class DialogStartAIContest : public QDialog, public DialogStartAIContestClass
{
	Q_OBJECT

public:
	DialogStartAIContest(QWidget *parent = 0);
	~DialogStartAIContest();

private:
	QStringList dlls;
};

#endif // DIALOGSTARTAICONTEST_H

#ifndef RISIKO_H
#define RISIKO_H

#include <QMainWindow>
#include "ui_risiko.h"
#include "HumanPlayerGUI.h"
#include "QGameFlowViewer.h"

#include "RiskMap.h"
#include "RiskFlowControl.h"

class Risiko : public QMainWindow
{
	Q_OBJECT

public:
	Risiko(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~Risiko();

	void closeEvent( QCloseEvent * event );
	RiskFlowControl flowControl;

public slots:
	void forceSceneUpdate();
	void loadMap( void );
	void aiContest( void );

private:
	HumanPlayerGUI humanPlayerGUI;
	QGameFlowViewer gameFlowGUI;
	Ui::RisikoClass ui;
};

#endif // RISIKO_H

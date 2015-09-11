#ifndef QGAMEFLOWVIEWER_H
#define QGAMEFLOWVIEWER_H

#include <QDockWidget>
#include "ui_QGameFlowViewer.h"

using namespace Ui;

class QGameFlowViewer : public QDockWidget, public QGameFlowViewerClass
{
	Q_OBJECT

public:
	QGameFlowViewer(QWidget *parent = 0);
	~QGameFlowViewer();

public slots:
	void addLog( QString log );
};

#endif // QGAMEFLOWVIEWER_H

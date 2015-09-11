#include "stdafx.h"
#include "QGameFlowViewer.h"

QGameFlowViewer::QGameFlowViewer(QWidget *parent)
	: QDockWidget(parent)
{
	setupUi(this);
}

QGameFlowViewer::~QGameFlowViewer()
{

}

void QGameFlowViewer::addLog( QString log )
{
	this->textEditLog->append( log );
}
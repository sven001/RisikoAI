#include "stdafx.h"
#include "RiskMapGraphicsView.h"
#include "RiskMapScene.h"

RiskMapGraphicsView::RiskMapGraphicsView( QWidget *widget )
	: QGraphicsView( widget )
{

}

RiskMapGraphicsView::~RiskMapGraphicsView()
{

}

void RiskMapGraphicsView::resizeEvent ( QResizeEvent * event )
{
	if( scene() != NULL )
	{
		((RiskMapScene*)scene())->resize();
	}
}
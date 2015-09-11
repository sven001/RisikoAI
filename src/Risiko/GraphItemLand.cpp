#include "stdafx.h"
#include "GraphItemLand.h"
#include "RiskMapScene.h"
#include <QGraphicsSceneMouseEvent>

GraphItemLand::GraphItemLand(QGraphicsItem *parent, RiskMapLand *land)
	: QGraphicsItem(parent)
{
	myLand = land;
	myLand->setItem( this );
	setZValue( 0 );

	setPos( myLand->getArmyPos() );

	sizeRect = QRectF(0 , -12,  18, 12 );
	sizeRectShadow = QRectF(-5 , -17,  28, 23 );

	valueShade = (double)qrand() / RAND_MAX * 255;
}

GraphItemLand::~GraphItemLand()
{

}

RiskMapLand *GraphItemLand::getLand()
{
	return( myLand );
}

QRectF GraphItemLand::boundingRect() const
{
	return sizeRectShadow;
}

void GraphItemLand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							  QWidget *widget)
{
	(widget);
	(option);
	painter->setPen( Qt::lightGray );

	Qt::GlobalColor col= Qt::lightGray;
	col = (Qt::GlobalColor)((int)(col)+myLand->getOwner());

	QBrush brush( Qt::SolidPattern );
	brush.setColor( myLand->getAIShadeColor() );
	painter->fillRect( sizeRectShadow, brush );

	painter->setBrush( col );
	painter->drawRects( &sizeRect, 1);

	painter->setPen( Qt::black );
	QString text = QString::number( myLand->getArmys() );
	painter->drawText( 3 , -2, text );
}

void GraphItemLand::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	event->accept();
	(event);
}

void GraphItemLand::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	(event);

}

void GraphItemLand::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	((RiskMapScene*)scene())->emitLandSelected( myLand );
	event->accept();
	(event);
}


#include "stdafx.h"
#include "GraphItemConnection.h"
#include "GraphItemLand.h"

GraphItemConnection::GraphItemConnection(QGraphicsItem *parent, GraphItemLand *land1, GraphItemLand *land2)
	: QGraphicsItem(parent)
{
	this->land1 = land1;
	this->land2 = land2;

	setZValue( 1 );
}

GraphItemConnection::~GraphItemConnection()
{

}

QRectF GraphItemConnection::boundingRect() const
{
	QPointF point( 0,0 ), start, end;
	start = mapFromItem( land1, point );
	end = mapFromItem( land2, point );

	point = start;
	int lenX, lenY;
	if( start.x() > end.x() ) 
	{
		point.setX( end.x() );	
		lenX = start.x() - end.x();
	}
	else
	{
		lenX = end.x() - start.x();
	}
	if( start.y() > end.y() ) 
	{
		point.setY( end.y() );	
		lenY = start.y() - end.y();
	}
	else
	{
		lenY = end.y() - start.y();
	}
	return QRectF(point.x(), point.y(),
				  lenX, lenY );
}

void GraphItemConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							  QWidget *widget)
{
	(widget);
	(option);
	QPointF point( 0,0 ), start, end;
	start = mapFromItem( land1, point );
	end = mapFromItem( land2, point );
	painter->setPen( Qt::black );
	painter->drawLine( start,  end );
}
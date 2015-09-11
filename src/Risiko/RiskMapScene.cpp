#include "stdafx.h"
#include "GraphItemLand.h"
#include "GraphItemConnection.h"
#include "RiskMapScene.h"
#include "RiskMapLand.h"
#include "RiskMap.h" 

#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

RiskMapScene::RiskMapScene(RiskMap *map, QWidget *parent) 
	: QGraphicsScene(map) //parent)
{
	myMap = map;
	backGround = new QPixmap( map->getPictureMap() );
	clickableMap = new QImage( map->getClickableMap() );
	clickableMap->convertToFormat( QImage::Format_RGB16 );
/*
	Qpixmap = pixmap.scaled( rect.width(),
							rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
*/
	QGraphicsPixmapItem *back = addPixmap( *backGround );
	back->setPos( 0, 0 );
	back->setZValue(-10);
	//back->setEnabled( false );

	QList<RiskMapLand *> list = map->getAllLands();
	for( int i=0; i<list.size(); i++ )
	{	
		//GridPos;
		double deg = (3.1415*2) / list.size() * i;
		GraphItemLand *gi = new GraphItemLand( NULL, list[i] );
		this->addItem( gi );
	}	
/*
	QList<RiskMapConnection *> conn =  map->getAllConnections();
	for( int i=0; i<conn.size(); i++ )
	{
		RiskMapLand *l1 = conn[i]->getLand( 0 );
		RiskMapLand *l2 = conn[i]->getLand( 1 );

		GraphItemConnection *gc = new GraphItemConnection( NULL, getGraphItemFor(l1), getGraphItemFor(l2) );
		this->addItem( gc );
	}
*/
}

RiskMapScene::~RiskMapScene()
{

}

void RiskMapScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
}

void RiskMapScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	QPoint p( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
	QColor c( clickableMap->pixel( p ) );

	if( c.red() < 255 )
	{
		mouseEvent->accept();
		return;
	}
	mouseEvent->ignore();
}

void RiskMapScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
	QPoint p( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
	QColor c( clickableMap->pixel( p ) );
	if( c.red() != 0 )
	{
		emitLandSelected( myMap->findLandById( c.red() ) );
	}
}

void RiskMapScene::emitLandSelected( RiskMapLand *land)
{
	emit signalLandSelected( land );
}
 
GraphItemLand *RiskMapScene::getGraphItemFor( RiskMapLand *land )
{
	QList<QGraphicsItem *> iList = items();
	for( int i=0; i<iList.size(); i++ )
	{
		GraphItemLand* landItem = qgraphicsitem_cast<GraphItemLand*>( iList[i] );
		if( landItem )
		{
			if( landItem->getLand() == land ) return( landItem );
		}
	}
	return( NULL );
}

void RiskMapScene::resize()
{
	this->views()[0]->fitInView( this->sceneRect() );
}
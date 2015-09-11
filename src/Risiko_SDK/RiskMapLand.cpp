#include "RiskMapLand.h"
#include "RiskMapConnection.h"

RiskMapLand::RiskMapLand(int id, QString name, int continentalIndex, QPointF armyPos)
	: QObject(NULL)
{
	this->landId = id;
	this->landName = name;

	neighbors.clear();

	this->continentalIndex = continentalIndex;
	this->armyPos = armyPos;
	this->owner = 0;
	this->armys = 0;

	myItem = NULL;

	shadeColor.setRgb( 0,0,0,0 );
}

RiskMapLand::~RiskMapLand()
{
	neighbors.clear();
}

void RiskMapLand::addNeighbor( RiskMapConnection *neighbor )
{
	neighbors.append( neighbor );
}

QString RiskMapLand::getLandName()
{
	return( landName );
}

int RiskMapLand::getLandId()
{
	return( landId );
}

int RiskMapLand::getOwner()
{
	return( owner );
}

int RiskMapLand::getArmys()
{
	return( armys );
}

bool RiskMapLand::hasEnemyNeighbor()
{
	QLandList list = this->getNeighbors();
	for( int i=0; i<list.size(); i++ )
	{
		if( list[i]->getOwner() != this->owner ) return( true );
	}
	return( false );
}

void RiskMapLand::setOwner( int owner )
{
	this->owner = owner;

	if( myItem!= NULL ) myItem->update();
}

void RiskMapLand:: setArmys( int armys )
{
	this->armys = armys;

	if( myItem!= NULL ) myItem->update();
}


QList<RiskMapLand *> RiskMapLand::getNeighbors()
{
	QList<RiskMapLand *> list;
	
	for( int i=0; i<neighbors.size(); i++ )
	{
		RiskMapLand *l = neighbors[i]->getOtherLand( this );
		if( l == NULL ) qWarning( "RiskMapLand::getNeighbors(): FAIL" );
		list.append( l );	
	}
	return( list );
}

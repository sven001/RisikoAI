#include "RiskMapContinent.h"

RiskMapContinent::RiskMapContinent( QString name, int id, int armyBonus )
	: QObject(NULL)
{
	lands.clear();
	this->name = name;
	this->id = id;
	this->armyBonus = armyBonus;
}

RiskMapContinent::~RiskMapContinent()
{
	lands.clear();
}

void RiskMapContinent::addLand( RiskMapLand *land )
{
	if( land->getContinentId() == id )
	{
		lands.append( land );
	}
}

QList<RiskMapLand *> RiskMapContinent::getLands()
{
	return( lands );
}

int RiskMapContinent::isOwnedByPlayer()
{
	int owner = lands[0]->getOwner();
	for( int i=1; i<lands.size(); i++ )
	{
		if( owner != lands[i]->getOwner() ) return( -1 );
	}
	return( owner );
}
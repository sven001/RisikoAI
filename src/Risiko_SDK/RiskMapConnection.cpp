#include "RiskMapConnection.h"
#include "RiskMapLand.h"

RiskMapConnection::RiskMapConnection(RiskMapLand *land1, RiskMapLand *land2)
	: QObject(NULL)
{
	if( (land1 == NULL ) ||
		(land2 == NULL ) ) qWarning( "RiskMapConnection: FAIL" );
	this->land1 = land1;
	this->land2 = land2;

	land1->addNeighbor( this );
	land2->addNeighbor( this );

}

RiskMapConnection::~RiskMapConnection()
{

}

RiskMapLand *RiskMapConnection::getLand( int index )
{
	if( index == 0 ) return( land1 );
	if( index == 1 ) return( land2 );
	return( NULL );
}

RiskMapLand *RiskMapConnection::getOtherLand( RiskMapLand *land )
{
	if( land1 == land ) return( land2 );
	if( land2 == land ) return( land1 );
	return( NULL );
}

bool RiskMapConnection::connects( int id1, int id2 )
{
	if( id1 == land1->getLandId() && id2 == land2->getLandId() ) return( true );
	if( id2 == land1->getLandId() && id1 == land2->getLandId() ) return( true );
	return( false );
}
	
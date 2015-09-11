#include "RiskMap.h"

#include <QFile>

RiskMap::RiskMap()
	: QObject(NULL)
{
	clearLists();
}

RiskMap::~RiskMap()
{
	clear();
	
}

void RiskMap::clear()
{
	mapName = "";
	picName = "";
	clearLists();
}

void RiskMap::connectLands( int id1, int id2 )
{
	//try to find equal connection
	for( int i=0; i<connections.size(); i++ )
	{
		if( connections[i]->connects( id1, id2 ) ) return;
	}

	connections.append( new RiskMapConnection( findLandById( id1 ), findLandById( id2 ) ) ); 
}

void RiskMap::clearLists()
{
	while( !lands.isEmpty() ) delete( lands.takeFirst() );
	while( !connections.isEmpty() ) delete( connections.takeFirst() );
	while( !continents.isEmpty() ) delete( continents.takeFirst() );
}

bool RiskMap::loadTestMap()
{
	loadFromFile( ":/Risiko/standardMapText" );
/*
	lands.append( new RiskMapLand( 0, "Land 1", 1, 3 ) );
	lands.append( new RiskMapLand( 1, "Land 2", 1, 3 ) );
	lands.append( new RiskMapLand( 2, "Land 3", 1, 3 ) );
	lands.append( new RiskMapLand( 3, "Land 4", 2, 4 ) );
	lands.append( new RiskMapLand( 4, "Land 5", 2, 4 ) );
	lands.append( new RiskMapLand( 5, "Land 6", 2, 3 ) );

	connectLands( 0, 1 );
	connectLands( 1, 2 );
	connectLands( 2, 3 );
	connectLands( 3, 4 );
	connectLands( 4, 5 );
	connectLands( 1, 3 );
	connectLands( 3, 5 );
*/
	return( true );
}

bool RiskMap::loadFromFile( QString filename )
{
	QStringList sl;
	QFile file( filename );

	if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) 
	{
		return( false );
	}

	clearLists();

	int section = 0;
	while( !file.atEnd() ) 
	{
         QByteArray ba = file.readLine();
		 QString line( ba );
		 line.chop( 1 );
		 line.trimmed();
		 sl = line.split(" ");

		 if( section == 0 )
		 {
			 if( sl[0].compare( "pic" ) == 0 )
			 {
				 picName = sl[1];
			 }
			 if( sl[0].compare( "map" ) == 0 )
			 {
				 mapName = sl[1];
			 }
		 }
		 if( section == 1 &&
			 sl.size() > 2)
		 {
			QString name = sl[0];
			int armyBonus = sl[1].toInt();
			//Color isnt needed
			this->continents.append( new RiskMapContinent( name, continents.size(), armyBonus ) );
		 }
		 if( section == 2 &&
			 sl.size() > 4 )
		 {
			 int index = sl[0].toInt();
			 QString name = sl[1];
			 int contIndex = sl[2].toInt() - 1;
			 QPointF arnmyPos( sl[3].toInt(), sl[4].toInt() );

			 lands.append( new RiskMapLand( index, name, contIndex, arnmyPos ) );
		 }
		 if( section == 3 &&
			 sl.size() > 1 )
		 {
			 int home = sl[0].toInt();
			 for( int i=1; i<sl.size(); i++ )
			 {
				connectLands( home, sl[i].toInt() );
		     }
		 }

		 if( sl[0].compare( "[continents]" ) == 0 ) { section = 1; }
		 if( sl[0].compare( "[countries]" ) == 0 ) { section = 2; }
		 if( sl[0].compare( "[borders]" ) == 0 ) { section = 3; }
	}
 
	//Fill continents with lands
	for( int i=0; i<lands.size(); i++ )
	{
		int contId = lands[i]->getContinentId();
		continents[contId]->addLand( lands[i] );
	}

	qWarning( "Map loaded: Lands [%d] Continents [%d] Connections [%d]\n", lands.size(), continents.size(), connections.size() );
	

	return( true );
}

RiskMapLand *RiskMap::findLandById( int id )
{
	for( int i=0; i<lands.size(); i++ )
	{
		if( lands[i]->getLandId() == id ) return( lands[i] );
	}
	return( NULL );
}

QList<RiskMapLand *> RiskMap::getAllLands()
{
	return( lands );
}

QList<RiskMapConnection *> RiskMap::getAllConnections()
{
	return( connections );
}

void RiskMap::markAll( double mark )
{
	for( int i=0; i<lands.size(); i++ ) lands[i]->temp = mark;
}

QList<RiskMapLand *> RiskMap::findShortestPath( RiskMapLand *att, RiskMapLand *def, bool ownerOnly )
{
	int maxTemp = lands.size()+1;
	markAll( maxTemp );
	def->temp = 0;
	rekFindShortestPath( def, att, ownerOnly );

	//Gather shortest path
	QList<RiskMapLand *> list;
	RiskMapLand *cur = att;
	
	while( cur != def )
	{
		list.append( cur );

		//Find neighbor with lowest temp, that is not me.
		// If temp == maxTemp its unreachable
		RiskMapLand *nextCur = NULL;
		
		QList<RiskMapLand *> n = cur->getNeighbors();
		for( int i=0; i<n.size(); i++ )
		{
			if( n[i]->temp == cur->temp-1 ) 
			{
				nextCur = n[i];
			}
		}
		if( !nextCur ) return( QList<RiskMapLand *>() );
		cur = nextCur;
	}
	list.append( def );
	return( list );
}

void RiskMap::rekFindShortestPath( RiskMapLand *att, RiskMapLand *def, bool ownerOnly )
{
	if( def == att )
	{
		return;
	}

	double currentMark = att->temp;
	int owner = att->getOwner();


	QList<RiskMapLand *> n = att->getNeighbors();
	for( int i=0; i<n.size(); i++ )
	{
		if( n[i]->temp > (currentMark + 1) )
		{
			if( !ownerOnly ||
				n[i]->getOwner() == owner )
			{
				n[i]->temp = currentMark+1;
				rekFindShortestPath( n[i], def, ownerOnly );
			}
		}
	}
	return;
}

bool RiskMap::playerHasLands( int owner )
{
	for( int i=0; i<lands.size(); i++ ) 
	{
		if( lands[i]->getOwner() == owner ) return( true );
	}
	return( false );
}	

bool RiskMap::playerHasAllLands( int owner )
{
	for( int i=0; i<lands.size(); i++ ) 
	{
		if( lands[i]->getOwner() != owner ) return( false );
	}
	return( true );
}

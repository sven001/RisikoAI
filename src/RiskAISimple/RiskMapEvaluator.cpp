
#include "RiskMapEvaluator.h"

RiskMapEvaluator::RiskMapEvaluator(RiskMap *map)
	: QObject(NULL)
{
	this->map = map;

	numberOfLands = map->getAllLands().size();
	threatVal = new double[numberOfLands];
	stratVal = new double[numberOfLands];

	findStrategicValues( 100 );
}

RiskMapEvaluator::~RiskMapEvaluator()
{
	delete [] threatVal;
	delete [] stratVal;
}

void RiskMapEvaluator::findStrategicValues( int maxIter )
{
	QList<RiskMapLand *> list;
	
	memset( stratVal, 0, numberOfLands * sizeof( double ) );

	int max = numberOfLands-1;
	for( int i=0; i<maxIter; i++ )
	{
		RiskMapLand *l1 = map->getAllLands().at( (int)((double)qrand()/RAND_MAX) * max );
		RiskMapLand *l2 = map->getAllLands().at( (int)((double)qrand()/RAND_MAX) * max );

		list = map->findShortestPath( l1, l2, false );

		while( !list.isEmpty() ) 
		{
			RiskMapLand *cur = list.takeFirst();
			stratVal[cur->getLandId()-1]++;
		}
	}

	QString landVal = "";
	list = map->getAllLands();
	max = 0;
	for( int i=0; i<numberOfLands; i++ )
	{
		if( stratVal[i] > max ) max = stratVal[i];

		//Just for debug
		//landVal += list[i]->getLandName() + " " + QString::number( stratVal[i] ) + "\n";
	}
	for( int i=0; i<numberOfLands; i++ )
	{
		stratVal[i] = stratVal[i] / max;
	}
	//QMessageBox qm( "Info", landVal, QMessageBox::Information, 0, 0, 0 );
	//qm.exec();
}

int RiskMapEvaluator::filterAll()
{
	filtered = map->getAllLands();
	return( filtered.size() );
}

int RiskMapEvaluator::filterOnlyConti( int contiIndex )
{
	QList<RiskMapContinent*> list = map->getAllContinents();
	if( list.size() > contiIndex )
	{
		filtered = list[contiIndex]->getLands();
	}
	else
	{
		filtered.clear();
	}
	return( filtered.size() );
}
int RiskMapEvaluator::filterOnlyPlayer( int playerIndex )
{
	QList<RiskMapLand*> list = map->getAllLands();
	filtered.clear();

	while( !list.isEmpty() )
	{
		RiskMapLand *cur = list.takeFirst();
		if( cur->getOwner() == playerIndex )
		{
			filtered.append( cur );
		}
	}
	return( filtered.size() );
}
int RiskMapEvaluator::filterBorderPlayer( int playerIndex )
{
	QList<RiskMapLand*> list = map->getAllLands();
	filtered.clear();

	while( !list.isEmpty() )
	{
		RiskMapLand *cur = list.takeFirst();
		if( cur->getOwner() == playerIndex )
		{
			QList<RiskMapLand*> neigh = cur->getNeighbors();
			while( !neigh.isEmpty() )
			{
				RiskMapLand *curNeigh = neigh.takeFirst();
				if( curNeigh->getOwner() != playerIndex )
				{
					if( filtered.indexOf( curNeigh ) == -1 )
					{
						filtered.append( curNeigh );
					}
				}
			}
		}
	}
	
	return( filtered.size() );
}

int RiskMapEvaluator::filterNeighbors( RiskMapLand *mid )
{
	filtered = mid->getNeighbors();
	return( filtered.size() );
}

int RiskMapEvaluator::addFilterHasEnemyNeighbor()
{
	QList<RiskMapLand*> list;
	for( int i = filtered.size()-1; i>=0; i-- )
	{
		list = filtered[i]->getNeighbors();
		bool takeIt = false;
		while( !list.isEmpty() )
		{
			if( list.takeFirst()->getOwner() != filtered[i]->getOwner() )
			{
				takeIt = true;
			}
		}
		if( !takeIt )
		{
			filtered.takeAt( i );
		}
	}
	return( filtered.size() );
}

int RiskMapEvaluator::addFilterHasNoEnemyNeighbor()
{
	QList<RiskMapLand*> list;
	for( int i = filtered.size()-1; i>=0; i-- )
	{
		list = filtered[i]->getNeighbors();
		bool takeIt = true;
		while( !list.isEmpty() )
		{
			if( list.takeFirst()->getOwner() != filtered[i]->getOwner() )
			{
				takeIt = false;
			}
		}
		if( !takeIt )
		{
			filtered.takeAt( i );
		}
	}
	return( filtered.size() );
}

int RiskMapEvaluator:: addFilterHasMoreThanXArmys( int armys )
{
	QList<RiskMapLand*> list;
	for( int i = filtered.size()-1; i>=0; i-- )
	{
		if( filtered[i]->getArmys() <= armys )
		{
			filtered.takeAt( i );
		}
	}
	return( filtered.size() );
}

int RiskMapEvaluator::addFilterHasLessThanXArmys( int armys )
{
	QList<RiskMapLand*> list;
	for( int i = filtered.size()-1; i>=0; i-- )
	{
		if( filtered[i]->getArmys() >= armys )
		{
			filtered.takeAt( i );
		}
	}
	return( filtered.size() );
}

int RiskMapEvaluator::addFilterIsEnemy( int playerIndex )
{
	QList<RiskMapLand*> list;
	for( int i = filtered.size()-1; i>=0; i-- )
	{
		if( filtered[i]->getOwner() == playerIndex )
		{
			filtered.takeAt( i );
		}
	}
	return( filtered.size() );
}

RiskMapLand *RiskMapEvaluator::getHighestArmyCountFromFilter()
{
	int max = 0;
	RiskMapLand *ret = NULL;

	for( int i=0; i<filtered.size(); i++ )
	{
		if( filtered[i]->getArmys() > max ||
			ret == NULL )
		{
			max = filtered[i]->getArmys();
			ret = filtered[i];
		}
	}
	return( ret );
}

RiskMapLand *RiskMapEvaluator::getHighestStratFromFilter()
{
	int max = 0;
	RiskMapLand *ret = NULL;
	int idx;
	for( int i=0; i<filtered.size(); i++ )
	{
		idx = filtered[i]->getLandId() - 1;
		if( stratVal[i] > max ||
			ret == NULL )
		{
			max = stratVal[i];
			ret = filtered[i];
		}
	}
	return( ret );
}

RiskMapLand *RiskMapEvaluator::getHighestThreatFromFilter()
{
	int max = 0;
	RiskMapLand *ret = NULL;
	int idx;
	for( int i=0; i<filtered.size(); i++ )
	{
		idx = filtered[i]->getLandId() - 1;
		if( threatVal[i] > max ||
			ret == NULL )
		{
			max = threatVal[i];
			ret = filtered[i];
		}
	}
	return( ret );
}
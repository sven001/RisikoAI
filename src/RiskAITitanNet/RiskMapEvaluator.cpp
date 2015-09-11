#include "stdafx.h"
#include "RiskMapEvaluator.h"

RiskMapEvaluator::RiskMapEvaluator(RiskMap *map)
	: QObject(NULL)
{
	this->map = map;

	numberOfLands = map->getAllLands().size();
	tempVal = new double[numberOfLands];
	stratVal = new double[numberOfLands];

	findStrategicValues();
}

RiskMapEvaluator::~RiskMapEvaluator()
{
	delete [] tempVal;
	delete [] stratVal;
}

void RiskMapEvaluator::findStrategicValues()
{
	double *contiStratVal = new double[map->getAllContinents().size()];
	memset( stratVal, 0, numberOfLands * sizeof( double ) );
	memset( contiStratVal, 0, map->getAllContinents().size() * sizeof( double ) );

	for( int i=0; i<map->getAllContinents().size(); i++ )
	{
		this->filterOnlyConti( i );
		for( int n=0; n<filtered.size(); n++ )
		{
			int landIndex = filtered[n]->getLandId() -1;
			QLandList list = filtered[n]->getNeighbors();
			int isContiBorder = 0;
			for( int k=0; k<list.size(); k++ )
			{
				if( filtered.indexOf( list[k] ) == -1 ) 
				{
					isContiBorder = 1;
					break;
				}
			}
			stratVal[landIndex] = isContiBorder;
			contiStratVal[i] += stratVal[landIndex];
		}
	}

	for( int i=0; i<map->getAllContinents().size(); i++ )
	{
		this->filterOnlyConti( i );
		for( int n=0; n<filtered.size(); n++ )
		{
			int landIndex = filtered[n]->getLandId() -1;
			stratVal[landIndex] += 8-contiStratVal[i];
		}
	}

	QString landVal = "";
	QLandList list = map->getAllLands();

	free( contiStratVal );
}

void RiskMapEvaluator::evaluateTempVal( int playerIndex )
{
	double *contiStratVal = new double[map->getAllContinents().size()];
	memset( tempVal, 0, numberOfLands * sizeof( double ) );
	memset( contiStratVal, 0, map->getAllContinents().size() * sizeof( double ) );

	double max = 0;
	double factor = 1;
	for( int i=0; i<map->getAllContinents().size(); i++ )
	{
		this->filterOnlyConti( i );
		contiStratVal[i] = filtered.size();

		this->addFilterIsEnemy( playerIndex );
		contiStratVal[i] /= filtered.size() * filtered.size();

		contiStratVal[i] *= 5;

		factor = 1;
		//Triple value of lands, if conti is sole accessed by single player
		if( map->getAllContinents()[i]->isOwnedByPlayer() != -1 )
		{
			factor = 3;
		}

		this->filterOnlyConti( i );
		for( int n=0; n<filtered.size(); n++ )
		{
			tempVal[filtered[n]->getLandId()-1] = contiStratVal[i] * factor;
			if( (stratVal[filtered[n]->getLandId()-1] + tempVal[filtered[n]->getLandId()-1]) > max ) max = stratVal[filtered[n]->getLandId()-1] + tempVal[filtered[n]->getLandId()-1];
		}


	}



/*
	QString landVal = "";
	for( int i=0; i<numberOfLands; i++ )
	{
		//Just for debug
		landVal += map->getAllLands()[i]->getLandName() + " " + QString::number( stratVal[i] ) + " " + QString::number( tempVal[i] ) + " = " + QString::number( stratVal[i]+tempVal[i] )+"\n";
		map->getAllLands()[i]->setAIShadeColor( 255, 0, 0, (stratVal[i] + tempVal[i]) / max * 200 );
	}

	QMessageBox qm( "Info", landVal, QMessageBox::Information, 0, 0, 0 );
	qm.exec();
*/
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
	double max = 0;
	RiskMapLand *ret = NULL;
	int idx;
	for( int i=0; i<filtered.size(); i++ )
	{
		idx = filtered[i]->getLandId() - 1;
		if( (stratVal[idx]+tempVal[idx]) > max || 
			ret == NULL )
		{
			max = (stratVal[idx]+tempVal[idx]);
			ret = filtered[i];
		}
	}
	return( ret );
}

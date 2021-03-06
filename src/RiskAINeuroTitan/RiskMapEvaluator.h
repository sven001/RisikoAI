#ifndef RISKMAPEVALUATOR_H
#define RISKMAPEVALUATOR_H

#include <QObject>
#include "RiskMap.h"

class RiskMapEvaluator : public QObject
{
	Q_OBJECT

public:
	RiskMapEvaluator( RiskMap *map );
	~RiskMapEvaluator();

	QList<RiskMapLand*> getFilteredList() {return( filtered );};
	int getFilteredCount() { return( filtered.size() );};

	void evaluateTempVal( int playerIndex );

	int filterAll();
	int filterOnlyConti( int contiIndex );
	int filterOnlyPlayer( int playerIndex );
	int filterBorderPlayer( int playerIndex );
	int filterNeighbors( RiskMapLand *mid );

	int addFilterHasEnemyNeighbor();
	int addFilterHasNoEnemyNeighbor();
	int addFilterHasMoreThanXArmys( int armys );
	int addFilterHasLessThanXArmys( int armys );
	int addFilterIsEnemy( int playerIndex );
	
	RiskMapLand *getHighestArmyCountFromFilter();
	RiskMapLand *getHighestStratFromFilter();


private:

	void findStrategicValues();

	RiskMap *map;

	int numberOfLands;
	double *tempVal;
	double *stratVal;

	QList<RiskMapLand*> filtered;

};

#endif // RISKMAPEVALUATOR_H

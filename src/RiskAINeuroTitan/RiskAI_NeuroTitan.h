#ifndef RISKAI_TitanNet_H
#define RISKAI_TitanNet_H

#include "Player.h"
#include "RiskAI.h"
#include "RiskMap.h"
#include "RiskGameLogic.h"
#include "RiskMapEvaluator.h"
#include "NeuralNet.h"

#include <qdir.h>

class RiskAI_NeuroTitan : public RiskAI
{
	Q_OBJECT

public:
	RiskAI_NeuroTitan();
	~RiskAI_NeuroTitan();

	bool yourTurn( GAMEPHASE phase );

private:
	QDir dir;
	RiskMapEvaluator *eval;
	NeuralNet *nnet;
	void addArmyPhase();
	RiskMapLand *attackFrom( RiskMapLand *attacker );
	void exchangeCards();
	void attackPhase();
	void movePhase();

	void populateNNet();
	void gatherInputVector( SVector &input );

	int myTier;
	int findNumberOfTiers();
	int findNumberOfGenomsInTier( int tier );
	QList<double> loadGenom( int tier, int index );
	void saveGenom( int tier, QList<double> genom );
	
	QList<double> crossGenoms( int tier, int g1, int g2 );
	void mutateGenom( QList<double> &genom );

};

#endif // RISKAI_TitanNet_H

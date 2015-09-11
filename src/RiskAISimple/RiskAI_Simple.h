#ifndef RISKAI_SIMPLE_H
#define RISKAI_SIMPLE_H

#include "Player.h"
#include "RiskAI.h"
#include "RiskMap.h"
#include "RiskGameLogic.h"

#include "RiskMapEvaluator.h"

class RiskAI_Simple : public RiskAI
{
	Q_OBJECT

public:
	RiskAI_Simple();
	~RiskAI_Simple();

	bool yourTurn( GAMEPHASE phase );

private:
	void addArmyPhase();
	void exchangeCards();
	
	void attackPhase();
	RiskMapLand *attackFrom( RiskMapLand *attacker );
	void movePhase();

	double defPercent;
	RiskMapEvaluator *eval;
};

#endif // RISKAI_SIMPLE_H

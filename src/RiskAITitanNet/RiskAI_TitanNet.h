#ifndef RISKAI_TitanNet_H
#define RISKAI_TitanNet_H

#include "Player.h"
#include "RiskAI.h"
#include "RiskMap.h"
#include "RiskGameLogic.h"

#include "RiskMapEvaluator.h"

class RiskAI_TitanNet : public RiskAI
{
	Q_OBJECT

public:
	RiskAI_TitanNet();
	~RiskAI_TitanNet();

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

#endif // RISKAI_TitanNet_H

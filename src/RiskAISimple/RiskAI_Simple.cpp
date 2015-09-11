#include "RiskAI_Simple.h"

RiskAI_Simple::RiskAI_Simple()
	: RiskAI()
{
	eval = NULL;
	defPercent = (((double)qrand() / RAND_MAX * 30) + 10) / 100;

}

RiskAI_Simple::~RiskAI_Simple()
{

}

bool RiskAI_Simple::yourTurn( GAMEPHASE phase )
{
	if( eval == NULL )
	{
		eval = new RiskMapEvaluator( map );
	}

	switch( phase )
	{
	case ADD_ARMYS:
		{
			addArmyPhase();
		}
		break;
	case ATTACK:
		{
			attackPhase();
		}
		break;
	case MOVE:
		{
			movePhase();
		}
		break;
	}

	return( true );
}

void RiskAI_Simple::addArmyPhase()
{
	//Exchange cards
	exchangeCards();

	int armys = logic->getCurrentFreeArmys();

	int defArmys = armys * defPercent;
	

	//Def Armys
	eval->filterOnlyPlayer( player->getPlayerId() );
	eval->addFilterHasEnemyNeighbor();
	logic->setArmy( player, eval->getHighestStratFromFilter(), defArmys );

	//Att Armys
	armys = logic->getCurrentFreeArmys();
	eval->filterBorderPlayer( player->getPlayerId() );
	//Find valueable target 
	RiskMapLand *target = eval->getHighestStratFromFilter();
	//Find my land in reach
	RiskMapLand *inReach = NULL;
	QList<RiskMapLand*> neigh = target->getNeighbors();
	for( int i=0; i<neigh.size(); i++ )
	{
		if( neigh[i]->getOwner() == player->getPlayerId() )
		{
			if( inReach == NULL ||
				neigh[i]->getArmys() > inReach->getArmys() )
			{
				inReach = neigh[i];
			}

		}
	}
	logic->setArmy( player, inReach, armys );

	armys = logic->getCurrentFreeArmys();
	if( armys > 0 )
	{
		eval->filterBorderPlayer( player->getPlayerId() );
		logic->setArmy( player, eval->getHighestStratFromFilter(), armys );
	}
}

void RiskAI_Simple::exchangeCards()
{
	RiskCard *c[3];
	RiskCardDeck *deck = player->getHand();
	if( deck->findSet( c ) )
	{
		if( deck->getNumberOfCards() > 4 ||
			logic->getCurrentFreeArmys() == 3 )
		{
			logic->exchangeCards( player, c[0], c[1], c[2] );
		}
	}
}

void RiskAI_Simple::attackPhase()
{
	QList<RiskMapLand *> willAttack;
	
	eval->filterOnlyPlayer( player->getPlayerId() );
	eval->addFilterHasEnemyNeighbor();
	eval->addFilterHasMoreThanXArmys( 1 );
	willAttack = eval->getFilteredList();

	if( eval->addFilterHasMoreThanXArmys( 3 ) > 0 )
	{
		willAttack = eval->getFilteredList();
	}

	while( !willAttack.isEmpty() )
	{
		RiskMapLand *newAttack = attackFrom( willAttack.takeFirst() );
		if( newAttack != NULL ) willAttack.append( newAttack );
	}
}

RiskMapLand *RiskAI_Simple::attackFrom( RiskMapLand *attacker )
{
	int defArmys = 1;

	//Find defender
	eval->filterNeighbors( attacker );
	eval->addFilterIsEnemy( player->getPlayerId() );
	eval->addFilterHasLessThanXArmys( attacker->getArmys() - defArmys );
	if( eval->getFilteredCount() == 0 ) return( NULL );

	RiskMapLand *defender = eval->getHighestStratFromFilter();

	while( attacker->getArmys() > defArmys &&
		   attacker->getOwner() != defender->getOwner() )
	{
		logic->attack( player, attacker, defender, attacker->getArmys() - defArmys );
	}

	if( defender->getOwner() != attacker->getOwner() ) defender = NULL;
	return( defender );
}

void RiskAI_Simple::movePhase()
{
	// Find max armys not on border
	eval->filterOnlyPlayer( player->getPlayerId() );
	eval->addFilterHasNoEnemyNeighbor();
	RiskMapLand *maxArmyLand = eval->getHighestArmyCountFromFilter();

	//No armys in the heartland.. screw it
	if( maxArmyLand == NULL ) return;

	//Find max threaded border land
	eval->filterOnlyPlayer( player->getPlayerId() );
	eval->addFilterHasEnemyNeighbor();
	RiskMapLand *landInNeed = eval->getHighestStratFromFilter();

	if( landInNeed == NULL ) return;

	logic->moveArmy( player, maxArmyLand, landInNeed, maxArmyLand->getArmys() - 1 );
}

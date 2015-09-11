#include "stdafx.h"
#include "RiskAI_NeuroTitan.h"
#include "float.h"
#include <SVector.h>
#include <SMatrix.h>

RiskAI_NeuroTitan::RiskAI_NeuroTitan()
	: RiskAI()
{
	myTier = -1;
	eval = NULL;
	nnet = NULL;

	dir.setCurrent( qApp->applicationDirPath() );
	dir.mkdir( "NeuroTitan v1" );
	dir.cd( "NeuroTitan v1" );
}

RiskAI_NeuroTitan::~RiskAI_NeuroTitan()
{

}

bool RiskAI_NeuroTitan::yourTurn( GAMEPHASE phase )
{
	if( eval == NULL )
	{
		eval = new RiskMapEvaluator( map );
	}
	if( !nnet )
	{
		nnet = new NeuralNet( map->getAllLands().size() + 1, 1 );
		populateNNet();
	}
	switch( phase )
	{
	case ADD_ARMYS:
		{
			eval->evaluateTempVal( player->getPlayerId() );
			addArmyPhase();
		}
		break;
	case ATTACK:
		{
			attackPhase();

			//Check if I have won
			eval->filterOnlyPlayer( player->getPlayerId() );
			if( eval->getFilteredCount() == map->getAllLands().size() )
			{
				// I won.. I won..
				//this->saveGenom( nnet->getNumberOfLearningSteps(), nnet->getGenom() );
				this->saveGenom( myTier + 1, nnet->getGenom() );
			}
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

void RiskAI_NeuroTitan::addArmyPhase()
{
	SVector in;
	gatherInputVector( in );

	this->exchangeCards();
	in[0] = logic->getCurrentFreeArmys();
	nnet->setInput( in );
	nnet->compute();
	in = nnet->getOutput();

	RiskMapLand *target = NULL;
	eval->filterOnlyPlayer( player->getPlayerId() );
	QList<RiskMapLand*> list = eval->getFilteredList();

	while( !list.isEmpty() )
	{
		RiskMapLand *temp = list.takeFirst();

		if( target == NULL ||
		    in[target->getLandId()] < in[temp->getLandId()] )
		{
			target = temp;
		}
	}

	logic->setArmy( player, target, logic->getCurrentFreeArmys() );
}

void RiskAI_NeuroTitan::exchangeCards()
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

void RiskAI_NeuroTitan::attackPhase()
{
	//Find Lands with spare armys
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

RiskMapLand *RiskAI_NeuroTitan::attackFrom( RiskMapLand *attacker )
{
	SVector in;
	gatherInputVector( in );

	in[0] = 0;
	nnet->setInput( in );
	nnet->compute();
	in = nnet->getOutput();

	//Find defender
	eval->filterNeighbors( attacker );
	eval->addFilterIsEnemy( player->getPlayerId() );
	if( eval->getFilteredCount() == 0 ) return( NULL );
	QList<RiskMapLand*> list = eval->getFilteredList();

	//Find defender with highest output
	RiskMapLand *target = NULL; 
	in[0] = -DBL_MAX;

	while( !list.isEmpty() )
	{
		RiskMapLand *temp = list.takeFirst();

		if( target == NULL ||
		    in[target->getLandId()] < in[temp->getLandId()] )
		{
			target = temp;
		}
	}

	while( target &&
		   attacker->getArmys() > 1 &&
		   attacker->getOwner() != target->getOwner() )
	{
		logic->attack( player, attacker, target, attacker->getArmys() - 1 );
	}

	if( target && 
		target->getOwner() != attacker->getOwner() ) target = NULL;
	return( target ); 
}

void RiskAI_NeuroTitan::movePhase()
{
	// Find max armys not on border
	eval->filterOnlyPlayer( player->getPlayerId() );
	eval->addFilterHasNoEnemyNeighbor();
	RiskMapLand *maxArmyLand = eval->getHighestArmyCountFromFilter();

	//No armys in the heartland.. screw it
	if( maxArmyLand == NULL ) return;

	SVector in;
	gatherInputVector( in );
	in[0] = maxArmyLand->getArmys()-1;
	in[maxArmyLand->getLandId()] = 1;
	nnet->setInput( in );
	nnet->compute();
	RiskMapLand *landInNeed = map->findLandById( nnet->getHighestOutputIndex() );

	if( !landInNeed ||
		!landInNeed->hasEnemyNeighbor() )
	{
		nnet->learnOutput( nnet->getHighestOutputIndex(), 0.1 );
		landInNeed = NULL;
	}

	if( landInNeed )
	{
		if( map->findShortestPath( maxArmyLand, landInNeed, true ).size() > 0 )
		{
			logic->moveArmy( player, maxArmyLand, landInNeed, maxArmyLand->getArmys() - 1 );
		}
		else
		{
			nnet->learnOutput( nnet->getHighestOutputIndex(), 0.1 );
		}
	}

	
}



void RiskAI_NeuroTitan::populateNNet()
{	
	int tiers = findNumberOfTiers();
	if( tiers > 0 )
	{
		myTier = ((double)qrand() / RAND_MAX*tiers);
		int genoms = findNumberOfGenomsInTier( myTier );
		if( genoms > 0 )
		{
			QList<double> genom = this->loadGenom(myTier, ((double)qrand() / RAND_MAX*(genoms-1)));
			if( genom.size() > 10 )
			{
				mutateGenom( genom );
				nnet->setGenom( genom );
			}
			else myTier = -1;
		}
		else myTier = -1;
	}
	else myTier = -1;

	if( myTier == -1 )
	{
		//Generiere neues NNet
		int numberOfLands = map->getAllLands().size();
		//Verstärkungsknoten (0) durchverbinden
		for( int i = 0; i<(numberOfLands+1); i++ ) nnet->setWeight( 0, 0, i, 1 );
		nnet->setWeight( 0, 0, 0, 0 );
		RiskMapLand *land1, *land2;
		for( int l1 = 0; l1<numberOfLands; l1++ )
			for( int l2 = 0; l2<numberOfLands; l2++ )
			{
				nnet->setWeight( 0, l1+1, l2+1, 0 );
				land1 = map->getAllLands()[l1];
				land2 = map->getAllLands()[l2];
				if( land1->getNeighbors().indexOf( land2 ) != -1 )
				{
					nnet->setWeight( 0, l1+1, l2+1, ((double)qrand()/RAND_MAX*4)-2 );
				}
			}
	}

	player->setName( player->getName() + "(" + QString::number( myTier ) + ")" );
}

void RiskAI_NeuroTitan::gatherInputVector( SVector &input )
{
	input.changeSize( map->getAllLands().size()+1);
	for( int l = 0; l<map->getAllLands().size(); l++ )
	{
		RiskMapLand *land = map->getAllLands()[l];
		input[l+1] = land->getArmys();
		if( land->getOwner() != player->getPlayerId() )
		{
			input[l+1] = input[l+1] * -1;
		}
	}
}

int RiskAI_NeuroTitan::findNumberOfTiers()
{
	return( dir.entryList( QStringList("Tier*"), QDir::NoDotAndDotDot | QDir::AllDirs, QDir::Name ).size() );
}

int RiskAI_NeuroTitan::findNumberOfGenomsInTier( int tier )
{
	QDir tierDir( dir );
	tierDir.cd( dir.entryList( QStringList("Tier*"), QDir::NoDotAndDotDot | QDir::AllDirs, QDir::Name )[tier]);
	return( tierDir.entryList( QDir::NoDotAndDotDot | QDir::Files, QDir::Name ).size() );
}

QList<double> RiskAI_NeuroTitan::loadGenom( int tier, int index )
{
	QDir tierDir( dir );
	tierDir.cd( dir.entryList( QStringList("Tier*"), QDir::NoDotAndDotDot | QDir::AllDirs, QDir::Name )[tier] );
	//tierDir.cd( "Tier"+ QString::number( tier ) );
	QStringList entry = tierDir.entryList( QDir::NoDotAndDotDot | QDir::Files, QDir::Name );

	QList<double> genom;
	if( entry.size() > index )
	{
		QFile file( tierDir.absolutePath() + QDir::separator() + entry[index] );
		file.open( QIODevice::ReadOnly | QIODevice::Text );
		QString content(file.readAll() );
		file.close();
		QStringList list = content.split( ";" );
		QString n;
		while( !list.isEmpty() ) 
		{
			n = list.takeFirst();
			double e = n.toDouble();
			genom.append( e );
		}
	} 
	return( genom );
}

void RiskAI_NeuroTitan::saveGenom( int tier, QList<double> genom )
{
	QDir tierDir = dir;
	tierDir.mkdir( "Tier"+ QString::number( tier ) );
	tierDir.cd( "Tier"+ QString::number( tier ) );
	
	QString filename;
	QFile file( filename );
	while( filename.isEmpty() || file.exists() )
	{
		filename = tierDir.absolutePath() + QDir::separator() + "Genom_" + QString::number( qrand() ) + ".txt";
		file.setFileName( filename );
	}
	file.open( QIODevice::WriteOnly | QIODevice::Text );

	QString conv;
	while( !genom.isEmpty() )
	{
		conv += QString::number((double)genom.takeFirst()) +";";
	}
	file.write( conv.toUtf8() );
	file.flush();
	file.close();
}

QList<double> RiskAI_NeuroTitan::crossGenoms( int tier, int g1, int g2 )
{
	QList<double> result;
	QList<double> gen1 = loadGenom( tier, g1 );
	QList<double> gen2 = loadGenom( tier, g2 );

	int size = ((double)qrand() / RAND_MAX*(gen1.size()));
	for( int i=0; i<gen1.size(); i++ )
	{
		if( size < i ) result.append( gen1[i] );
		else result.append( gen2[i] );
	}
	return( result );
}

void RiskAI_NeuroTitan::mutateGenom( QList<double> &genom )
{
	for( int i=3; i<genom.size(); i++ )
	{
		double p = ((double)qrand() / RAND_MAX*(100));

		if( p < 2 )
		{
			if( p < 1 ) genom[i]+=0.1;
			else genom[i]-=0.1;
		}
	}
}
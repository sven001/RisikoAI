#include "RiskGameLogic.h"

RiskGameLogic::RiskGameLogic(void) : QObject( NULL )
{
	map = NULL;
	cardDeck = NULL;
	cardExchangeCounter = 0;
}

RiskGameLogic::~RiskGameLogic(void)
{
}

void RiskGameLogic::init( RiskMap *map, RiskCardDeck *cardDeck, MOVEOPTION opt, CARDEXCHANGEOPTION cardExchangeOption )
{
	this->map = map;
	this->cardDeck = cardDeck;
	this->moveOption = opt;
	this->cardExchangeOption = cardExchangeOption;
	notNeutralLandWasTaken = false;
	cardExchangeCounter = 0;
	armysFreeForCurrentPlayer = 0;
}

RISK_ERROR RiskGameLogic::attack( Player *attacker, RiskMapLand *att, RiskMapLand *def, int max, QString *logGUI )
{
	//Test if attackable
	if( att == NULL || def == NULL || max == 0 ) return( ARGUMENT_WRONG ); 
	if( attacker == NULL ) return( ARGUMENT_WRONG );

	if( att->getOwner() != attacker->getPlayerId() ) return( PLAYER_NOT_ALLOWED );

	if( att->getNeighbors().indexOf( def ) == -1 )
	{
		qWarning( "Can only attack neighbors" );
		return( TARGET_WRONG );
	}

	if( att->getOwner() == def->getOwner() )
	{
		qWarning( "Can not attack yourself" );
		return( TARGET_WRONG );
	}

	if( max > (att->getArmys() - 1) )
	{
		qWarning( "Not enough armys" );
		return( NOT_ENOUGH_ARMYS );
	}

	int moveMax = max;
	if( max > 3 ) max = 3;
	
	int attDice[3] = {0,0,0};
	int defDice[2] = {0,0};
	for( int i=0; i<max; i++ )
	{
		attDice[i] = ((double)qrand()*6/RAND_MAX)+1;
	}
	defDice[0] = ((double)qrand()*6/RAND_MAX)+1;
	if( def->getArmys() > 1 ) defDice[1] = ((double)qrand()*6/RAND_MAX)+1;

	//qWarning( "Att [%d][%d][%d] <-> Def[%d][%d]", attDice[0], attDice[1], attDice[2], defDice[0], defDice[1] );

	//sort dices
	if( defDice[1] > defDice[0] )
	{
		int t = defDice[0];
		defDice[0] = defDice[1];
		defDice[1] = t;
	}

	if( attDice[1] > attDice[0] )
	{
		int t = attDice[0];
		attDice[0] = attDice[1];
		attDice[1] = t;
	}
	if( attDice[2] > attDice[0] )
	{
		int t = attDice[0];
		attDice[0] = attDice[2];
		attDice[2] = t;
	}
	if( attDice[2] > attDice[1] )
	{
		int t = attDice[1];
		attDice[1] = attDice[2];
		attDice[2] = t;
	}
	//qWarning( "Att [%d][%d][%d] <-> Def[%d][%d]", attDice[0], attDice[1], attDice[2], defDice[0], defDice[1] );

	int attLoss = 0; int defLoss = 0;
	if( attDice[0] > defDice[0] )
	{
		defLoss++;
	}
	else
	{
		attLoss++;
	}

	if( attDice[1] > 0 &&
		defDice[1] > 0 )
	{
		if(	attDice[1] > defDice[1] )
		{
			defLoss++;
		}
		else
		{
			attLoss++;
		}
	}
	QString log;
	if( logGUI )
	{
		log.clear();
		log.append( "Att " );
		for( int i=0; i<max; i++ )
		{
			log.append( " [" + QString::number( attDice[i] ) + "]" );
		}
		log.append( " Def ");
		log.append( " [" + QString::number( defDice[0] ) + "]" );
		if( defDice[1] != 0 && max > 1 ) log.append( " [" + QString::number( defDice[1] ) + "]" );
		
		log.append( "\nAtt [" + QString::number(attLoss) +"], Def [" + QString::number(defLoss) +"]\n" );

		*logGUI = log;
	}
	log = "";
	log.append( attacker->getName() + " greift " + def->getLandName() + " an. A " + QString::number( attLoss ) + " D " + QString::number( defLoss ) );
	emit signalWriteLog( log );

	att->setArmys( att->getArmys()-attLoss );
	max -= attLoss;
	moveMax -= attLoss;
	def->setArmys( def->getArmys()-defLoss );

	if( def->getArmys() == 0 )
	{
		int oldOwner = def->getOwner();
		//Land taken
		def->setArmys( moveMax );
		att->setArmys( att->getArmys() - moveMax );
		def->setOwner( att->getOwner() );

		log = attacker->getName() + " erobert " + def->getLandName();
		emit signalWriteLog( log );

		notNeutralLandWasTaken = true;
	}

	
	return( OK );
}

RISK_ERROR RiskGameLogic::moveArmy( Player *mover, RiskMapLand *from, RiskMapLand *to, int number )
{
	//Test if moveable
	if( from == NULL || to == NULL ) return( ARGUMENT_WRONG ); 
	if( mover == NULL ) return( ARGUMENT_WRONG );
	if( !mover->isHuman() && number == 0 ) return( ARGUMENT_WRONG );

	if( from->getOwner() != mover->getPlayerId() ||
		to->getOwner() != mover->getPlayerId() ) return( TARGET_WRONG );

	if( number == 0 ) return( NOT_ENOUGH_ARMYS );
	if( from->getArmys() <= number ) return( TOO_MUCH_ARMYS );

	if( canArmyBeMoved( from, to ) )
	{
		from->setArmys( from->getArmys() - number );
		to->setArmys( to->getArmys() + number );

		emit signalWriteLog( mover->getName() + " bewegt " + QString::number( number ) + " Armeen von " + from->getLandName() + " nach " + to->getLandName() );

		return( OK );
	}

	return( NO_PATH_FOUND );
}

bool RiskGameLogic::canArmyBeMoved( RiskMapLand *from, RiskMapLand *to )
{
	QList<RiskMapLand *> path;
	if( moveOption == ONLY_PATH )
	{
		path = map->findShortestPath( from, to, true );
	}
	else if( moveOption == ONLY_NEIGHBOR )
	{
		path = from->getNeighbors();
		if( path.indexOf( to ) == -1 )
		{
			path.clear();
		}
	}

	if( path.size() != 0 )
	{
		path.clear();
		return( true );
	}
	return( false );
}

RISK_ERROR RiskGameLogic::exchangeCards( Player *player, RiskCard *c1, RiskCard *c2, RiskCard *c3 )
{
	int addArmys = 0;

	if( cardExchangeHasOccured ) return( CARDS_ALREADY_EXCHANGED );

	if( player == NULL || c1 == NULL || c2 == NULL || c3 == NULL ) return( ARGUMENT_WRONG );
	
	if( !player->getHand()->hasCard( c1 ) ) return( CARD_NOT_AVAILABLE );
	if( !player->getHand()->hasCard( c2 ) ) return( CARD_NOT_AVAILABLE );
	if( !player->getHand()->hasCard( c3 ) ) return( CARD_NOT_AVAILABLE );

	addArmys = getArmyUpByExchange( c1->getCardName(), c2->getCardName(), c3->getCardName() );

	if( addArmys == 0 ) return( NO_SET );

	if( player->getHand()->returnCard( c1, cardDeck ) &&
		player->getHand()->returnCard( c2, cardDeck ) &&
		player->getHand()->returnCard( c3, cardDeck ) )
	{
		armysFreeForCurrentPlayer += addArmys;
		cardExchangeCounter++;

		emit signalWriteLog( player->getName() + " tauscht Karten ein und erhält " + QString::number( addArmys ) + " Armeen. Er hat jetzt " +  QString::number( armysFreeForCurrentPlayer ) );
		return( OK );
	}
	return( ARGUMENT_WRONG );
}

RISK_ERROR RiskGameLogic::setArmy( Player *player, RiskMapLand *to, int number )
{
	if( player == NULL || to == NULL || number == 0 ) return( ARGUMENT_WRONG );

	if( this->armysFreeForCurrentPlayer < number ) return( NOT_ENOUGH_FREE_ARMYS );
	
	if( to->getOwner() != player->getPlayerId() ||
		(to->getOwner() == 0 && to->getArmys() > 0 ))  return( TARGET_WRONG );


	emit signalWriteLog( player->getName() + " setzt " + QString::number( number ) + " Armeen nach "+ to->getLandName() );
	to->setArmys( to->getArmys()+number );
	to->setOwner( player->getPlayerId() );
	armysFreeForCurrentPlayer -= number;
	
	return( OK );
}

RISK_ERROR RiskGameLogic::mayEndTurn( Player *player, GAMEPHASE currentGamephase, int currentPlayerId )
{
	if( player == NULL ) return( ARGUMENT_WRONG );

	if( player->getPlayerId() != currentPlayerId ) return( PLAYER_NOT_ALLOWED );

	switch( currentGamephase )
	{
	case INIT: return( OK ); break;
	case ADD_ARMYS:
		{
			if( armysFreeForCurrentPlayer != 0 ) return( TOO_MUCH_ARMYS );
			if( player->getHand()->getNumberOfCards() >= 5 ) return( TOO_MUCH_CARDS );
			return( OK );
		}
		break;
	case ATTACK:
		{
			return( OK );
		}
	case MOVE:
		{
			return( OK );
		}
	}

	return( ARGUMENT_WRONG );	
}

RISK_ERROR RiskGameLogic::addArmyByTurn( Player *player )
{
	if( player == NULL ) return( ARGUMENT_WRONG );

	resetArmyCount();

	armysFreeForCurrentPlayer = getAddArmyCountForPlayer( player );

	emit signalWriteLog( player->getName() + " erhält " + QString::number( armysFreeForCurrentPlayer ) + " Armeen" );

	return( OK );
}

int RiskGameLogic::getAddArmyCountForPlayer( Player *player )
{
	int count = 0;

	QList<RiskMapLand *> list = map->getAllLands();
	for( int i=0; i<list.size(); i++ )
	{
		if( list[i]->getOwner() == player->getPlayerId() ) count++;
	}
	if( count == 0 ) 
	{
		return( 0 ); //Player is lost...	
	}
	count /= 3;
	if( count < 3 ) count = 3;

	//Check conts
	QList<RiskMapContinent *> contiList = map->getAllContinents();
	for( int i=0; i<contiList.size(); i++ )
	{
		if( player->getPlayerId() == contiList[i]->isOwnedByPlayer() )
		{
			count += contiList[i]->getArmyBonus();
		}
	}

	return( count );
}

int RiskGameLogic::getArmyUpByExchange( QString c1, QString c2, QString c3 )
{
	int armyUp = 0;
	switch( cardExchangeOption )
	{
	case FOREVER_FIVE:
		armyUp = 5;
		break;
	case ASCENDING_BY_TIME:
		if( cardExchangeCounter == 0 ) armyUp = 4;
		if( cardExchangeCounter == 1 ) armyUp = 6;
		if( cardExchangeCounter == 2 ) armyUp = 8;
		if( cardExchangeCounter == 3 ) armyUp = 10;
		if( cardExchangeCounter == 4 ) armyUp = 12;
		if( cardExchangeCounter == 5 ) armyUp = 15;
		if( cardExchangeCounter > 5 ) armyUp = 15 + ((cardExchangeCounter-5) * 5);
		break;
	case BY_SYMBOL:
		QChar c[3];
		c[0] = c1.at(0);
		c[1] = c2.at(0);
		c[2] = c3.at(0);

		if( (c[0] == c[1]) && c[0] == 'A' ) armyUp = 4;
		if( (c[0] == c[1]) && c[0] == 'I' ) armyUp = 6;
		if( (c[0] == c[1]) && c[0] == 'C' ) armyUp = 8;

		if( (c[0] != c[1]) && (c[0] != c[2]) && (c[1] != c[2]) ) armyUp = 10;
		break;
	}
	return( armyUp );
}
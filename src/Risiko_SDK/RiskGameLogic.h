#pragma once

#include <QObject>

#include "Player.h"
#include "RiskMap.h"
#include "RiskMapLand.h"
#include "RiskCardDeck.h"

class RiskGameLogic : public QObject
{
	Q_OBJECT
	friend class RiskFlowControl;

public:
	//Action
	RISK_ERROR attack( Player *attacker, RiskMapLand *att, RiskMapLand *def, int max, QString *log = NULL );
	RISK_ERROR moveArmy( Player *mover, RiskMapLand *from, RiskMapLand *to, int number );
	RISK_ERROR exchangeCards( Player *player, RiskCard *c1, RiskCard *c2, RiskCard *c3 );
	RISK_ERROR setArmy( Player *player, RiskMapLand *to, int number );

	//Information
	bool canArmyBeMoved( RiskMapLand *from, RiskMapLand *to );
	RISK_ERROR turnMayBeEnded();
	bool cardMayBeDrawn() {return( notNeutralLandWasTaken );};
	bool cardsMayBeExchanged()  {return( !cardExchangeHasOccured );};
	int getCurrentFreeArmys() { return(armysFreeForCurrentPlayer);};
	int getArmyUpByExchange( QString c1, QString c2, QString c3 );
	int getAddArmyCountForPlayer( Player *player );

signals:
	void signalWriteLog( QString, bool = false );

protected:
	RiskGameLogic(void);
	~RiskGameLogic(void);

	void init( RiskMap *map, RiskCardDeck *cardDeck, MOVEOPTION opt, CARDEXCHANGEOPTION cardExchangeOption);
	void resetArmyCount() {armysFreeForCurrentPlayer = 0;};
	void resetTurnMarker() { notNeutralLandWasTaken=false; cardExchangeHasOccured = false;};

	RISK_ERROR addArmyByTurn( Player *player );
	RISK_ERROR mayEndTurn( Player *player, GAMEPHASE currentGamephase, int currentPlayerId );

	RiskMap *map;
	RiskCardDeck *cardDeck;

	int armysFreeForCurrentPlayer;
	int cardExchangeCounter;	//Anzahl bereits durchgeführter Kartentauschaktionen
	bool notNeutralLandWasTaken;// Wurde diese Runde ein NICHT NEUTRALES Land erobert?
	bool cardExchangeHasOccured;//Diese Runde wurden schon Karten eingetauscht
	MOVEOPTION moveOption;
	CARDEXCHANGEOPTION cardExchangeOption;

};

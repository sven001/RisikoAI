#ifndef RISKFLOWCONTROL_H
#define RISKFLOWCONTROL_H

#include <QObject>
#include "RiskMap.h"
#include "RiskCard.h"
#include "RiskCardDeck.h"
#include "RiskMapLand.h"
#include "Player.h"
#include "RiskGameLogic.h"

class RiskFlowControl : public QObject
{
	Q_OBJECT

public:
	RiskFlowControl();
	~RiskFlowControl();

	//GUI Anbindung
	RiskMap *getMap() {return( &map );};
	QString getCurPlayer();
	QString getCurPhase();
	RiskCardDeck *getCurCards();
	bool silentMode;

	//Spielinitialisierung
	void clear();
	void loadMap();
	void addPlayer( Player *player );


	void initGame( bool randomStart, CARDEXCHANGEOPTION option, MOVEOPTION moveOption );

	bool curPlayerIs( int i );
	bool curPhaseIs( GAMEPHASE i );
	int curAddArmyCount() { return(gameLogic.armysFreeForCurrentPlayer );};
	int curPlayerId() { return( players[currentPlayer]->getPlayerId() );};
	Player *getWinner();

	bool forceEndGame;
public slots:
	//Befehle von der GUI
	void slotNextPhase();
	void slotLandSelected( RiskMapLand * );
	void slotAddArmyByGUI( RiskMapLand * );
	void slotAttackByGUI( RiskMapLand *att, RiskMapLand *def );
	void slotMoveByGUI( RiskMapLand *from, RiskMapLand *to );
	void slotExchangeCardByGUI( QString c1, QString c2, QString c3 );

	void addLog( QString, bool = false );
	
signals:
	//Befehle zu der GUI
	void signalUpdateGUI();
	void signalEnablePlayer( bool onOff );
	void signalSelectLand( RiskMapLand * );
	void signalAddArmyByExchange();
	void signalForceSceneUpdate();

	void signalWriteLog( QString );

private:
	//Spiellogik
	RiskGameLogic gameLogic;
	RiskMap map;
	RiskCardDeck cardDeck;
	QList<Player *> players;

	//FlowControl
	int currentPlayer;
	GAMEPHASE currentPhase;

	//Regeloptionen
	bool currentPlayerIsHuman;
};

#endif // RISKFLOWCONTROL_H

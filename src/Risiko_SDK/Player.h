#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "RiskCard.h"
#include "RiskCardDeck.h"

class Player : public QObject
{
	friend class RiskFlowControl;
	Q_OBJECT

public:
	Player(QObject *parent = NULL);
	virtual ~Player();
	
	QString getName();
	void setName( QString name );

	int getPlayerId() { return( myId );};
	virtual bool isHuman() = 0;
	
	RiskCardDeck *getHand() { return( &hand ); };

	virtual bool yourTurn( GAMEPHASE phase ) { return( false ); };
protected:
	
	void drawCard( RiskCardDeck* cardDeck ) {hand.drawCard( cardDeck ); };
	bool isDefeated() { return( defeatedMarker ); };
	void defeated() {defeatedMarker = true; };

	int myId;
private:
	QString name;
	bool defeatedMarker;
	RiskCardDeck hand;
};

#endif // PLAYER_H

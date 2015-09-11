#ifndef RISKAI_H
#define RISKAI_H

#include <QObject>
#include "Player.h"
#include "RiskMap.h"
#include "RiskGameLogic.h"

class RiskAI : public QObject
{
	Q_OBJECT

public:
	RiskAI();
	~RiskAI();
	void initAI( Player *player, RiskMap *map, RiskGameLogic *logic );

	virtual bool yourTurn( GAMEPHASE phase ) = 0;
signals:
	void endTurn();

protected:
	Player *player;
	RiskMap *map;
	RiskGameLogic *logic;

};

#endif // RISKAI_H

#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
#include "RiskAI.h"
#include <QLibrary>
//#include <Windows.h>

class PlayerAI : public Player
{
	friend class RiskFlowControl;
	Q_OBJECT

public:
	PlayerAI( QString dllName );
	~PlayerAI();

	bool isHuman() { return( false );};
	bool yourTurn( GAMEPHASE phase) { return( myAI->yourTurn( phase )); };
private:
	QLibrary dll;

	RiskAI *myAI;
};

#endif // PLAYERAI_H

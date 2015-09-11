#ifndef PLAYERHUMAN_H
#define PLAYERHUMAN_H

#include "Player.h"

class PlayerHuman : public Player
{
	Q_OBJECT

public:
	PlayerHuman(QObject *parent = NULL);
	~PlayerHuman();

	bool isHuman() { return( true );};

private:
	
};

#endif // PLAYERHUMAN_H

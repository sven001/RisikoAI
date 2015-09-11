#ifndef PLAYERNEUTRAL_H
#define PLAYERNEUTRAL_H

#include "Player.h"

class PlayerNeutral : public Player
{
	Q_OBJECT

public:
	PlayerNeutral(QObject *parent = NULL);
	~PlayerNeutral();

	bool isHuman() { return( false );};
private:
	
};

#endif // PLAYERNEUTRAL_H

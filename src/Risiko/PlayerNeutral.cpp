#include "stdafx.h"
#include "PlayerNeutral.h"

PlayerNeutral::PlayerNeutral(QObject *parent)
	: Player(parent)
{
	setName( "Neutral" );
}

PlayerNeutral::~PlayerNeutral()
{

}

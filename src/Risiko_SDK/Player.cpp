#include "Player.h"

Player::Player(QObject *parent)
	: QObject(parent)
{
	defeatedMarker = false;
}

Player::~Player()
{

}

void Player::setName( QString name )
{
	this->name = name;
}
	
QString Player::getName()
{
	return( name );
}
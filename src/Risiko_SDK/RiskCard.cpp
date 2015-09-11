#include "RiskCard.h"

RiskCard::RiskCard(RiskMapLand *land)
	: QObject(NULL)
{
	myLand = land;

	symbol = (CARDSYMBOL)((int)land->getLandId() % 3);
}

RiskCard::~RiskCard()
{

}

CARDSYMBOL RiskCard::getSymbol()
{
	return( symbol );
}

RiskMapLand *RiskCard::getLand()
{
	return( myLand );
}

QString RiskCard::getCardName()
{
	QString ret = "";
	switch( getSymbol() )
	{
	case INFANTRY: ret = "I-"; break;
	case CAVALRY: ret = "C-"; break;
	case ARTILLERY: ret = "A-"; break;
	}
	ret.append( getLand()->getLandName() );
	return( ret );
}
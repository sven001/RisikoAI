#ifndef RISKCARD_H
#define RISKCARD_H

#include <QObject>
#include "RiskMapLand.h"
#include "RiskDefines.h"

class RiskCard : public QObject
{
	Q_OBJECT

public:
	RiskCard(RiskMapLand *land);
	~RiskCard();

	CARDSYMBOL getSymbol();
	RiskMapLand *getLand();
	QString getCardName();

private:
	RiskMapLand *myLand;
	CARDSYMBOL symbol;
	
};

#endif // RISKCARD_H

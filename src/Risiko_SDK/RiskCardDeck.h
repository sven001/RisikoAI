#ifndef RISKCARDDECK_H
#define RISKCARDDECK_H

#include <QObject>
#include "RiskCard.h"
#include "RiskMapLand.h"

class RiskCardDeck : public QObject
{
	Q_OBJECT

public:
	RiskCardDeck(QObject *parent = NULL);
	~RiskCardDeck();

	void create( QList<RiskMapLand*> lands );
	void clear();

	int getNumberOfCards() { return( cards.size() );};
	QString getCardName( int index );

	void drawCard( RiskCardDeck *fromDeck );
	RiskCard *findCard( QString card );
	bool hasCard( RiskCard *card );
	bool returnCard( RiskCard *card, RiskCardDeck *toDeck );
	bool fillMatching( RiskCard *card[3], CARDSYMBOL symbol );
	bool findSet( RiskCard *card[3] );

private:
	QList<RiskCard *> cards;
	RiskCard *takeFirstCard();
	void appendCard( RiskCard *card );
		
};

#endif // RISKCARDDECK_H

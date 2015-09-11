#include "RiskCardDeck.h"

RiskCardDeck::RiskCardDeck(QObject *parent)
	: QObject(parent)
{

}

RiskCardDeck::~RiskCardDeck()
{
	clear();
}

void RiskCardDeck::create( QList<RiskMapLand*> lands )
{
	clear();
	for( int i=0; i<lands.size(); i++ )
	{
		cards.append( new RiskCard( lands[i] ) );
	}

	//Shuffle
	for( int i = 0; i<cards.size(); i++ )
	{
		int fromIndex = ((double)(qrand())* (cards.size()-1) / RAND_MAX) ;
		int toIndex =  ((double)(qrand())* (cards.size()-2) / RAND_MAX);

		RiskCard *c = cards.takeAt( fromIndex );
		cards.insert( toIndex, c );
	}
	
}

void RiskCardDeck::clear()
{
	while( !cards.isEmpty() ) delete( cards.takeFirst() );
}

QString RiskCardDeck::getCardName( int index )
{
	RiskCard *card = cards.at( index );

	QString ret = "";

	if( card ) ret = card->getCardName();

	return( ret );
}

void RiskCardDeck::drawCard( RiskCardDeck *fromDeck )
{
	cards.append( fromDeck->takeFirstCard() );
}

RiskCard *RiskCardDeck::findCard( QString card )
{
	for( int i=0; i<cards.size(); i++ )
	{
		if( card.compare( getCardName( i ) ) == 0 )
		{
			return( cards[i] );
		}
	}
	return( NULL );
}

bool RiskCardDeck::hasCard( RiskCard *card )
{
	return( cards.indexOf( card ) != -1 );
}

bool RiskCardDeck::returnCard( RiskCard *card, RiskCardDeck *toDeck )
{
	int index = cards.indexOf( card );
	if( index != -1 )
	{
		toDeck->appendCard( cards.takeAt( index ) );
		return( true );
	}
	return( false );
}


RiskCard *RiskCardDeck::takeFirstCard()
{
	return( cards.takeFirst() );
}

void RiskCardDeck::appendCard( RiskCard *card )
{
	cards.append( card );
}

bool RiskCardDeck::fillMatching( RiskCard *card[3], CARDSYMBOL symbol )
{
	int index = 0;
	for( int i=0; i<3; i++ ) card[i] = NULL;
	for( int i=0; i<cards.size(); i++ )
	{
		if( cards[i]->getSymbol() == symbol )
		{
			if( index < 3 )
			{
				card[index] = cards[i];
				index++;
			}
		}
	}
	if( index == 3 ) return( true );
	return( false );
}

bool RiskCardDeck::findSet( RiskCard *card[3] )
{
	RiskCard *a, *c, *i;
	if( fillMatching( card, ARTILLERY ) ) return( true );
	a = card[0];
	if( fillMatching( card, CAVALRY ) ) return( true );
	c = card[0];
	if( fillMatching( card, INFANTRY ) ) return( true );
	i = card[0];

	if( a != NULL && c != NULL && i != NULL ) 
	{
		card[0] = a;
		card[1] = c;
		card[2] = i;
		return( true );
	}

	for( int i=0; i<3; i++ ) card[i] = NULL;
	return( false );
}
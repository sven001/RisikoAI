#include "stdafx.h"
#include "RiskFlowControl.h"

#include "PlayerHuman.h"
#include "PlayerAI.h"
#include "DialogAttack.h"
#include "DialogMove.h"

#include <qmessagebox.h>

RiskFlowControl::RiskFlowControl()
	: QObject(NULL)
{
	silentMode = false;
	connect( &gameLogic, SIGNAL( signalWriteLog( QString, bool ) ), this, SLOT( addLog( QString, bool ) ));
	forceEndGame = false;
	clear();
}

RiskFlowControl::~RiskFlowControl()
{
	clear();
}

void RiskFlowControl::clear()
{
	currentPhase = ADD_ARMYS;
	currentPlayer = 1;
	map.clear();
	cardDeck.clear();

	while( players.size() > 0 ) delete( players.takeFirst() );
}

void RiskFlowControl::loadMap()
{
	//Load Map	
	map.loadTestMap();
	
	//Load Cards
	cardDeck.create( map.getAllLands() );


}

void RiskFlowControl::addPlayer( Player *player )
{
	players.append( player );
	player->myId = players.size()-1;

	PlayerAI *ai = qobject_cast<PlayerAI*>( player );
	if( ai )
	{
		ai->myAI->initAI( ai, &map, &gameLogic );
	}
}

void RiskFlowControl::initGame( bool randomStart, CARDEXCHANGEOPTION option, MOVEOPTION moveOption )
{
	currentPhase = INIT;
	currentPlayer = 1;
	
	if( randomStart )
	{
		// Alle Länder zufällig verteilen (ausser neutraler Spieler) 
		// Startspieler kann auch NICHT 1 sein
		QList<RiskMapLand *> list = map.getAllLands();
		while( !list.isEmpty() )
		{
			//Pick random
			int index = (int)((double)(qrand()) / RAND_MAX * (list.size()-1));
			RiskMapLand *land = list.takeAt( index );
			land->setArmys( 1 );
			land->setOwner( currentPlayer );
			currentPlayer++;
			if( currentPlayer >= players.size() ) currentPlayer = 1;
		}	
	}
	gameLogic.init( &map, &cardDeck, moveOption, option );

	addLog( "Spiel gestartet", true );
	slotNextPhase();
}

void RiskFlowControl::slotNextPhase()
{
	if( gameLogic.mayEndTurn( players[currentPlayer], currentPhase, currentPlayer ) != OK )
	{
		addLog( "Zug kann nicht beendet werden.", true );
		return;
	}

	do
	{
		qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
		emit signalForceSceneUpdate();
		//Check looser
		for( int i=0; i<players.size(); i++ )
		{
			if( !players[i]->isDefeated() &&
				!map.playerHasLands( i ) &&
				i != 0 )
			{
				players[i]->defeated();
				//Return cards to deck
				while( players[i]->getHand()->getNumberOfCards() > 0 ) cardDeck.drawCard( players[i]->getHand() );
				addLog( players[i]->getName() +" hat kein Land mehr.", true );
			}
		}
		//Check winner
		for( int i=0; i<players.size(); i++ )
		{
			if( !players[i]->isDefeated() &&
				map.playerHasAllLands( i ) &&
				i != 0 )
			{
				QString log = players[i]->getName() +" hat die Erde erobert!";
				addLog( log, true );
				if( !silentMode )
				{
					QMessageBox qm( "Gewonnen", players[i]->getName() +" hat die Erde erobert!", QMessageBox::NoIcon, 0, 0, 0 );
					qm.exec();
				}
				emit signalEnablePlayer( false );
				return;
			}
		}

		if( currentPhase == INIT ) currentPhase = ADD_ARMYS;
		else if( currentPhase == ADD_ARMYS ) currentPhase = ATTACK;
		else if( currentPhase == ATTACK ) currentPhase = MOVE;
		else if( currentPhase == MOVE )
		{
			currentPhase = ADD_ARMYS;
			
			do{
				currentPlayer++;
				if( currentPlayer >= players.size() ) currentPlayer = 1;	//Player 0 is neutral
			} while( players[currentPlayer]->isDefeated() );
		}
		
		currentPlayerIsHuman = players.at(currentPlayer)->isHuman();

		if( curPhaseIs( ADD_ARMYS ) )
		{
			gameLogic.addArmyByTurn( players.at(currentPlayer) );
		}
		else
		{
			gameLogic.resetArmyCount();
		}

		//attack is done...
		if( curPhaseIs( MOVE ) )
		{
			if( gameLogic.cardMayBeDrawn() )
			{
				players[currentPlayer]->drawCard( &cardDeck );
			}
		}

		gameLogic.resetTurnMarker();

		emit signalUpdateGUI();
		emit signalEnablePlayer( currentPlayerIsHuman );

/*		if( !currentPlayerIsHuman )
		{
			QMessageBox qm1( "TitanNet", "Next Turn", QMessageBox::NoIcon, 0, 0, 0 );
			qm1.exec();
		}
*/
	} while( !forceEndGame &&
		     players[currentPlayer]->yourTurn( currentPhase ) ); //true if AI has played
}

void RiskFlowControl::slotLandSelected( RiskMapLand *sel )
{
	if( currentPlayerIsHuman )
	{
		emit signalSelectLand( sel );
	}
}

void RiskFlowControl::slotAddArmyByGUI( RiskMapLand *sel )
{
	gameLogic.setArmy( players[currentPlayer], sel, 1 );
}

void RiskFlowControl::slotAttackByGUI( RiskMapLand *att, RiskMapLand *def )
{
	if( att->getNeighbors().indexOf( def ) == -1 ) return;

	DialogAttack da( players.at( currentPlayer ), &gameLogic, att, def );
	da.exec();
}

void RiskFlowControl::slotMoveByGUI( RiskMapLand *from, RiskMapLand *to )
{
	if( from->getOwner() == to->getOwner() &&
		from->getOwner() == currentPlayer &&
		gameLogic.canArmyBeMoved( from, to ) )
	{
		DialogMove dm( &map, from, to );
		if( dm.exec() == QDialog::Accepted )
		{
			if( OK == gameLogic.moveArmy( players.at( currentPlayer ), from, to, dm.getNumberOfArmys() ) )
			{
			slotNextPhase();
			}
		}
	}
}

void RiskFlowControl::slotExchangeCardByGUI( QString c1, QString c2, QString c3 )
{
	RiskCard *pc1 = getCurCards()->findCard( c1 );
	RiskCard *pc2 = getCurCards()->findCard( c2 );
	RiskCard *pc3 = getCurCards()->findCard( c3 );

	if( OK == gameLogic.exchangeCards( players.at( currentPlayer ), pc1, pc2, pc3 ) )
	{
		emit signalAddArmyByExchange();
	}
}

QString RiskFlowControl::getCurPlayer()
{
	QString ret = "Error";
	Player *p = players.at( currentPlayer );
	if( ret != NULL ) ret = p->getName();
	return( ret );
}

RiskCardDeck *RiskFlowControl::getCurCards()
{
	Player *p = players.at( currentPlayer );
	return( p->getHand() );

}

QString RiskFlowControl::getCurPhase()
{
	QString ret = "Error";
	if( currentPhase == ADD_ARMYS ) ret = "Verteile Armeen";
	if( currentPhase == ATTACK ) ret = "Angreifen";
	if( currentPhase == MOVE ) ret = "Bewegen";

	return( ret );
}

bool RiskFlowControl::curPlayerIs( int i )
{
	return( i == currentPlayer );
}

bool RiskFlowControl::curPhaseIs( GAMEPHASE i )
{
	return( i == currentPhase );
}

Player *RiskFlowControl::getWinner()
{
	Player *winner = NULL;
	for( int i=1; i<players.size(); i++ )
	{
		if( !players[i]->isDefeated() )
		{
			if( winner == NULL )
			{
				winner = players[i];
			}
			else
			{
				return( NULL );
			}
		}
	}
	return( winner );
}

void RiskFlowControl::addLog( QString log, bool overrideSilentMode )
{
	if( !silentMode || overrideSilentMode ) emit signalWriteLog( log );
}
#include "stdafx.h"
#include "risiko.h"
#include "RiskMap.h"
#include "RiskMapScene.h"
#include "PlayerHuman.h"
#include "PlayerNeutral.h"
#include "PlayerAI.h"
#include "DialogStartGame.h" 
#include "DialogStartAIContest.h"
#include "time.h"

#include <QMessageBox.h>

Risiko::Risiko(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	qsrand(clock());
	ui.setupUi(this);

	this->setWindowIcon( QIcon( ":/prgIcon" ) );

	this->addDockWidget( Qt::RightDockWidgetArea, &humanPlayerGUI );
	humanPlayerGUI.connectControl( flowControl );

	this->addDockWidget( Qt::BottomDockWidgetArea, &gameFlowGUI );
	connect( &flowControl, SIGNAL( signalWriteLog( QString ) ), &gameFlowGUI, SLOT( addLog( QString ) ));
	connect( &flowControl, SIGNAL( signalForceSceneUpdate() ), this, SLOT( forceSceneUpdate() ));
}

Risiko::~Risiko()
{

}

void Risiko::closeEvent( QCloseEvent * event )
{
	flowControl.forceEndGame = true;
	QMainWindow::closeEvent( event );
}


void Risiko::aiContest( void )
{
	DialogStartAIContest dsaic;
	if( dsaic.exec() != QDialog::Accepted ) return;

	CARDEXCHANGEOPTION cardOption = FOREVER_FIVE;
	MOVEOPTION moveOption = ONLY_PATH;

	QList<QStringList> games;
	QStringList ai;
	QList<int> points;
	QStringList ai_reverse;
	if( dsaic.comboBoxAI1->currentIndex() != 0 ) ai.append( dsaic.comboBoxAI1->currentText() );
	if( dsaic.comboBoxAI2->currentIndex() != 0 ) ai.append( dsaic.comboBoxAI2->currentText() );
	if( dsaic.comboBoxAI3->currentIndex() != 0 ) ai.append( dsaic.comboBoxAI3->currentText() );

	for( int i=0; i<ai.size(); i++ )
	{
		points << 0;
		ai_reverse.append( ai[ai.size()-1-i] );
	}

	for( int i=0; i<dsaic.spinBoxNumberOfIterations->value(); i++ )
	{
		// Store games
		if( dsaic.checkBoxOne->isChecked() )
		{
			//Init one of a kind
			games.append( ai );
			games.append( ai_reverse );
		}
		if( dsaic.checkBoxTwo->isChecked() )
		{
			//Init two of a kind
			games.append( ai + ai );
			games.append( ai_reverse + ai_reverse );
		}
		if( dsaic.checkBoxThree->isChecked() )
		{
			//Init three of a kind
			games.append( ai + ai + ai);
			games.append( ai_reverse + ai_reverse + ai_reverse);
		}
		if( dsaic.checkBoxOneAgainsThree->isChecked() )
		{
			for( int i=0; i<ai.size(); i++ )
			{	
				for( int j=0; j<ai.size(); j++ )
				{	
					if( i != j )
					{
						QStringList game;
						game.append( ai[i] );
						game.append( ai[j] );
						game.append( ai[j] );
						game.append( ai[j] );
						games.append( game );
					}
				}
			}
		}
	}
	Player *p;
	int gamecount = games.size();
	
	while( !games.isEmpty() )
	{
		QStringList game = games.takeFirst();

		ui.boardGraphicsView->setScene( NULL );		
		flowControl.clear();
		flowControl.loadMap();

		//Init GameType and Players
		flowControl.addPlayer( new PlayerNeutral() );

		int count = 1;
		while( !game.isEmpty() )
		{
			QString player = game.takeFirst();
			p = new PlayerAI( player + ".dll" );
			p->setName( player + " ("+ QString::number( count++ ) + ")" );
			flowControl.addPlayer( p );
		}
		//Build scene from map
		RiskMapScene *rms = new RiskMapScene( flowControl.getMap() );
		ui.boardGraphicsView->setScene( rms );
		rms->resize();
		connect( rms, SIGNAL( signalLandSelected( RiskMapLand * ) ), 
		     &flowControl, SLOT( slotLandSelected( RiskMapLand * )));
		
		flowControl.silentMode = true;
		flowControl.initGame( true, cardOption, moveOption );

		//Spiel zu Ende
		Player *winner = flowControl.getWinner();
		//Find AI
		for( int i=0; i<ai.size(); i++ )
		{
			if( winner &&
				winner->getName().indexOf( ai[i] ) == 0 )
			{
				points[i]+=1;
			}
		}
	}

	//Ausgabe
	QString winList;
	for( int i=0;i<ai.size(); i++ )
	{
		winList.append( ai[i] + " hat " + QString::number( points[i] ) + " mal gewonnen  (" + QString::number( (double)points[i]/gamecount * 100 ) + "%)\n" );
	}
	QMessageBox qm( "Info", winList, QMessageBox::NoIcon, 0,0,0 );
	qm.exec();
}

void Risiko::loadMap( void ) 
{
	ui.boardGraphicsView->setScene( NULL );

	flowControl.clear();
	flowControl.loadMap();
	
	//Init GameType and Players
	DialogStartGame dsg;
	if( dsg.exec() != QDialog::Accepted ) return;
	int nr = dsg.ui.spinBoxNumberOfPlayers->value();
	CARDEXCHANGEOPTION cardOption;
	if( dsg.ui.radioButtonCEO5->isChecked() ) cardOption = FOREVER_FIVE;
	if( dsg.ui.radioButtonCEO_ByTime->isChecked() ) cardOption = ASCENDING_BY_TIME;
	if( dsg.ui.radioButtonCEO_BySymbol->isChecked() ) cardOption = BY_SYMBOL;
	MOVEOPTION moveOption;
	if( dsg.ui.radioButtonOnlyNeighbors->isChecked() ) moveOption = ONLY_NEIGHBOR;
	if( dsg.ui.radioButtonOnlyPath->isChecked() ) moveOption = ONLY_PATH;
	bool randomStart = dsg.ui.radioButtonRandom->isChecked();

	flowControl.addPlayer( new PlayerNeutral() );

	int startPlayer = 0;
	Player *p;
	if( dsg.ui.checkBoxHuman->isChecked() )
	{
		p = new PlayerHuman();
		p->setName( "Silly Mortal" );
		flowControl.addPlayer( p );
		startPlayer = 1;
	}

	

	for( int i=startPlayer; i<nr; i++ )
	{
		p = new PlayerAI( "RiskAINeuroTitan.dll" );
		QString pName;
		pName.sprintf( "Player %d", i+1 );
		p->setName( pName );
		flowControl.addPlayer( p );
	}

	//Build scene from map
	RiskMapScene *rms = new RiskMapScene( flowControl.getMap() );
	ui.boardGraphicsView->setScene( rms );
	rms->resize();
	connect( rms, SIGNAL( signalLandSelected( RiskMapLand * ) ), 
		     &flowControl, SLOT( slotLandSelected( RiskMapLand * )));
	
	flowControl.silentMode = false;
	flowControl.initGame( randomStart, cardOption, moveOption );
}

void Risiko::forceSceneUpdate()
{
	QWidget* viewport = ui.boardGraphicsView->viewport();
	viewport->repaint();
}
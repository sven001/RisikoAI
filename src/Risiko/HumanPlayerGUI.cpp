#include "stdafx.h"
#include "HumanPlayerGUI.h"


HumanPlayerGUI::HumanPlayerGUI(QWidget *parent)
	: QDockWidget(parent)
{
	myControl = NULL;
	ui.setupUi(this);
	att = NULL;
	def = NULL;
}

HumanPlayerGUI::~HumanPlayerGUI()
{

}

void HumanPlayerGUI::connectControl( RiskFlowControl &flowControl )
{
	myControl = &flowControl;
	connect( ui.pushButtonNextPhase, SIGNAL( clicked() ), 
		     &flowControl, SLOT( slotNextPhase() ));
	connect( &flowControl, SIGNAL( signalUpdateGUI() ), 
		     this, SLOT( update() ));
	connect( &flowControl, SIGNAL( signalEnablePlayer( bool ) ), 
		     this, SLOT( enablePlayer( bool ) ));
	connect( &flowControl, SIGNAL( signalSelectLand( RiskMapLand * ) ), 
		     this, SLOT( selectLand( RiskMapLand * ) ));
	connect( this, SIGNAL( signalAttackByGUI( RiskMapLand *, RiskMapLand * ) ), 
		     &flowControl, SLOT( slotAttackByGUI( RiskMapLand *, RiskMapLand * ) ));
	connect( this, SIGNAL( signalAddArmyByGUI( RiskMapLand * ) ), 
		     &flowControl, SLOT( slotAddArmyByGUI( RiskMapLand * ) ));
	connect( this, SIGNAL( signalMoveByGUI( RiskMapLand *, RiskMapLand * ) ), 
		     &flowControl, SLOT( slotMoveByGUI( RiskMapLand *, RiskMapLand * ) ));
	connect( this, SIGNAL( signalExchangeCardByGUI( QString, QString, QString ) ), 
		     &flowControl, SLOT( slotExchangeCardByGUI( QString, QString, QString ) ));
	connect( &flowControl, SIGNAL( signalAddArmyByExchange() ), 
		     this, SLOT( slotAddArmyByExchange() ));

	
}

void HumanPlayerGUI::update()
{
	Qt::GlobalColor col= Qt::lightGray;
	col = (Qt::GlobalColor)((int)(col)+myControl->curPlayerId() );
	QPalette pal = ui.labelPlayer->palette();
	pal.setColor( QPalette::Background, col ); 
	ui.labelPlayer->setPalette( pal );
	
	ui.labelPlayer->setText( myControl->getCurPlayer() );
	ui.labelPhase->setText( myControl->getCurPhase() );
}

void HumanPlayerGUI::enablePlayer( bool onOff )
{
	att = NULL;
	def = NULL;

	RiskCardDeck *deck = myControl->getCurCards();
	ui.listWidgetCards->clear();
	for( int i=0; i<deck->getNumberOfCards(); i++ )
	{
		ui.listWidgetCards->addItem( deck->getCardName( i ) );
	}

	if( myControl->curPhaseIs( ADD_ARMYS ) )
	{
		ui.labelAtt->setVisible( false );
		ui.labelDef->setVisible( false );
		ui.label1->setVisible( false );
		ui.label2->setVisible( false );
		ui.label3->setVisible( onOff );
		ui.label4->setVisible( false );
		ui.lineEditArmys->setVisible( onOff );
		ui.lineEditArmys->setText( QString::number( myControl->curAddArmyCount() ) );
		slotCardSelectionChanged();
		
		if( ui.lineEditArmys->text() == "0" )ui.pushButtonNextPhase->setEnabled( onOff  );
		else ui.pushButtonNextPhase->setEnabled( false);
	}
	if( myControl->curPhaseIs( ATTACK ) )
	{
		ui.pushButtonNextPhase->setEnabled( onOff );
		ui.labelAtt->setVisible( onOff );
		ui.labelDef->setVisible( onOff );
		ui.label1->setVisible( onOff );
		ui.label2->setVisible( onOff );
		ui.label4->setVisible( false );
		ui.label3->setVisible( false );
		ui.lineEditArmys->setVisible( false );
	}
	if( myControl->curPhaseIs( MOVE ) )
	{
		ui.pushButtonNextPhase->setEnabled( onOff );
		ui.labelAtt->setVisible( onOff );
		ui.labelDef->setVisible( false );
		ui.label1->setVisible( false );
		ui.label2->setVisible( false );
		ui.label4->setVisible( onOff );
		ui.label3->setVisible( false );
		ui.lineEditArmys->setVisible( false );
	}
	selectLand( NULL );
}

void HumanPlayerGUI::selectLand( RiskMapLand *sel )
{
	if( sel == NULL )
	{
		att = NULL;
		def = NULL;
		ui.labelAtt->setText( "" );
		ui.labelDef->setText( "" );
		return;
	}

	//AddArmy Phase
	if( myControl->curPhaseIs( ADD_ARMYS ) )
	{
		if( myControl->curPlayerIs( sel->getOwner() ) ||
			(sel->getOwner() == 0 && sel->getArmys() == 0 ))
		{
			QString t= ui.lineEditArmys->text();
			int left = t.toInt();

			if( left > 0 )
			{
				emit signalAddArmyByGUI( sel );
				left = myControl->curAddArmyCount();
				ui.lineEditArmys->setText( QString::number( left ) );

				if( left == 0 && myControl->getCurCards()->getNumberOfCards() < 5 )
				{
					ui.pushButtonNextPhase->setEnabled( true );
				}
			}
		}
	}

	//Attack Phase
	if( myControl->curPhaseIs( ATTACK ) )
	{
		if( myControl->curPlayerIs( sel->getOwner() ))
		{
			att = sel;
			ui.labelAtt->setText( att->getLandName() );
		}
		else
		{
			def = sel;
			ui.labelDef->setText( def->getLandName() );
		}

		if( att && def )
		{
			emit signalAttackByGUI( att, def );
			if( def->getOwner() == att->getOwner() )
			{
				RiskMapLand *temp = def;
				selectLand( NULL );
				selectLand( temp );
			}
			else 
			{
				selectLand( NULL );
			}
			
		}
	}

	//Move Phase
	if( myControl->curPhaseIs( MOVE ) )
	{
		if( att == NULL &&
			myControl->curPlayerIs( sel->getOwner() ))
		{
			att = sel;
			ui.labelAtt->setText( att->getLandName() );
		}

		if( att && sel && (att != sel)  )
		{
			emit signalMoveByGUI( att, sel );
			selectLand( NULL );
		}
	}

}

void HumanPlayerGUI::slotCardSelectionChanged()
{
	if( !myControl->curPhaseIs( ADD_ARMYS ) ) 
	{
		this->ui.pushButtonExchange->setEnabled( false );
		return;
	}

	QList<QListWidgetItem*> list = ui.listWidgetCards->selectedItems();

	if( list.size() != 3 )
	{
		this->ui.pushButtonExchange->setEnabled( false );
		return;
	}

	QChar c[3];
	for( int i=0; i<3; i++ )
	{
		c[i] = list[i]->text().at(0);
	}

	if( (c[0] == c[1] && c[0] == c[2]) ||
		(c[0] != c[1] && c[0] != c[2] && c[1] != c[2] ))
	{
		this->ui.pushButtonExchange->setEnabled( true );
		return;
	}
	
	this->ui.pushButtonExchange->setEnabled( false );
}

void HumanPlayerGUI::slotExchangeCards()
{
	RiskCardDeck *deck = myControl->getCurCards();
	QList<QListWidgetItem*> list = ui.listWidgetCards->selectedItems();
		
	if( list.size() != 3 ) return;

	emit signalExchangeCardByGUI( list[0]->text(), list[1]->text(), list[2]->text() );
}

void HumanPlayerGUI::slotAddArmyByExchange()
{
	RiskCardDeck *deck = myControl->getCurCards();
	ui.listWidgetCards->clear();
	for( int i=0; i<deck->getNumberOfCards(); i++ )
	{
		ui.listWidgetCards->addItem( deck->getCardName( i ) );
	}

	ui.lineEditArmys->setText( QString::number( myControl->curAddArmyCount() ) );
}

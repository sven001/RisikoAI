#include "stdafx.h"
#include "DialogAttack.h"

DialogAttack::DialogAttack( Player *myPlayer, RiskGameLogic *logic, RiskMapLand *att, RiskMapLand *def, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	this->myPlayer = myPlayer;
	this->att = att;
	this->def = def;
	this->logic = logic;
	ui.labelAtt->setText( att->getLandName() );
	ui.labelDef->setText( def->getLandName() );
	ui.spinBoxAttackers->setSingleStep( 1 );
	ui.checkBoxAutoEnd->setChecked( autoEnd );
	fillDice();
}

DialogAttack::~DialogAttack()
{

}
#include "qabstractslider.h"
void DialogAttack::fillDice()
{
	if( att->getOwner() == def->getOwner() )
	{
		if( autoEnd )
		{
			accept();
		}
		else
		{
			ui.pushButtonRoll->setEnabled( false );
		}
	}
	if( att->getArmys() <= 1 ) ui.pushButtonRoll->setEnabled( false );
	if( def->getArmys() < 1 ) ui.pushButtonRoll->setEnabled( false );
	ui.lineEditAtt->setText( QString::number( att->getArmys() ) );
	ui.lineEditDef->setText( QString::number( def->getArmys() ) );

	ui.spinBoxAttackers->setRange( 0, att->getArmys()-1 );
	ui.spinBoxAttackers->setValue(att->getArmys()-1);
}

void DialogAttack::roll()
{
	QString log;
	logic->attack( myPlayer, att, def, ui.spinBoxAttackers->value(), &log );
	ui.textEdit->append( log );

	fillDice();
}

void DialogAttack::autoEndChanged( bool onOff )
{
	autoEnd = onOff;
}
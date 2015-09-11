#include "stdafx.h"
#include "DialogMove.h"

DialogMove::DialogMove(RiskMap *map, RiskMapLand *att, RiskMapLand *def, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->att = att;
	this->def = def;
	this->map = map;
	ui.labelAtt->setText( att->getLandName() );
	ui.labelDef->setText( def->getLandName() );
	ui.spinBoxAttackers->setSingleStep( 1 );
	ui.spinBoxAttackers->setRange( 0, att->getArmys()-1 );

	fillCount( att->getArmys()-1 );

}

DialogMove::~DialogMove()
{

}

void DialogMove::fillCount( int count )
{
	ui.lineEditAtt->setText( QString::number( att->getArmys() - count ) );
	ui.lineEditDef->setText( QString::number( def->getArmys() + count ) );
	ui.spinBoxAttackers->setValue( count );
}

void DialogMove::accept()
{
	int count = ui.spinBoxAttackers->value();

	if( count == 0 )
	{
		reject();
		return;
	}

	QDialog::accept();
}

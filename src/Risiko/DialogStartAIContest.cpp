#include "stdafx.h"
#include "DialogStartAIContest.h"

DialogStartAIContest::DialogStartAIContest(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	QDir dir;
	dir.setPath( qApp->applicationDirPath() );

	dlls = dir.entryList( QDir::nameFiltersFromString("RiskAI*.dll"), QDir::Files );

	for( int i=0; i<dlls.size(); i++ )
	{
		dlls[i] = dlls[i].left( dlls[i].size() - 4 );
	}
	dlls.insert( 0, "<Keine>" );

	this->comboBoxAI1->addItems( dlls );
	if( dlls.size() > 1 ) this->comboBoxAI1->setCurrentIndex( 1 ); else this->comboBoxAI1->setCurrentIndex( 0 );
	this->comboBoxAI2->addItems( dlls );
	if( dlls.size() > 2 ) this->comboBoxAI2->setCurrentIndex( 2 ); else this->comboBoxAI2->setCurrentIndex( 0 );
	this->comboBoxAI3->addItems( dlls );
	if( dlls.size() > 3 ) this->comboBoxAI3->setCurrentIndex( 3 ); else this->comboBoxAI3->setCurrentIndex( 0 );
	
}

DialogStartAIContest::~DialogStartAIContest()
{

}

#include "stdafx.h"
#include "PlayerAI.h"
#include <qmessagebox.h>

typedef RiskAI *(* DLLFKTP) ();

PlayerAI::PlayerAI( QString dllName )
	: Player()
{
	myAI = NULL;

	//dll = LoadLibrary( LPCWSTR(dllName.utf16()) );
	dll.setFileName( dllName );
	dll.load();
	//if( dll == NULL )
	if( !dll.isLoaded() )
	{
		QMessageBox qm( "Fehler","Konnte "+dllName+" nicht laden. Tschö...", QMessageBox::Critical, 0, 0, 0 );
		qm.exec();
		exit( -1 );
	}
	DLLFKTP fkt = NULL;
	//fkt = (DLLFKTP) GetProcAddress(dll, "getRiskAI");
	fkt = (DLLFKTP)dll.resolve( "getRiskAI" );
	if( fkt == NULL )
	{
		QMessageBox qm( "Fehler","Konnte Funktion getRiskAI() aus "+dllName+" nicht finden. Tschö...", QMessageBox::Critical, 0, 0, 0 );
		qm.exec();
		exit( -1 );
	}

	myAI = fkt();
}

PlayerAI::~PlayerAI()
{
	if( myAI != NULL ) delete myAI;
	
	dll.unload();
	//FreeLibrary( dll );
	
}

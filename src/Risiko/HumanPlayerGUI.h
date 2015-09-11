#ifndef HUMANPLAYERGUI_H
#define HUMANPLAYERGUI_H

#include <QDockWidget>
#include "RiskFlowControl.h"
#include "ui_HumanPlayerGUI.h"
#include "RiskMapLand.h"

class HumanPlayerGUI : public QDockWidget
{
	Q_OBJECT

public:
	HumanPlayerGUI(QWidget *parent = 0);
	~HumanPlayerGUI();

	void connectControl( RiskFlowControl &flowControl );
	
public slots:
	void update();
	void enablePlayer( bool onOff );
	void selectLand( RiskMapLand *sel );
	
	void slotCardSelectionChanged();
	void slotExchangeCards();
	void slotAddArmyByExchange();

signals:
	void signalAddArmyByGUI( RiskMapLand * );
	void signalAttackByGUI( RiskMapLand *att, RiskMapLand *def );
	void signalMoveByGUI( RiskMapLand *from, RiskMapLand *to );
	void signalExchangeCardByGUI( QString c1, QString c2, QString c3 );

private:
	RiskFlowControl *myControl;
	RiskMapLand *att;
	RiskMapLand *def;

	Ui::HumanPlayerGUIClass ui;
};

#endif // HUMANPLAYERGUI_H

#ifndef DIALOGATTACK_H
#define DIALOGATTACK_H

#include <QDialog>
#include "ui_DialogAttack.h"

#include "Player.h"
#include "RiskMapLand.h"
#include "RiskGameLogic.h"
static bool autoEnd = false;
class DialogAttack : public QDialog
{
	Q_OBJECT

public:
	DialogAttack( Player *myPlayer, RiskGameLogic *logic, RiskMapLand *att, RiskMapLand *def, QWidget *parent = 0);
	~DialogAttack();

public slots:
	void roll();
	void autoEndChanged( bool onOff );

private:
	void fillDice();
	Ui::DialogAttackClass ui;

	Player *myPlayer;
	RiskGameLogic *logic;
	RiskMapLand *att;
	RiskMapLand *def;
};

#endif // DIALOGATTACK_H

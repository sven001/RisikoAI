#ifndef DIALOGMOVE_H
#define DIALOGMOVE_H

#include <QDialog>
#include "ui_DialogMove.h"

#include "RiskMapLand.h"
#include "RiskMap.h"

class DialogMove : public QDialog
{
	Q_OBJECT

public:
	DialogMove(RiskMap *map, RiskMapLand *att, RiskMapLand *def, QWidget *parent = 0);
	~DialogMove();

public slots:
	void fillCount( int count );
	void accept();

	int getNumberOfArmys() { return( ui.spinBoxAttackers->value() ); };
private:

	RiskMap *map;
	RiskMapLand *att;
	RiskMapLand *def;

	Ui::DialogMoveClass ui;
};

#endif // DIALOGMOVE_H

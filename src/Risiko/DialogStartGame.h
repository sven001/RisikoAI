#ifndef DIALOGSTARTGAME_H
#define DIALOGSTARTGAME_H

#include <QDialog>
#include "ui_DialogStartGame.h"

class DialogStartGame : public QDialog
{
	Q_OBJECT

public:
	DialogStartGame(QWidget *parent = 0);
	~DialogStartGame();

	Ui::DialogStartGameClass ui;
private:
};

#endif // DIALOGSTARTGAME_H

#ifndef RISKMAPCONTINENT_H
#define RISKMAPCONTINENT_H

#include <QObject>
#include "RiskMapLand.h"

class RiskMapContinent : public QObject
{
	Q_OBJECT

public:
	RiskMapContinent( QString name, int id, int armyBonus );
	~RiskMapContinent();

	void addLand( RiskMapLand *land );
	QList<RiskMapLand *> getLands();

	int isOwnedByPlayer();
	int getArmyBonus() {return( armyBonus ); };

private:
	QString name;
	int id;
	int armyBonus;
	QList<RiskMapLand *> lands;
};

#endif // RISKMAPCONTINENT_H

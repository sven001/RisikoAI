#ifndef RISKMAPCONNECTION_H
#define RISKMAPCONNECTION_H

#include <QObject>
class RiskMapLand;

class RiskMapConnection : public QObject
{
	Q_OBJECT

public:
	RiskMapConnection(RiskMapLand *land1, RiskMapLand *land2);
	~RiskMapConnection();

	RiskMapLand *getLand( int index );
	RiskMapLand *getOtherLand( RiskMapLand *land );
	bool connects( int id1, int id2 );
private:
	RiskMapLand *land1;
	RiskMapLand *land2;
	
};

#endif // RISKMAPCONNECTION_H

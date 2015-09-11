#ifndef RISKMAPLAND_H
#define RISKMAPLAND_H

#include <QObject>
#include <QPointF>
#include <QColor>
class RiskMapConnection;
#include <QGraphicsItem>
class RiskMapLand : public QObject
{
	Q_OBJECT

public:
	double temp;	//Knotenmarker. Ändern/Löschen/lesen von map-Algos...

	RiskMapLand(int id, QString name, int continentalIndex, QPointF armyPos );
	~RiskMapLand();

	void addNeighbor( RiskMapConnection *neighbor );

	QString getLandName();
	int getLandId();
	int getContinentId() { return( continentalIndex );};
	QPointF getArmyPos() {return( armyPos ); };
	int getOwner();
	int getArmys();

	void setOwner( int owner );
	void setArmys( int armys );
	QList<RiskMapLand *> getNeighbors();
	bool hasEnemyNeighbor();

	void setItem( QGraphicsItem *landItem ) {myItem = landItem;};
	QColor getAIShadeColor() { return( shadeColor ); };
	void setAIShadeColor( int r, int g, int b, int a ) { shadeColor.setRgb( r,g,b,a );};
private:
	QColor shadeColor;
	QString landName;
	int landId;
	int continentalIndex;
	QPointF armyPos;

	QList<RiskMapConnection *>neighbors;

	int owner;
	int armys;

	//for gui updates...
	QGraphicsItem *myItem;
};

#define QLandList QList<RiskMapLand *>

#endif // RISKMAPLAND_H

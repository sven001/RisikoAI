#ifndef RISKMAP_H
#define RISKMAP_H

#include <QObject>
#include <QList>

#include "RiskMapLand.h"
#include "RiskMapConnection.h"
#include "RiskMapContinent.h"

class RiskMap : public QObject
{
	friend class RiskFlowControl;
	Q_OBJECT

public:
	RiskMap();
	~RiskMap();

	QString getPictureMap() {return( picName ); };
	QString getClickableMap() {return( mapName ); };

	RiskMapLand *findLandById( int id );
	QList<RiskMapLand *> getAllLands();
	QList<RiskMapConnection *> getAllConnections();
	QList<RiskMapContinent *> getAllContinents() {return( continents );};
	QList<RiskMapLand *> findShortestPath( RiskMapLand *att, RiskMapLand *def, bool ownerOnly=false );

	bool playerHasLands( int owner );
	bool playerHasAllLands( int owner );

signals:
	void signalLandCaptured( int owner, RiskMapLand *land );
protected:
	void clear();
	bool loadTestMap();
	bool loadFromFile( QString filename );

private:
	QString mapName;
	QString picName;

	QList<RiskMapLand *> lands;
	QList<RiskMapConnection *> connections;
	QList<RiskMapContinent *> continents;

	void markAll( double mark );
	void rekFindShortestPath( RiskMapLand *att, RiskMapLand *def, bool ownerOnly );

	void connectLands( int id1, int id2 ); 
	void clearLists();
};

#endif // RISKMAP_H

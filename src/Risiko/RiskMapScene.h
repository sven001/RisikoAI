#ifndef RISKMAPSCENE_H
#define RISKMAPSCENE_H

#include <QGraphicsScene>
class RiskMapLand;
class RiskMap;
class GraphItemLand;

class RiskMapScene : public QGraphicsScene
{
	Q_OBJECT

public:
	RiskMapScene( RiskMap *map, QWidget *parent = 0);
	~RiskMapScene();

	void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );

	void emitLandSelected( RiskMapLand *land);
	void resize();

signals:
	void signalLandSelected( RiskMapLand *land );


private:
	RiskMap *myMap;
	QPixmap *backGround;
	QImage *clickableMap;
	GraphItemLand *getGraphItemFor( RiskMapLand *land );
};

#endif // RISKMAPSCENE_H

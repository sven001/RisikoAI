#ifndef GRAPHITEMLAND_H
#define GRAPHITEMLAND_H

#include <QGraphicsItem>
#include "RiskMapLand.h"

class GraphItemLand : public QGraphicsItem
{
public:
	enum { Type = UserType + 1 };
	int type() const {
		return Type;
	}

	GraphItemLand(QGraphicsItem *parent, RiskMapLand *land);
	~GraphItemLand();


	RiskMapLand *getLand();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

private:
	double valueShade;
	QRectF sizeRectShadow;
	QRectF sizeRect;
	RiskMapLand *myLand;
};

#endif // GRAPHITEMLAND_H

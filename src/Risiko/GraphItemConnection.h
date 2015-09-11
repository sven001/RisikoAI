#ifndef GRAPHITEMCONNECTION_H
#define GRAPHITEMCONNECTION_H

#include <QGraphicsItem>
#include "GraphItemLand.h"

class GraphItemConnection : public QGraphicsItem
{
public:
	enum { Type = UserType + 2 };
	int type() const {
		return Type;
	}
	GraphItemConnection(QGraphicsItem *parent, GraphItemLand *land1, GraphItemLand *land2);
	~GraphItemConnection();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	GraphItemLand *land1;
	GraphItemLand *land2;
};

#endif // GRAPHITEMCONNECTION_H

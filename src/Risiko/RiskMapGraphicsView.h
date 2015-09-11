#ifndef RISKMAPGRAPHICSVIEW_H
#define RISKMAPGRAPHICSVIEW_H

#include <QGraphicsView>

class RiskMapGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	RiskMapGraphicsView( QWidget *widget );
	~RiskMapGraphicsView();

	void resizeEvent ( QResizeEvent * event );

private:
	
};

#endif // RISKMAPGRAPHICSVIEW_H

#ifndef NEURALNET_H
#define NEURALNET_H

#include <QObject>

#include "SVector.h"
#include "SMatrix.h"

class NeuralNet : public QObject
{
	Q_OBJECT
public:
	NeuralNet( int inputLength, int numberOfLayers );
	~NeuralNet();

	double getNetWeight();
	void setWeight( int layer, int fromNode, int toNode, double weight );
	double getWeight( int layer, int fromNode, int toNode );

	void setInput( SVector &input );
	void compute();
	SVector &getOutput();
	int getHighestOutputIndex();

	void learnOutput( int index, double val );
	int getNumberOfLearningSteps() { return( numberOfLearningSteps );};

	QList<double> getGenom();
	void setGenom( QList<double> );


private:
	int numberOfLearningSteps;
	int numberOfLayers;
	SVector input;
	SMatrix *layers;
	SVector output;
};

#endif // NEURALNET_H

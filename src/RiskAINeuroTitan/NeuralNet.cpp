#include "NeuralNet.h"

NeuralNet::NeuralNet( int inputLength, int numberOfLayers )
{
	numberOfLearningSteps = 0;
	this->numberOfLayers = numberOfLayers;
	this->layers = new SMatrix [numberOfLayers];

	for( int i=0; i<numberOfLayers; i++ )
	{
		layers[i].changeSize( inputLength, inputLength );
	}

	input.changeSize( inputLength );
	output.changeSize( inputLength );
}

NeuralNet::~NeuralNet()
{
	if( layers )
	{
		delete [] layers;
	}
}

double NeuralNet::getWeight( int layer, int fromNode, int toNode )
{
	if( layer >= numberOfLayers ) return(0);	
	if( fromNode >= layers[layer].columns() ) return(0);
	if( toNode >= layers[layer].rows() ) return(0);

	return( layers[layer](toNode, fromNode) );
}

void NeuralNet::setWeight( int layer, int fromNode, int toNode, double weight )
{
	if( layer >= numberOfLayers ) return;	
	if( fromNode >= layers[layer].columns() ) return;
	if( toNode >= layers[layer].rows() ) return;

	layers[layer](toNode, fromNode) = weight;
}

void NeuralNet::setInput( SVector &input )
{
	this->input = input;
}

void NeuralNet::compute()
{
	output = input;
	for( int i=0; i<numberOfLayers; i++ )
	{
		output = layers[i] * output;
	}
}

SVector &NeuralNet::getOutput()
{
	if( output.size() == 0 )
	{
		int arg=1;
	}
	return( output );
}

int NeuralNet::getHighestOutputIndex()
{
	return( output.getHighestValueIndex() );
}

QList<double> NeuralNet::getGenom()
{
	QList<double> arr;
	arr << numberOfLayers;
	arr << layers[0].rows();
	arr << layers[0].columns();
	

	for( int i=0; i<numberOfLayers; i++ )
	{
		for( int x=0; x<layers[i].rows(); x++ )
			for( int y=0; y<layers[i].columns(); y++ )
			{
				arr << layers[i](x,y);
			}
	}
	
	return( arr );
}

void NeuralNet::setGenom( QList<double> genom )
{
	int nr, c, r;
	nr = genom.takeFirst();
	r = genom.takeFirst();
	c = genom.takeFirst();

	if( nr != numberOfLayers ) return;

	for( int i=0; i<numberOfLayers; i++ )
	{
		layers[i].changeSize( r, r );
		for( int x=0; x<layers[i].rows(); x++ )
			for( int y=0; y<layers[i].columns(); y++ )
			{
				layers[i](x,y) = genom.takeFirst();
			}
	}
}

double NeuralNet::getNetWeight()
{
	double w = 0;
	for( int i=0; i<numberOfLayers; i++ )
		for( int x=0; x<layers[i].rows(); x++ )
			for( int y=0; y<layers[i].columns(); y++ )
			{
				w += layers[i](x,y);
			}
	return( w );
}

void NeuralNet::learnOutput( int index, double val )
{
	for( int y = 0; y < layers[0].columns(); y++ )
	{
		double valAnteil = layers[0](index, y) * input[y] / output[index] * val;
		layers[0](index, y) *= val;
	}
	numberOfLearningSteps ++;
}
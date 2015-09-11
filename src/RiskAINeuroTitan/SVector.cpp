#include "SVector.h"

SVector::SVector(int size)
{
	val = 0;
	changeSize( size );
}

SVector::SVector( const SVector &other )
{
	val = 0;
	*this = other;
}

SVector::~SVector(void)
{
	delete [] val;
}

void SVector::changeSize( int newSize )
{
	if( val != 0 ) delete [] val;
	this->sizeVal = newSize;
	val = new double[sizeVal];
	for( int i=0; i<sizeVal; i++ )
	{
		val[i] = 0;
	}
}

double &SVector::operator [](int index)
{
	if( index < sizeVal )
	{
		return( val[index] );	
	}
	
	//Böse Dinge passieren...
	return( val[0] );
}

double SVector::operator [](int index) const
{
	if( index < sizeVal )
	{
		return( val[index] );	
	}
	
	//Böse Dinge passieren...
	return( val[0] );
}

SVector &SVector::operator = ( const SVector &other)
{
	changeSize( other.size() );
	for( int i=0; i<sizeVal; i++ )
	{
		val[i] = other[i];
	}

	return( *this );
}

int SVector::getHighestValueIndex()
{
	double maxVal;
	double index = -1;
	for( int i=0; i<size(); i++ )
	{
		if( index == -1 ||
			maxVal < (*this)[i] )
		{
			index = i;
			maxVal = (*this)[i];
		}
	}
	return( index );
}
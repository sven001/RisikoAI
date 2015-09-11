#pragma once

#include "SVector.h"
class SMatrix
{
public:
	SMatrix(int row = 0, int column = 0 );
	virtual ~SMatrix(void);

	void changeSize( int row, int column ); 
	int rows() { return( rowVal );};
	int columns() { return( columnVal );};

	SVector &operator [](int index);
	double &operator ()(int row, int col );
	SVector operator *(SVector &vec);

protected:
	int rowVal, columnVal;
	SVector *val;
};

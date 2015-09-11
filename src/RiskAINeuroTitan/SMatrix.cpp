#include "SMatrix.h"

SMatrix::SMatrix(int row, int column)
{
	val = 0;
	changeSize( row, column );
}

SMatrix::~SMatrix(void)
{
	delete [] val;
}

void SMatrix::changeSize( int row, int column )
{
	if( val ) delete [] val;
	this->rowVal = row;
	this->columnVal = column;

	val = new SVector [row];
	for( int i=0; i<row; i++ )
	{
		val[i].changeSize( column );
	}
}

SVector &SMatrix::operator [](int index)
{
	if( index < rowVal )
	{
		return( val[index] );	
	}
	
	//Böse Dinge passieren...
	return( val[0] );
}

double &SMatrix::operator ()(int row, int col )
{
	if( row < rowVal )
	{
		return( val[row][col] );	
	}
	return( val[0][0] );
}

SVector SMatrix::operator *(SVector &vec)
{
	if( columnVal == vec.size() )
	{
		SVector ret(rowVal);
		for( int x=0; x<rowVal; x++ )
		{
			ret[x] = 0;
			for( int y=0; y<columnVal; y++ )
			{
				ret[x] += val[x][y] * vec[y];
				
			}
		}
		return( ret );
	}
	return( SVector() );
}
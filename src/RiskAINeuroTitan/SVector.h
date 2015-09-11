#pragma once

class SVector
{
public:
	SVector( int size = 0 );
	SVector( const SVector &other );
	virtual ~SVector(void);

	bool isValid() {return( sizeVal != 0 );};
	int size() const { return( sizeVal ); };
	void changeSize( int newSize );
	
    double &operator [](int);
	double operator [](int) const;
	SVector &operator = ( const SVector &other);
	int getHighestValueIndex();

protected:
	int sizeVal;
	double *val;
};

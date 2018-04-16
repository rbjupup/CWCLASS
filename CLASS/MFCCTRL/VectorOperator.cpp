#include "stdafx.h"
#include "VectorOperator.h"
#include <math.h>


#ifdef __cplusplus	/* C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ */

VECTOR operator + ( VECTOR& a, VECTOR& b)
{
	VECTOR c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

VECTOR2D operator +(VECTOR2D& a, VECTOR2D& b)
{
	VECTOR2D c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

VECTOR operator - (VECTOR& a, VECTOR& b)
{
	VECTOR c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}
VECTOR operator - (VECTOR& a)
{
	VECTOR c;
	c.x = -a.x;
	c.y = -a.y;
	c.z = -a.z;

	return c;
}

/* Dot product */
double operator *(VECTOR& a, VECTOR& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}


/* Number product */
VECTOR operator *(double k, VECTOR&a)
{
	VECTOR b;
	b.x = k*a.x;
	b.y = k*a.y;
	b.z = k*a.z;

	return b;
}

/* Cross product */
VECTOR operator %(VECTOR& a, VECTOR& b)
{
	VECTOR c;
	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

VECTOR2D operator *(double k, VECTOR2D& a)
{
	VECTOR2D c;
	c.x = k*a.x;
	c.y = k*a.y;

	return c;
}

VECTOR2D operator /(VECTOR2D& a, double k)
{
	VECTOR2D b;
	b.x = a.x/k;
	b.y = a.y/k;

	return b;
}

VECTOR operator /(VECTOR& a, double k)
{
	VECTOR b;
	b.x = a.x/k;
	b.y = a.y/k;
	b.z = a.z/k;

	return b;
}

VECTOR2D operator *(VECTOR2D& a, double k)
{
	VECTOR2D b;
	b.x = a.x*k;
	b.y = a.y*k;

	return b;
}

double operator *(VECTOR2D& a, VECTOR2D& b)
{
	return a.x*b.x + a.y*b.y;
}
/*
VECTOR2D operator *(VECTOR2D a, double k)
{
	VECTOR2D b;
	b.x = a.x*k;
	b.y = a.y*k;

	return b;
}
*/
VECTOR2D operator -(VECTOR2D& a, VECTOR2D& b)
{
	VECTOR2D c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;

	return c;
}

/*------------------------------------------------------------------------------------*/
#endif /* C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ C++ */
/*------------------------------------------------------------------------------------*/

double Module(VECTOR2D a)
{
	return sqrt(a.x*a.x +a.y*a.y);
}

double dot_product(VECTOR a, VECTOR b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

VECTOR sub(VECTOR a, VECTOR b)
{
	VECTOR c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}

VECTOR add(VECTOR a, VECTOR b)
{
	VECTOR c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

/* Cross product */
VECTOR cross_product(VECTOR a, VECTOR b)
{
	VECTOR c;
	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;

	return c;
}

VECTOR product(VECTOR a, double k)
{
	VECTOR c;

	c.x = a.x*k;
	c.y = a.y*k;
	c.z = a.z*k;

	return c;
}

double Module(VECTOR a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

/* High Level Functions. */
void RotateAxis(VECTOR axis, VECTOR point, double theta, VECTOR *pv)
{
	VECTOR v;
	v = *pv - point;
	RotateAxis(axis, theta, &v);
	*pv = v + point;
}

void Multip(VECTOR *pv, double T[4][4])
{
	double a[4], b[4];
	int i;
	a[0] = pv->x; a[1] = pv->y; a[2] = pv->z; a[3] = 1;

	for(i=0; i<4; i++)
		b[i] = a[0]*T[0][i] + a[1]*T[1][i] + a[2]*T[2][i] + a[3]*T[3][i];

	pv->x = b[0]; pv->y = b[1]; pv->z = b[2];
}

void RotateAxis(VECTOR vAxis, double alpha, VECTOR *pv)
{
	double Tz[4][4]={0,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,0,1},
		   Ty[4][4]={0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,1},
		   Tx[4][4]={1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1};
	double phyi, theta;

	theta = atan2(vAxis.y, vAxis.x);
	phyi = atan2(vAxis.z, sqrt(vAxis.x*vAxis.x + vAxis.y*vAxis.y));

	Tz[0][0] = cos(-theta); Tz[0][1] = sin(-theta);
	Tz[1][0] = -sin(-theta); Tz[1][1] = cos(-theta);
	Multip(pv, Tz);

	Ty[0][0] = cos(phyi); Ty[0][2] = -sin(phyi);
	Ty[2][0] = sin(phyi); Ty[2][2] = cos(phyi);
	Multip(pv, Ty);

	Tx[1][1] = cos(alpha); Tx[1][2] = sin(alpha);
	Tx[2][1] = -sin(alpha); Tx[2][2] = cos(alpha);
	Multip(pv, Tx);

	Ty[0][0] = cos(-phyi); Ty[0][2] = -sin(-phyi);
	Ty[2][0] = sin(-phyi); Ty[2][2] = cos(-phyi);
	Multip(pv, Ty);

	Tz[0][0] = cos(theta); Tz[0][1] = sin(theta);
	Tz[1][0] = -sin(theta); Tz[1][1] = cos(theta);
	Multip(pv, Tz);
}

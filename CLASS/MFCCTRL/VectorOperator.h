#ifndef _VECTOR_OPERATOR_H_
#define _VECTOR_OPERATOR_H_

#include "aglib.h"

#ifdef __cplusplus

VECTOR operator + ( VECTOR& a, VECTOR& b);
VECTOR operator - (VECTOR& a, VECTOR& b);
VECTOR operator - (VECTOR& a);
double operator *(VECTOR& a, VECTOR& b);
VECTOR operator *(double k, VECTOR&a);
VECTOR operator %(VECTOR& a, VECTOR& b);
VECTOR2D operator *(double k, VECTOR2D& a);
VECTOR2D operator /(VECTOR2D& a, double k);
VECTOR2D operator *(VECTOR2D& a, double k);
VECTOR operator /(VECTOR& a, double k);
VECTOR2D operator -(VECTOR2D& a, VECTOR2D& b);
double operator *(VECTOR2D& a, VECTOR2D& b);
VECTOR2D operator +(VECTOR2D& a, VECTOR2D& b);

void Multip(VECTOR *pv, double T[4][4]);
void RotateAxis(VECTOR axis, VECTOR point, double theta, VECTOR *pv);
void RotateAxis(VECTOR vAxis, double alpha, VECTOR *pv);
double Module(VECTOR2D a);

#endif /* C++ */

double Module(VECTOR a);
double dot_product(VECTOR a, VECTOR b);
VECTOR sub(VECTOR a, VECTOR b);
VECTOR add(VECTOR a, VECTOR b);
VECTOR cross_product(VECTOR a, VECTOR b);
VECTOR product(VECTOR a, double k);



#endif /* _VECTOR_OPERATOR_H_ */
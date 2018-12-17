#pragma once
#ifndef _T_MATH_H
#define _T_MATH_H
#include "typedefs.h"

//Common math functions

//sign of a float
int sign(double x);
//floating point division
double fdiv(double nominator, double denominator);

//solve functions
void solveLinear(double &a, double &b, double &x1);
void solveQuadraticREAL(double &a, double &b, double &c, double &x1, double &x2, int &realVals );

//convert coords cartesian -> spherical, returns <theta, phi> -> <x,y>
double2 cart2sph(double x, double y, double z);
#endif;

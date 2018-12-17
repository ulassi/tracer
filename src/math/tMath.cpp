#include ".\stdafx.h"
#include ".\tMath.h"
#include <math.h>

/*
	Convert carthesian to spherical coordinates
	
	Right hand coord system, Y defined as "up"
		x = r sin(phi) cos(theta)
		y = r cos(phi)
		z = r sin(phi) sin(theta)

		->

		theta = atan(z/x)
		phi = acos(y/r)

	returns
		double2 (x = theta, y = phi) in radians

	Status: Unit tested

*/
double2 cart2sph(double x, double y, double z) {
	double2 sph;
	double r = sqrt(x*x + y*y + z*z);
	sph.x = atan(fdiv(z,x)); //theta
	if (sph.x < 0)
		sph.x += 2*PI;
    sph.y = acos(fdiv(y,r)); //phi
	if (sph.y < 0)
		sph.y += PI;
	return sph;
}
/*
	Convert spherical to cartesian coordinates

	Details see cart2sph

	returns
		double3 (x,y,z)
*/
double3 sph2cart(double theta, double phi) {
	double3 cart;
	cart.x = sin(phi)*cos(theta);
	cart.y = cos(phi);
	cart.z = sin(phi)*sin(theta);
	return cart;
}
int sign(double x)
{
  return (x == 0 ? 0 : (x < 0 ? -1 : 1));
}
//safe floating point division
// returns nom / denom (0 / 0 = 0, overflow = LARGE_VAL with correct sign)
double fdiv(double nominator, double denominator)
{
  if (denominator == 0)
  {
    if (nominator == 0) return 0;
    else return LARGE_VAL * sign(nominator);
  }
  else
  {
    if (nominator == 0) return 0;
    else
    {
      if ((nominator + denominator) == nominator) return LARGE_VAL * sign(nominator) * sign(denominator);
      else return nominator / denominator;
    }
  }
}
/*
	Solve linear equation
		a*x + b = 0
*/
void solveLinear(double &a, double &b, double &x1) {
	if (a == 0)
		x1 = 0;
	else 
		x1 = fdiv(-b, a);
}
/*
	Solves the two quadratic roots, x1 and x2, to
		a*x^2 + b*x + c = 0
	Returns
		x1 and x2 are roots 
		realVals -> 0 if no real roots
		realVals -> 1 if x1==x2
		realVals -> 2 if x1 and x2 real
*/
void solveQuadraticREAL(double &a, double &b, double &c, double &x1, double &x2, int &realVals ) {
	//Numerical Recipes in C
	//double q = -0.5 * (b + sign(b)*sqrt(b*b - 4*a*c));
	//x1 = fdiv(q,a);
	//x2 = fdiv(c,q);
	if (a == 0) {
		solveLinear(b,c,x1);
		realVals = 1;
	}
	else
	{
		/*double d = (b * b) - (4 * a * c);
		if (d == 0)
		{
			//q = -0.5 * b;
			x1 = fdiv(-0.5 * b,a);
			realVals = 1;
		}
		else if (d < 0)
			realVals = 0;
		else {
			x1=(-b + sqrt((b*b)-(4*a*c)))/(2*a);
			x2=(-b - sqrt((b*b)-(4*a*c)))/(2*a);
			realVals = 2;
		}*/
        double d = b * b - 4 * a * c;
		double q;
		if (d == 0)
		{
			q = -0.5 * b;
			x1 = fdiv(q,a);
			realVals = 1;
		}
		else
		{
			if (d > 0)
			{
				q = -0.5 * (b + sign(b)*sqrt(b*b - 4*a*c));
				x1 = fdiv(q,a);
				x2 = fdiv(c,q);
				realVals = 2;
			}
			else {
				//imag roots
				realVals = 0;
			}
		}
	}
}
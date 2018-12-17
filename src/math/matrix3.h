/* 
Copyright (C) 2002 Urban Lassi, not for commercial use. 
	Feel free to use it for learning or anything that you
	wont make money out of.

	Questions: mail me at urbla606@student.liu.se
*/
// matrix3.h: interface for the matrix3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX3_H__9164EA99_C545_4A39_BA79_B890B263C2AD__INCLUDED_)
#define AFX_MATRIX3_H__9164EA99_C545_4A39_BA79_B890B263C2AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CommonTypes.h"
#include "vec3.h"

//##ModelId=3DF799E302BB
class matrix3  
{
public:
	//##ModelId=3DF8CFD90328
	real elements[3][3];

	//constructors
	//##ModelId=3DF799E302E3
	matrix3::matrix3()
	{
		elements[0][0] = 1.0f;
		elements[0][1] = 0.0f;
		elements[0][2] = 0.0f;
		elements[1][0] = 0.0f;
		elements[1][1] = 1.0f;
		elements[1][2] = 0.0f;
		elements[2][0] = 0.0f;
		elements[2][1] = 0.0f;
		elements[2][2] = 1.0f;
	}
	
	//##ModelId=3DF799E302ED
	matrix3::matrix3(real init)
	{
		elements[0][0] = init;
		elements[0][1] = init;
		elements[0][2] = init;
		elements[1][0] = init;
		elements[1][1] = init;
		elements[1][2] = init;
		elements[2][0] = init;
		elements[2][1] = init;
		elements[2][2] = init;
	}
	
	//##ModelId=3DF799E302F7
	matrix3::matrix3(real a11, real a12, real a13,
			real a21, real a22, real a23,
			real a31, real a32, real a33)
	{
		elements[0][0] = a11;
		elements[0][1] = a12;
		elements[0][2] = a13;
		elements[1][0] = a21;
		elements[1][1] = a22;
		elements[1][2] = a23;
		elements[2][0] = a31;
		elements[2][1] = a32;
		elements[2][2] = a33;
	}
	
	//##ModelId=3DF799E30347
	inline real matrix3::determinant()
	{
		return elements[0][0] * elements[1][1] * elements[2][2] +
			elements[0][1] * elements[1][2] * elements[2][0] +
			elements[0][2] * elements[1][0] * elements[2][1] -
			elements[0][0] * elements[1][2] * elements[2][1] -
			elements[0][1] * elements[1][0] * elements[2][2] -
			elements[0][2] * elements[1][1] * elements[2][0];
		
	}
	
	//##ModelId=3DF799E30365
	inline matrix3 matrix3::transpose()
	{	//transpose matrix
		matrix3 mat;
		mat.elements[0][0] = this->elements[0][0];
		mat.elements[0][1] = this->elements[1][0];
		mat.elements[0][2] = this->elements[2][0];
		mat.elements[1][0] = this->elements[0][1];
		mat.elements[1][1] = this->elements[1][1];
		mat.elements[1][2] = this->elements[2][1];
		mat.elements[2][0] = this->elements[0][2];
		mat.elements[2][1] = this->elements[1][2];
		mat.elements[2][2] = this->elements[2][2];
		return mat;
	}
	
	//##ModelId=3DF799E3036F
	inline matrix3 matrix3::invert()
	{	//invert matrix
		real i_det = 1/this->determinant();
		
		real d11 = this->get(2,2) * this->get(3,3) - this->get(2,3)*this->get(3,2);
		real d12 = this->get(1,2) * this->get(3,3) - this->get(1,3)*this->get(3,2);
		real d13 = this->get(1,2) * this->get(2,3) - this->get(1,3)*this->get(2,2);
		
		real d21 = this->get(2,1) * this->get(3,3) - this->get(2,3)*this->get(3,1);
		real d22 = this->get(1,1) * this->get(3,3) - this->get(1,3)*this->get(3,1);
		real d23 = this->get(1,1) * this->get(2,3) - this->get(1,3)*this->get(2,1);
		
		real d31 = this->get(2,1) * this->get(3,2) - this->get(2,2)*this->get(3,1);
		real d32 = this->get(1,1) * this->get(3,2) - this->get(1,2)*this->get(3,1);
		real d33 = this->get(1,1) * this->get(2,2) - this->get(1,2)*this->get(2,1);
		
		matrix3 adj(d11,-d12,d13,-d21,d22,-d23,d31,-d32,d33);
			   
		return (adj*i_det);
	}
	
	
	//##ModelId=3DF799E30379
	inline vec3 matrix3::getColumnVector(short c)
	{	//returns column vector c
		vec3 ret;
		if ( (c < 4) && (c > 0)) 
		{
			ret.set(elements[0][c-1],elements[1][c-1],elements[2][c-1]);
		}
		
		return ret;
	}
	
	//##ModelId=3DF799E30397
	inline vec3 matrix3::getRowVector(short r)
	{	//returns row vector r
		vec3 ret(0.0f,0.0f,0.0f);
		if ( (r < 4) && (r > 0)) 
		{
			ret.set(elements[r-1][0],elements[r-1][1],elements[r-1][2]);
		}
		
		return ret;
	}
	
	//##ModelId=3DF799E303AB
	inline void matrix3::setColumnVector(short c, const vec3 &vec)
	{	//sets column vector c
		if ( (c < 4) && (c > 0)) 
		{
			elements[0][c-1] = vec[0];
			elements[1][c-1] = vec[1];
			elements[2][c-1] = vec[2];
		}
	}
	
	//##ModelId=3DF799E303C9
	inline void matrix3::setColumnVector(short c, vec3 *vec)
	{	//sets column vector c
		if ( (c < 4) && (c > 0)) 
		{
			elements[0][c-1] = (*vec)[0];
			elements[1][c-1] = (*vec)[1];
			elements[2][c-1] = (*vec)[2];
		}

	}
	
	//##ModelId=3DF799E303DD
	inline void matrix3::setRowVector(short r, const vec3 &vec)
	{	//sets row vector r
		if ( (r < 4) && (r > 0)) 
		{
			elements[r-1][0] = vec[0];
			elements[r-1][1] = vec[1];
			elements[r-1][2] = vec[2];
		}
	}

	//##ModelId=3DF799E40013
	inline void matrix3::setRowVector(short r, vec3 *vec)
	{	//sets row vector r
		if ( (r < 4) && (r > 0)) 
		{
			elements[r-1][0] = (*vec)[0];
			elements[r-1][1] = (*vec)[1];
			elements[r-1][2] = (*vec)[2];
		}
	}

	//##ModelId=3DF799E40028
	inline void matrix3::orthonormalize()
	{
		vec3 x(this->elements[0][0],this->elements[1][0],this->elements[2][0]);
		vec3 y(this->elements[0][1],this->elements[1][1],this->elements[2][1]);
		vec3 z;
		
		x.normalize();
		z = x*y;
		z.normalize();
		y = z*x;
		y.normalize();

		elements[0][0] = x[0];
		elements[0][1] = y[0];
		elements[0][2] = z[0];
		elements[1][0] = x[1];
		elements[1][1] = y[1];
		elements[1][2] = z[1];
		elements[2][0] = x[2];
		elements[2][1] = y[2];
		elements[2][2] = z[2];
	}

	//##ModelId=3DF799E40032
	inline real matrix3::get(short r, short c)
	{	//gets element 
		return elements[r-1][c-1];
	}

	//##ModelId=3DF799E40050
	inline void matrix3::set(short r, short c, real value)
	{	//set element
		elements[r][c] = value;
	}
	

	//##ModelId=3DF799E4006E
	inline void matrix3::setRotation(const vec3 & v, real rotAngle)
	{	//set element
		real qs, qva, qvb, qvc;
		vec3 u;
		u = v;
		u.normalize();
		rotAngle /= 2;

		qs =  cos(rotAngle);
		qva =  u[0] * sin(rotAngle);
		qvb =  u[1] * sin(rotAngle);
		qvc =  u[2] * sin(rotAngle);
    
		// quaternion calculations, switcha?
		elements[0][0] = 1 - 2*qvb*qvb - 2*qvc*qvc;
		elements[0][1] = 2*qvb*qva - 2*qs*qvc;
		elements[0][2] = 2*qva*qvc + 2*qs*qvb;
    
		elements[1][0] = 2*qva*qvb + 2*qs*qvc;
		elements[1][1] = 1-2*qva*qva - 2*qvc*qvc;
		elements[1][2] = 2*qvb*qvc -2*qs*qva;
    
		elements[2][0] = 2*qva*qvc - 2*qs*qvb;
		elements[2][1] = 2*qvb*qvc + 2*qs*qva;
		elements[2][2] = 1 - 2*qva*qva - 2*qvb*qvb;
		
	}
	

	inline vec3 matrix3::operator *(const vec3 &Mult) const
	{	//matrix * vector multiplication
		real x = elements[0][0] * Mult[0] + elements[0][1] * Mult[1] + elements[0][2] * Mult[2];
		real y = elements[1][0] * Mult[0] + elements[1][1] * Mult[1] + elements[1][2] * Mult[2];
		real z = elements[2][0] * Mult[0] + elements[2][1] * Mult[1] + elements[2][2] * Mult[2];
		vec3 ret(x,y,z);
		return ret;
	}
	inline matrix3 matrix3::operator *(const real mult) const
	{	//matrix * const multiplication
		matrix3 ret(0.0f);
		
		for (short int i=0; i<3; i++)
		{
			for (short int j=0; j<3; j++)
			{
				real a = (elements[i][j] )*mult;
				ret.set(i,j,a);
			}
		}
		return ret;
	}

	//##ModelId=3DF799E4008C
	inline matrix3 matrix3::operator* (const matrix3 &Mult) const
	{

		real a = this->elements[0][0]*Mult.elements[0][0] + this->elements[0][1]*Mult.elements[1][0] + this->elements[0][2]*Mult.elements[2][0];
		real b = this->elements[1][0]*Mult.elements[0][0] + this->elements[1][1]*Mult.elements[1][0] + this->elements[1][2]*Mult.elements[2][0];
		real c = this->elements[2][0]*Mult.elements[0][0] + this->elements[2][1]*Mult.elements[1][0] + this->elements[2][2]*Mult.elements[2][0];

		real d = this->elements[0][0]*Mult.elements[0][1] + this->elements[0][1]*Mult.elements[1][1] + this->elements[0][2]*Mult.elements[2][1];
		real e = this->elements[1][0]*Mult.elements[0][1] + this->elements[1][1]*Mult.elements[1][1] + this->elements[1][2]*Mult.elements[2][1];
		real f = this->elements[2][0]*Mult.elements[0][1] + this->elements[2][1]*Mult.elements[1][1] + this->elements[2][2]*Mult.elements[2][1];

		real g = this->elements[0][0]*Mult.elements[0][2] + this->elements[0][1]*Mult.elements[1][2] + this->elements[0][2]*Mult.elements[2][2]; 
		real h = this->elements[1][0]*Mult.elements[0][2] + this->elements[1][1]*Mult.elements[1][2] + this->elements[1][2]*Mult.elements[2][2];
		real i = this->elements[2][0]*Mult.elements[0][2] + this->elements[2][1]*Mult.elements[1][2] + this->elements[2][2]*Mult.elements[2][2];
		return matrix3(a,d,g,b,e,h,c,f,i);
	}
	inline matrix3 matrix3::operator *=(const matrix3 &Mult) 
	{
		this->elements[0][0] *= Mult.elements[0][0];
		this->elements[0][1] *= Mult.elements[0][1];
		this->elements[0][2] *= Mult.elements[0][2];
		this->elements[1][0] *= Mult.elements[1][0];
		this->elements[1][1] *= Mult.elements[1][1];
		this->elements[1][2] *= Mult.elements[1][2];
		this->elements[2][0] *= Mult.elements[2][0];
		this->elements[2][1] *= Mult.elements[2][1];
		this->elements[2][2] *= Mult.elements[2][2];
		return *this;
	}
	inline matrix3 matrix3::operator =(const matrix3 &Mult) 
	{
		this->elements[0][0] = Mult.elements[0][0];
		this->elements[0][1] = Mult.elements[0][1];
		this->elements[0][2] = Mult.elements[0][2];
		this->elements[1][0] = Mult.elements[1][0];
		this->elements[1][1] = Mult.elements[1][1];
		this->elements[1][2] = Mult.elements[1][2];
		this->elements[2][0] = Mult.elements[2][0];
		this->elements[2][1] = Mult.elements[2][1];
		this->elements[2][2] = Mult.elements[2][2];
		return *this;
	}
	inline matrix3 matrix3::operator /(const real d) const
	{	//divide each matrix element with d
		matrix3 ret(0.0f);

		for (short int i=0; i<3; i++)
		{
			for (short int j=0; j<3; j++)
			{
				real a = (elements[i][j] )/d;
				ret.set(i,j,a);
			}
		}
		return ret;
	}
	inline matrix3 matrix3::operator /=(const real d)
	{
		this->elements[0][0] /= d;
		this->elements[0][1] /= d;
		this->elements[0][2] /= d;
		this->elements[1][0] /= d;
		this->elements[1][1] /= d;
		this->elements[1][2] /= d;
		this->elements[2][0] /= d;
		this->elements[2][1] /= d;
		this->elements[2][2] /= d;
		return *this;
	}
	inline matrix3 matrix3::operator +(const matrix3 &Add) const
	{
		matrix3 ret;
		for (short int i=0; i<3; i++)
		{
			for (short int j=0; j<3; j++)
			{
				real a = elements[i][j] + Add.elements[i][j];
				ret.set(i,j,a);
			}
		}
		return ret;
	}
	inline matrix3 matrix3::operator +=(const matrix3 &Add) 
	{
		this->elements[0][0] += Add.elements[0][0];
		this->elements[0][1] += Add.elements[0][1];
		this->elements[0][2] += Add.elements[0][2];
		this->elements[1][0] += Add.elements[1][0];
		this->elements[1][1] += Add.elements[1][1];
		this->elements[1][2] += Add.elements[1][2];
		this->elements[2][0] += Add.elements[2][0];
		this->elements[2][1] += Add.elements[2][1];
		this->elements[2][2] += Add.elements[2][2];
		return *this;
	}
	inline matrix3 matrix3::operator -(const matrix3 &Sub) const
	{
		matrix3 ret;
		for (short int i=0; i<3; i++)
		{
			for (short int j=0; j<3; j++)
			{
				real a = elements[i][j] - Sub.elements[i][j];
				ret.set(i,j,a);
			}
		}
		return ret;
	}
	inline matrix3 matrix3::operator -=(const matrix3 &Sub) 
	{
		this->elements[0][0] -= Sub.elements[0][0];
		this->elements[0][1] -= Sub.elements[0][1];
		this->elements[0][2] -= Sub.elements[0][2];
		this->elements[1][0] -= Sub.elements[1][0];
		this->elements[1][1] -= Sub.elements[1][1];
		this->elements[1][2] -= Sub.elements[1][2];
		this->elements[2][0] -= Sub.elements[2][0];
		this->elements[2][1] -= Sub.elements[2][1];
		this->elements[2][2] -= Sub.elements[2][2];
		return *this;
	}
	//##ModelId=3DF799E400AA
	matrix3::~matrix3()
	{

	}

};

#endif // !defined(AFX_MATRIX3_H__9164EA99_C545_4A39_BA79_B890B263C2AD__INCLUDED_)

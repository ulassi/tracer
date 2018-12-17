/* 
Copyright (C) 2002 Urban Lassi, not for commercial use. 
	Feel free to use it for learning or anything that you
	wont make money out of.

	Questions: mail me at urbla606@student.liu.se
*/
// matrix4.h: interface for the matrix4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX4_H__C03587AB_DA95_491A_BB91_A95F60FAD854__INCLUDED_)
#define AFX_MATRIX4_H__C03587AB_DA95_491A_BB91_A95F60FAD854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CommonTypes.h"
#include "vec3.h"
#include "matrix3.h"
//##ModelId=3DF799E30008
class matrix4  
{
public:
    // constructors/destructors
	//##ModelId=3DF799E30012
	inline matrix4();
	//##ModelId=3DF799E3001C
	inline matrix4(real * f);

	inline matrix4(real f);
	
	//##ModelId=3DF799E30027
    inline matrix4(const vec3 & translation, const matrix3 & rotation); //added
	//##ModelId=3DF799E3003B
	inline ~matrix4();
   
    
	
	//##ModelId=3DF799E30044
	inline void setTransformation(const vec3 & translation, const matrix3 & rotation); //added
	
	//##ModelId=3DF799E30062
    inline real getElement(int row, int col) const;
	
	//##ModelId=3DF799E30077
    inline void setIdentity();
	
	//##ModelId=3DF799E3008A
    inline void setTranslation(const vec3 & translation);	//added
	
	//##ModelId=3DF799E3009E
	inline void setRotation(const matrix3 & rotation);		//added
	
	//##ModelId=3DF799E300A9
	inline void setRotation(const vec3 & v, real rotAngle);
	
	//##ModelId=3DF799E300C6
    inline void transpose();
	
	//##ModelId=3DF799E300D0
    inline void invert();
	
	//##ModelId=3DF799E300E4
    inline real det();
    

    // typecast
	
	//##ModelId=3DF799E300EE
    inline operator const real * () const;
	
	//##ModelId=3DF799E300F9
    inline void asColMajor(real arr[16] );

    //operators
	
	//##ModelId=3DF799E3010C
    inline real operator [] (int index) const;
	
	//##ModelId=3DF799E30120
    inline real & operator [] (int index);

	
	//##ModelId=3DF799E30134
    inline matrix4 & operator = (const matrix4 & mat);
	
	//##ModelId=3DF799E30148
    inline matrix4 & operator += (const matrix4 & mat);
	
	//##ModelId=3DF799E3015D
    inline matrix4 & operator -= (const matrix4 & mat);
	
	//##ModelId=3DF799E30171
    inline matrix4 & operator *= (const matrix4 &mat);
	
	
	//##ModelId=3DF799E3017C
	inline matrix3 getRotation();
	
	//##ModelId=3DF799E3018F
	inline vec3 getTranslation();
	
	
	
	//##ModelId=3DF8CFD901DE
    real m[4][4];
};

extern inline matrix4 mulmm(const matrix4 &m1, const matrix4 &m2);
extern inline vec3 mulvm(const vec3 &v, const matrix4 &m);

#include "matrix4.cpp"
#endif // !defined(AFX_MATRIX4_H__C03587AB_DA95_491A_BB91_A95F60FAD854__INCLUDED_)

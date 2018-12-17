//#include "matrix4.h"


inline matrix4::matrix4()
{
    // doesn't inline for loops?
    //printf("initiated matrix to null matrix\n");
    // lousy slow for test only
    /*
	for(int i=0;i<4;i++){
	for(int j=0;j<4;j++){
	    m[i][j] = 0.0;
	}
    }*/
	setIdentity();
}

inline matrix4::matrix4(real * f)
{ 
    // doesn't inline for loops?
    //printf("initiated matrix from real array\n");
    // lousy slow for test only
    for(int i=0;i<4;i++){
	for(int j=0;j<4;j++){
	    m[i][j] = f[i*4+j];
	}
    }
}

inline matrix4::matrix4(real f)
{ 
    // doesn't inline for loops?
    //printf("initiated matrix from real array\n");
    // lousy slow for test only
    for(int i=0;i<4;i++){
	for(int j=0;j<4;j++){
	    m[i][j] = 0.0;
	}
    }
}

inline matrix4::matrix4(const vec3 & translation, const matrix3 & rotation)
{
	setIdentity();
	setTransformation(translation, rotation);
}

inline matrix4::~matrix4()
{

}



	
	/*############################################################################*/
// methods
inline void matrix4::setTransformation(const vec3 & translation, const matrix3 & rotation)
{
	this->setTranslation(translation);
	this->setRotation(rotation);
}

inline void matrix4::setTranslation(const vec3 & translation)
{
	
	m[0][3] = translation[0];
	m[1][3] = translation[1];
	m[2][3] = translation[2];
	
	  /*
	m[3][0] = translation.x;
	m[3][1] = translation.y;
	m[3][2] = translation.z;
	*/
	
}

inline void matrix4::setRotation(const matrix3 & rotation)
{
	m[0][0] = rotation.elements[0][0];
    m[0][1] = rotation.elements[0][1];
    m[0][2] = rotation.elements[0][2];
    
    m[1][0] = rotation.elements[1][0];
    m[1][1] = rotation.elements[1][1];
    m[1][2] = rotation.elements[1][2];
    
    m[2][0] = rotation.elements[2][0];
    m[2][1] = rotation.elements[2][1];
    m[2][2] = rotation.elements[2][2];
}
	


inline real matrix4::getElement(int row, int col) const
{
    return m[row][col];
}

inline void matrix4::setIdentity()
{
    // doesn't inline for loops?
    for(int i=0;i<4;i++){
	for(int j=0;j<4;j++){
	    if(j==i)
		m[i][j] = 1.0;
	    else
		m[i][j] = 0.0;
	}
    }
}

inline void matrix4::setRotation(const vec3 & v, real rotAngle)
{
    real qs, qva, qvb, qvc;
    vec3 u;
    u = v;
    u.normalize();
    rotAngle /= 2;

    qs = cos(rotAngle);
    qva = u[0] * sin(rotAngle);
    qvb = u[1] * sin(rotAngle);
    qvc = u[2] * sin(rotAngle);
    
    // quaternion calculations
    m[0][0] = 1 - 2*qvb*qvb - 2*qvc*qvc;
    m[0][1] = 2*qvb*qva - 2*qs*qvc;
    m[0][2] = 2*qva*qvc + 2*qs*qvb;
    
    m[1][0] = 2*qva*qvb + 2*qs*qvc;
    m[1][1] = 1-2*qva*qva - 2*qvc*qvc;
    m[1][2] = 2*qvb*qvc -2*qs*qva;
    
    m[2][0] = 2*qva*qvc - 2*qs*qvb;
    m[2][1] = 2*qvb*qvc + 2*qs*qva;
    m[2][2] = 1 - 2*qva*qva - 2*qvb*qvb;
}

inline matrix3 matrix4::getRotation()
{
	matrix3 r;
	r.elements[0][0] = m[0][0];
	r.elements[0][1] = m[0][1];
	r.elements[0][2] = m[0][2];

	r.elements[1][0] = m[1][0];
	r.elements[1][1] = m[1][1];
	r.elements[1][2] = m[1][2];

	r.elements[2][0] = m[2][0];
	r.elements[2][1] = m[2][1];
	r.elements[2][2] = m[2][2];
	
	return r;
}

inline vec3 matrix4::getTranslation()
{
	vec3 t;
	t.elements[0] = m[0][3]; 
	t.elements[1] = m[1][3];  
	t.elements[2] = m[2][3];  

	return t;
}






inline void matrix4::transpose()
{
    real temp;
    for(int i=0; i<4;i++){
	for(int j=0;j<4;j++){
	    temp = m[i][j];
	    m[i][j] = m[j][i];
	    m[j][i] = temp;
	}
    }
}

inline void matrix4::invert()
{
    real oneOverDet;
    real 
	d00, d01, d02, d03,
	d10, d11, d12, d13,
	d20, d21, d22, d23,
	d30, d31, d32, d33;
    
    oneOverDet = 1/det();
	
    d00 =  
	  m[1][1] * ( m[2][2]*m[3][3] - m[2][3]*m[3][2] )
	- m[1][2] * ( m[2][1]*m[3][3] - m[2][3]*m[3][1] )
	+ m[1][3] * ( m[2][1]*m[3][2] - m[2][2]*m[3][1] );
    
    d01 =  
	  m[1][0] * ( m[2][2]*m[3][3] - m[2][3]*m[3][2] )
	- m[1][2] * ( m[2][0]*m[3][3] - m[2][3]*m[3][0] )
	+ m[1][3] * ( m[2][0]*m[3][2] - m[2][2]*m[3][0] );
    
    d02 =  
	  m[1][0] * ( m[2][1]*m[3][3] - m[2][3]*m[3][1] )
	- m[1][1] * ( m[2][0]*m[3][3] - m[2][3]*m[3][0] )
	+ m[1][3] * ( m[2][0]*m[3][1] - m[2][1]*m[3][0] );
    
    d03 = 
	  m[1][0] * ( m[2][1]*m[3][2] - m[2][2]*m[3][1] )
	- m[1][1] * ( m[2][0]*m[3][2] - m[2][2]*m[3][0] )
	+ m[1][2] * ( m[2][0]*m[3][1] - m[2][1]*m[3][0] );
    
    d10 =  
	  m[0][1] * ( m[2][2]*m[3][3] - m[2][3]*m[3][2] )
	- m[0][2] * ( m[2][1]*m[3][3] - m[2][3]*m[3][1] )
	+ m[0][3] * ( m[2][1]*m[3][2] - m[2][2]*m[3][1] );
    
    d11 =  
	  m[0][0] * ( m[2][2]*m[3][3] - m[2][3]*m[3][2] )
	- m[0][2] * ( m[2][0]*m[3][3] - m[2][3]*m[3][0] )
	+ m[0][3] * ( m[2][0]*m[3][2] - m[2][2]*m[3][0] );
    
    d12 = 
	  m[0][0] * ( m[2][1]*m[3][3] - m[2][3]*m[3][1] )
	- m[0][1] * ( m[2][0]*m[3][3] - m[2][3]*m[3][0] )
	+ m[0][3] * ( m[2][0]*m[3][1] - m[2][1]*m[3][0] );
    
    d13 = 
	  m[0][0] * ( m[2][1]*m[3][2] - m[2][2]*m[3][1] )
	- m[0][1] * ( m[2][0]*m[3][2] - m[2][2]*m[3][0] )
	+ m[0][2] * ( m[2][0]*m[3][1] - m[2][1]*m[3][0] );
    
    d20 = 
	  m[0][1] * ( m[1][2]*m[3][3] - m[1][3]*m[3][2] )
	- m[0][2] * ( m[1][1]*m[3][3] - m[1][3]*m[3][1] )
	+ m[0][3] * ( m[1][1]*m[3][2] - m[1][2]*m[3][1] );
    
    d21 = 
	  m[0][0] * ( m[1][2]*m[3][3] - m[1][3]*m[3][2] )
	- m[0][2] * ( m[1][0]*m[3][3] - m[1][3]*m[3][0] )
	+ m[0][3] * ( m[1][0]*m[3][2] - m[1][2]*m[3][0] );
    
    d22 = 
	  m[0][0] * ( m[1][1]*m[3][3] - m[1][3]*m[3][1] )
	- m[0][1] * ( m[1][0]*m[3][3] - m[1][3]*m[3][0] )
	+ m[0][3] * ( m[1][0]*m[3][1] - m[1][1]*m[3][0] );
    
    d23 = 
	  m[0][0] * ( m[1][1]*m[3][2] - m[1][2]*m[3][1] )
	- m[0][1] * ( m[1][0]*m[3][2] - m[1][2]*m[3][0] )
	+ m[0][2] * ( m[1][0]*m[3][1] - m[1][1]*m[3][0] );
        
    d30 = 
	  m[0][1] * ( m[1][2]*m[2][3] - m[1][3]*m[2][2] )
	- m[0][2] * ( m[1][1]*m[2][3] - m[1][3]*m[2][1] )
	+ m[0][3] * ( m[1][1]*m[2][2] - m[1][2]*m[2][1] );
    
    d31 = 
	  m[0][0] * ( m[1][2]*m[2][3] - m[1][3]*m[2][2] )
	- m[0][2] * ( m[1][0]*m[2][3] - m[1][3]*m[2][0] )
	+ m[0][3] * ( m[1][0]*m[2][2] - m[1][2]*m[2][0] );
    
    d32 = 
	  m[0][0] * ( m[1][1]*m[2][3] - m[1][3]*m[2][1] )
	- m[0][1] * ( m[1][0]*m[2][3] - m[1][3]*m[2][0] )
	+ m[0][3] * ( m[1][0]*m[2][1] - m[1][1]*m[2][0] );
    
    d33 = 
	  m[0][0] * ( m[1][1]*m[2][2] - m[1][2]*m[2][1] )
	- m[0][1] * ( m[1][0]*m[2][2] - m[1][2]*m[2][0] )
	+ m[0][2] * ( m[1][0]*m[2][1] - m[1][1]*m[2][0] );
    
    m[0][0] =   d00*oneOverDet;
    m[0][1] = - d10*oneOverDet;
    m[0][2] =   d20*oneOverDet;
    m[0][3] = - d30*oneOverDet;
    m[1][0] = - d01*oneOverDet;
    m[1][1] =   d11*oneOverDet;
    m[1][2] = - d21*oneOverDet;
    m[1][3] =   d31*oneOverDet;
    m[2][0] =   d02*oneOverDet;
    m[2][1] = - d12*oneOverDet;
    m[2][2] =   d22*oneOverDet;
    m[2][3] = - d32*oneOverDet;
    m[3][0] = - d03*oneOverDet;
    m[3][1] =   d13*oneOverDet;
    m[3][2] = - d23*oneOverDet;
    m[3][3] =   d33*oneOverDet;
    
    
}

inline real matrix4::det()
{
  //    real sd1, sd2, sd3, sd4;
    
//      sd1 = m[1][1] * (( m[2][2]*m[3][3] ) - ( m[2][3]*m[3][2] ));
//      sd2 = m[1][2] * (( m[2][3]*m[3][0] ) - ( m[2][0]*m[3][3] ));
//      sd3 = m[1][3] * (( m[2][0]*m[3][1] ) - ( m[2][1]*m[3][0] ));
//      sd4 = m[1][0] * (( m[2][1]*m[3][2] ) - ( m[2][2]*m[3][1] ));

//      return 
//  	( m[0][0]*sd1 ) - ( m[0][1]*sd2 ) + 
//  	( m[0][2]*sd3 ) - ( m[0][3]*sd4 ) ;



    return  m[0][0]*(m[1][1]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])
		     -m[1][2]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])
		     +m[1][3]*(m[2][1]*m[3][2]-m[2][2]*m[3][1]))
	-m[0][1]*(m[1][0]*(m[2][2]*m[3][3]-m[2][3]*m[3][2])
		  -m[1][2]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])
		  +m[1][3]*(m[2][0]*m[3][2]-m[2][2]*m[3][0]))
	+m[0][2]*(m[1][0]*(m[2][1]*m[3][3]-m[2][3]*m[3][1])
		  -m[1][1]*(m[2][0]*m[3][3]-m[2][3]*m[3][0])
		  +m[1][3]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]))
	-m[0][3]*(m[1][0]*(m[2][1]*m[3][2]-m[2][2]*m[3][1])
		  -m[1][1]*(m[2][0]*m[3][2]-m[2][2]*m[3][0])
		  +m[1][2]*(m[2][0]*m[3][1]-m[2][1]*m[3][0]));
    

}

/*
inline void matrix4::print()
{ 
    // doesn't inline for loops?
  printf("Printing matrix!\n");
  for (int row = 0; row < 4; row++) {
    printf("% 2.2f  % 2.2f  % 2.2f  % 2.2f\n",
	   m[row][0],
	   m[row][1],
	   m[row][2],
	   m[row][3]);
  }
}


*/

// typecast
inline matrix4::operator const real * () const
{
    return (real *)m;
}


/*############################################################################*/

inline void matrix4::asColMajor(real arr[16] )
{
    arr[0] = m[0][0];
    arr[1] = m[1][0];
    arr[2] = m[2][0];
    arr[3] = m[3][0];

    arr[4] = m[0][1];
    arr[5] = m[1][1];
    arr[6] = m[2][1];
    arr[7] = m[3][1];

    arr[8] = m[0][2];
    arr[9] = m[1][2];
    arr[10] = m[2][2];
    arr[11] = m[3][2];
    
    arr[12] = m[0][3];
    arr[13] = m[1][3];
    arr[14] = m[2][3];
    arr[15] = m[3][3];

}

/*
inline void matrix4::setTransformationToOGLArray(GLfloat trans_array[16])
{
	//OGL does it columnwise
	trans_array[0] = (GLfloat) m[0][0];
    trans_array[1] = (GLfloat) m[1][0];
    trans_array[2] = (GLfloat) m[2][0];
    trans_array[3] = (GLfloat) m[3][0];

    trans_array[4] = (GLfloat) m[0][1];
    trans_array[5] = (GLfloat) m[1][1];
    trans_array[6] = (GLfloat) m[2][1];
    trans_array[7] = (GLfloat) m[3][1];

    trans_array[8] = (GLfloat) m[0][2];
    trans_array[9] = (GLfloat) m[1][2];
    trans_array[10] = (GLfloat) m[2][2];
    trans_array[11] = (GLfloat) m[3][2];
    
    trans_array[12] = (GLfloat) m[0][3];
    trans_array[13] = (GLfloat) m[1][3];
    trans_array[14] = (GLfloat) m[2][3];
    trans_array[15] = (GLfloat) m[3][3];


}
*/





/*############################################################################*/
// operators

inline real matrix4::operator [] (int index) const
{
    return m[index/4][index%4];
}

inline real & matrix4::operator [] (int index)
{
    return m[index/4][index%4];
}

inline matrix4 & matrix4::operator = (const matrix4 & mat)
{
    // doesn't inline for loops?
    for(int i=0;i<4;i++){
	m[i][0] = mat.m[i][0];
	m[i][1] = mat.m[i][1];
	m[i][2] = mat.m[i][2];
	m[i][3] = mat.m[i][3];
    }
    return *this;
}

inline matrix4 & matrix4::operator += (const matrix4 & mat)
{ 
    // doesn't inline for loops?
    for(int i=0;i<4;i++){
	m[i][0] += mat.m[i][0];
	m[i][1] += mat.m[i][1];
	m[i][2] += mat.m[i][2];
	m[i][3] += mat.m[i][3];
    }
    return *this;
}

inline matrix4 & matrix4::operator -= (const matrix4 & mat)
{ 
    // doesn't inline for loops?
    for(int i=0;i<4;i++){
	m[i][0] -= mat.m[i][0];
	m[i][1] -= mat.m[i][1];
	m[i][2] -= mat.m[i][2];
	m[i][3] -= mat.m[i][3];
    }
    return *this;
}

inline matrix4 & matrix4::operator *= (const matrix4 & mat)
{
    matrix4 r;
    
    // Row 0 
    r.m[0][0] = m[0][0] * mat.m[0][0] 
	+ m[0][1] * mat.m[1][0]
	+ m[0][2] * mat.m[2][0]  
	+ m[0][3] * mat.m[3][0];
    
    r.m[0][1] = m[0][0] * mat.m[0][1]
	+ m[0][1] * mat.m[1][1]
	+ m[0][2] * mat.m[2][1]
	+ m[0][3] * mat.m[3][1];
    
    r.m[0][2] = m[0][0] * mat.m[0][2]
	+ m[0][1] * mat.m[1][2]
	+ m[0][2] * mat.m[2][2]
	+ m[0][3] * mat.m[3][2];
    
    r.m[0][3] = m[0][0] * mat.m[0][3]
	+ m[0][1] * mat.m[1][3]
	+ m[0][2] * mat.m[2][3]
	+ m[0][3] * mat.m[3][3];
    
	// R.Mow 1 
    r.m[1][0] = m[1][0] * mat.m[0][0]
	+ m[1][1] * mat.m[1][0]
	+ m[1][2] * mat.m[2][0]  
	+ m[1][3] * mat.m[3][0];
    
    r.m[1][1] = m[1][0] * mat.m[0][1]
	+ m[1][1] * mat.m[1][1]
	+ m[1][2] * mat.m[2][1]
	+ m[1][3] * mat.m[3][1];
    
    r.m[1][2] = m[1][0] * mat.m[0][2]
	+ m[1][1] * mat.m[1][2]
	+ m[1][2] * mat.m[2][2]
	+ m[1][3] * mat.m[3][2];
    
    r.m[1][3] = m[1][0] * mat.m[0][3]
	+ m[1][1] * mat.m[1][3]
	+ m[1][2] * mat.m[2][3]
	+ m[1][3] * mat.m[3][3];
    
	// R.Mow 2 
    r.m[2][0] = m[2][0] * mat.m[0][0]
	+ m[2][1] * mat.m[1][0]
	+ m[2][2] * mat.m[2][0]  
	+ m[2][3] * mat.m[3][0];
    
    r.m[2][1] = m[2][0] * mat.m[0][1]
	+ m[2][1] * mat.m[1][1]
	+ m[2][2] * mat.m[2][1]
	+ m[2][3] * mat.m[3][1];
    
    r.m[2][2] = m[2][0] * mat.m[0][2]
	+ m[2][1] * mat.m[1][2]
	+ m[2][2] * mat.m[2][2]
	+ m[2][3] * mat.m[3][2];
    
    r.m[2][3] = m[2][0] * mat.m[0][3]
	+ m[2][1] * mat.m[1][3]
	+ m[2][2] * mat.m[2][3]
	+ m[2][3] * mat.m[3][3];
    
	// r.mow 3 
    r.m[3][0] =  m[3][0] * mat.m[0][0]
	+ m[3][1] * mat.m[1][0]
	+ m[3][2] * mat.m[2][0]  
	+ m[3][3] * mat.m[3][0];
    
    r.m[3][1] =  m[3][0] * mat.m[0][1]
	+ m[3][1] * mat.m[1][1]
	+ m[3][2] * mat.m[2][1]
	+ m[3][3] * mat.m[3][1];
    
    r.m[3][2] =  m[3][0] * mat.m[0][2]
	+ m[3][1] * mat.m[1][2]
	+ m[3][2] * mat.m[2][2]
	+ m[3][3] * mat.m[3][2];
    
    r.m[3][3] =  m[3][0] * mat.m[0][3]
	+ m[3][1] * mat.m[1][3]
	+ m[3][2] * mat.m[2][3]
	+ m[3][3] * mat.m[3][3];
     
    *this = r;
    return *this;
    delete &r;
}





/*############################################################################*/
// global functions

/*############################################################################*/
// global operators

inline matrix4 mulmm(const matrix4 &m1, const matrix4 &m2){

	matrix4 mout;

   
    mout.m[0][0] = m1.m[0][0] * m2.m[0][0];
    mout.m[0][1] = m1.m[0][0] * m2.m[0][1];
    mout.m[0][2] = m1.m[0][0] * m2.m[0][2];
    mout.m[0][3] = m1.m[0][0] * m2.m[0][3];
    mout.m[0][0] += m1.m[0][1] * m2.m[1][0];
    mout.m[0][1] += m1.m[0][1] * m2.m[1][1];
    mout.m[0][2] += m1.m[0][1] * m2.m[1][2];
    mout.m[0][3] += m1.m[0][1] * m2.m[1][3];
    mout.m[0][0] += m1.m[0][2] * m2.m[2][0];
    mout.m[0][1] += m1.m[0][2] * m2.m[2][1];
    mout.m[0][2] += m1.m[0][2] * m2.m[2][2];
    mout.m[0][3] += m1.m[0][2] * m2.m[2][3];
    mout.m[0][0] += m1.m[0][3] * m2.m[3][0];
    mout.m[0][1] += m1.m[0][3] * m2.m[3][1];
    mout.m[0][2] += m1.m[0][3] * m2.m[3][2];
    mout.m[0][3] += m1.m[0][3] * m2.m[3][3];

   
    mout.m[1][0] = m1.m[1][0] * m2.m[0][0];
    mout.m[1][1] = m1.m[1][0] * m2.m[0][1];
    mout.m[1][2] = m1.m[1][0] * m2.m[0][2];
    mout.m[1][3] = m1.m[1][0] * m2.m[0][3];
    mout.m[1][0] += m1.m[1][1] * m2.m[1][0];
    mout.m[1][1] += m1.m[1][1] * m2.m[1][1];
    mout.m[1][2] += m1.m[1][1] * m2.m[1][2];
    mout.m[1][3] += m1.m[1][1] * m2.m[1][3];
    mout.m[1][0] += m1.m[1][2] * m2.m[2][0];
    mout.m[1][1] += m1.m[1][2] * m2.m[2][1];
    mout.m[1][2] += m1.m[1][2] * m2.m[2][2];
    mout.m[1][3] += m1.m[1][2] * m2.m[2][3];
    mout.m[1][0] += m1.m[1][3] * m2.m[3][0];
    mout.m[1][1] += m1.m[1][3] * m2.m[3][1];
    mout.m[1][2] += m1.m[1][3] * m2.m[3][2];
    mout.m[1][3] += m1.m[1][3] * m2.m[3][3];

  
    mout.m[2][0] = m1.m[2][0] * m2.m[0][0];
    mout.m[2][1] = m1.m[2][0] * m2.m[0][1];
    mout.m[2][2] = m1.m[2][0] * m2.m[0][2];
    mout.m[2][3] = m1.m[2][0] * m2.m[0][3];
    mout.m[2][0] += m1.m[2][1] * m2.m[1][0];
    mout.m[2][1] += m1.m[2][1] * m2.m[1][1];
    mout.m[2][2] += m1.m[2][1] * m2.m[1][2];
    mout.m[2][3] += m1.m[2][1] * m2.m[1][3];
    mout.m[2][0] += m1.m[2][2] * m2.m[2][0];
    mout.m[2][1] += m1.m[2][2] * m2.m[2][1];
    mout.m[2][2] += m1.m[2][2] * m2.m[2][2];
    mout.m[2][3] += m1.m[2][2] * m2.m[2][3];
    mout.m[2][0] += m1.m[2][3] * m2.m[3][0];
    mout.m[2][1] += m1.m[2][3] * m2.m[3][1];
    mout.m[2][2] += m1.m[2][3] * m2.m[3][2];
    mout.m[2][3] += m1.m[2][3] * m2.m[3][3];

    
    mout.m[3][0] = m1.m[3][0] * m2.m[0][0];
    mout.m[3][1] = m1.m[3][0] * m2.m[0][1];
    mout.m[3][2] = m1.m[3][0] * m2.m[0][2];
    mout.m[3][3] = m1.m[3][0] * m2.m[0][3];
    mout.m[3][0] += m1.m[3][1] * m2.m[1][0];
    mout.m[3][1] += m1.m[3][1] * m2.m[1][1];
    mout.m[3][2] += m1.m[3][1] * m2.m[1][2];
    mout.m[3][3] += m1.m[3][1] * m2.m[1][3];
    mout.m[3][0] += m1.m[3][2] * m2.m[2][0];
    mout.m[3][1] += m1.m[3][2] * m2.m[2][1];
    mout.m[3][2] += m1.m[3][2] * m2.m[2][2];
    mout.m[3][3] += m1.m[3][2] * m2.m[2][3];
    mout.m[3][0] += m1.m[3][3] * m2.m[3][0];
    mout.m[3][1] += m1.m[3][3] * m2.m[3][1];
    mout.m[3][2] += m1.m[3][3] * m2.m[3][2];
    mout.m[3][3] += m1.m[3][3] * m2.m[3][3];

	return mout;

}

inline vec3 mulvm(const vec3 &v, const matrix4 &m){

		vec3 temp;
		
		temp.elements[0] = m.m[0][0]*v.elements[0] + m.m[0][1]*v.elements[1] + m.m[0][2]*v.elements[2] + m.m[0][3]*1;
		temp.elements[1] = m.m[1][0]*v.elements[0] + m.m[1][1]*v.elements[1] + m.m[1][2]*v.elements[2] + m.m[1][3]*1;
		temp.elements[2] = m.m[2][0]*v.elements[0] + m.m[2][1]*v.elements[1] + m.m[2][2]*v.elements[2] + m.m[2][3]*1;
		//temp.elements[3] = m.m[3][0]*v.elements[0] + m.m[3][1]*v.elements[1] + m.m[3][2]*v.elements[2] + m.m[3][3]*v.elements[3];
		
		return temp;

}

inline matrix4 operator *(const matrix4 m, const real mult) 
{	//matrix * const multiplication
	matrix4 ret(0.0);
	
	for (short int i=0; i<4; i++)
	{
		for (short int j=0; j<4; j++)
		{
			real a = (m.m[i][j])*mult;
			ret.m[i][j] = a;
		}
	}
	return ret;
}
inline matrix4 operator *(const real mult, const matrix4 m) 
{	//matrix * const multiplication
	matrix4 ret(0.0);
	
	for (short int i=0; i<4; i++)
	{
		for (short int j=0; j<4; j++)
		{
			real a = (m.m[i][j])*mult;
			ret.m[i][j] = a;
		}
	}
	return ret;
}

inline vec3 operator * (const matrix4 & mat, const vec3 & vec)	//only rotates vector
{
    vec3 result;
    

    result.elements[0] =	mat[0]*vec[0] +
				mat[1]*vec[1] +
				mat[2]*vec[2] +
				mat[3];	//translational part
    
    result.elements[1] =	mat[4]*vec[0] +
				mat[5]*vec[1] +
				mat[6]*vec[2] +
				mat[7];//translational part
	        
    
    result.elements[2] =	mat[8]*vec[0] +
 				mat[9]*vec[1] +	
				mat[10]*vec[2] +
				mat[11];//translational part

	        
	//last row is (0,0,0,1) and wont be placed in vec
	// (homo-matrix at its best)

    return result;
}

inline vec3 operator ^ (const matrix4 & mat, const vec3 & vec) //rotate only
{
    vec3 result;
    

    result.elements[0] =	mat[0]*vec[0] +
				mat[1]*vec[1] +
				mat[2]*vec[2];
				//mat[3];	//translational part
    
    result.elements[1] =	mat[4]*vec[0] +
				mat[5]*vec[1] +
				mat[6]*vec[2];
				//mat[7];//translational part
	        
    
    result.elements[2] =	mat[8]*vec[0] +
 				mat[9]*vec[1] +	
				mat[10]*vec[2];
				//mat[11];//translational part

	        
	//last row is (0,0,0,1) and wont be placed in vec
	// (homo-matrix at its best)

    return result;
}
#pragma once
#ifndef _VEC3_H_
#define _VEC3_H_
#include <CommonTypes.h>
#include <Math.h>

/*
TODO: Implement unary ops


*/

class vec3  
{
public:
	inline vec3();
	inline vec3(real xval, real yval,real zval);

	inline real length(void);
	inline real lengthSquared(void);
	inline void set(real xval, real yval, real zval);
	inline void normalize(void);
	inline int equals( vec3 &Other );
	inline real operator ,(const vec3 &ScaledOn) const;
	
	inline vec3 operator *(const vec3 &Mult) const; //OBS! cross product
	inline vec3 operator *(const real scalar) const;
	inline vec3 operator /(const real d) const;
	inline vec3 operator -(const vec3 &Sub) const;
	inline vec3 operator - (const real scalar) const;
	inline vec3 operator +(const vec3 &Add) const;
	inline vec3 operator + (const real scalar) const;
	inline vec3 operator =(const vec3 &Other);

	//Unary operators
	inline vec3 operator -() const;

	inline vec3 & operator *= (const real scalar);
	inline vec3 & operator += (const vec3 &other);
	inline vec3 & operator -= (const vec3 &other);
	inline real operator [](const int index) const;
	inline real & operator [](const int index);
	inline operator float * () const; 
	inline operator double * () const;

	real elements[3];
};

//Globals
extern inline vec3 operator * (float f, const vec3 &a);
extern inline vec3 reflect(vec3 n, vec3 v);



inline vec3::vec3() {
	elements[0] = 0.0f; elements[1] = 0.0f; elements[2] = 0.0f;
}

inline vec3::vec3(real xval, real yval,real zval){
	elements[0] = xval;elements[1] = yval;elements[2] = zval;
}

//get length of vector
inline real vec3::length(void){
	return sqrt((elements[0]*elements[0] + elements[1]*elements[1] +elements[2]*elements[2]));
}

inline real vec3::lengthSquared(void){
	return (elements[0]*elements[0] + elements[1]*elements[1] +elements[2]*elements[2]);
}

inline void vec3::set(real xval, real yval, real zval){
	elements[0] = xval;elements[1] = yval;elements[2] = zval;
}

//normalize vector
inline void vec3::normalize(void){
	real l = length();	
	if (l != 0.0f){ 
		elements[0] = elements[0]/l;
		elements[1] = elements[1]/l;
		elements[2] = elements[2]/l;
	}
}

inline int vec3::equals( vec3 &Other ){

	if(elements[0] < Other.elements[0]*1.03f && elements[0] > Other.elements[0]*0.97f 
		&& elements[1] < Other.elements[1]*1.03f && elements[1] > Other.elements[1]*0.97f
		&& elements[2] < Other.elements[2]*1.03f && elements[2] > Other.elements[2]*0.97f)
		return 1;

	else
		return 0;
}

inline real vec3::operator ,(const vec3 &ScaledOn) const{
	//returns scalar product of two vectors
	return elements[0] * ScaledOn.elements[0] + 
		elements[1] * ScaledOn.elements[1] + 
		elements[2] * ScaledOn.elements[2];
}

inline vec3 vec3::operator *(const vec3 &Mult) const{
	//cross product on two vectors
	return vec3(this->elements[1] * Mult[2] - Mult[1] * this->elements[2], 
		this->elements[2] * Mult[0] - Mult[2] * this->elements[0], 
		this->elements[0] * Mult[1] - Mult[0] * this->elements[1]);
}

inline vec3 vec3::operator *(const real scalar) const{

	return vec3(this->elements[0]*scalar,this->elements[1]*scalar,this->elements[2]*scalar);
}

inline vec3 vec3::operator /(const real d) const{
	//divides vector elements with d
	return vec3(elements[0]/d,elements[1]/d,elements[2]/d);
}

inline vec3 vec3::operator -(const vec3 &Sub) const{
	//subtract two vectors
	vec3 ret(elements[0] - Sub.elements[0], elements[1] - Sub.elements[1], elements[2] - Sub.elements[2]);
	return ret;
}
inline vec3 vec3::operator - (const real scalar) const{
	vec3 ret(elements[0] - scalar, elements[1] - scalar, elements[2] - scalar);
	return ret;
}

inline vec3 vec3::operator +(const vec3 &Add) const{
	//add two vectors
	vec3 ret(elements[0] + Add.elements[0], elements[1] + Add.elements[1], elements[2] + Add.elements[2]);
	return ret;
}

inline vec3 vec3::operator + (const real scalar) const{
	vec3 ret(elements[0] + scalar, elements[1] + scalar, elements[2] + scalar);
	return ret;
}

inline vec3 vec3::operator =(const vec3 &Other){
	//set two vector equal
	this->elements[0] = Other.elements[0];
	this->elements[1] = Other.elements[1];
	this->elements[2] = Other.elements[2];
	return *this;
}

//Unary operator
inline vec3 vec3::operator -() const {
	vec3 ret(elements[0]*-1.0, elements[1]*-1.0, elements[2]*-1.0);
	return ret;
}

inline vec3 & vec3::operator *= (const real scalar){

	this->elements[0] *= scalar;
	this->elements[1] *= scalar;
	this->elements[2] *= scalar;
	return *this;
}

inline vec3 & vec3::operator += (const vec3 &other){

	this->elements[0] += other.elements[0];
	this->elements[1] += other.elements[1];
	this->elements[2] += other.elements[2];		
	return *this;
}

inline vec3& vec3::operator -= (const vec3 &other){

	this->elements[0] -= other[0];
	this->elements[1] -= other[1];
	this->elements[2] -= other[2];
	return *this;
}

inline real vec3::operator [](const int index) const{

	//if (index > 0 && index <3)
	return elements[index];
	//else 
	//	return 0;
}

inline real & vec3::operator [](const int index){

	//if (index > 0 && index <3)
	return elements[index];
	//else 
	//	return 0;
}

//Cast Vec3f a to const float *
inline vec3::operator float * () const {
	return (float*)&elements[0];
}

//Cast Vec3f a to const double *
inline vec3::operator double * () const {
	//double *apa = reinterpret_cast<double *>(&elements[0]);
	return (double *)&elements[0];
}


//GLOBALS
inline vec3 operator * (real scalar, const vec3 &a){
	vec3 ret(scalar*a[0], scalar*a[1], scalar*a[2]);
	return ret;
}


//	reflect the vector in n, the vector should be incident on reflection point
inline vec3 reflect(vec3 n, vec3 v){
	real length = v.length();
	n.normalize();
	v.normalize();
	vec3 r = (2.0*(n,-v)*n+v)*length;
	return r;
}
#endif

#pragma once
#include <vec3.h>
#include <matrix4.h>
#include <tRay.h>
#include "tShader.h"
#include <tIntersection.h>
#include <tUVmapper2D.h>
/*
	Abstract class tShape

	An interface definition for all shapes.
*/
class tShape
{
public:
	matrix4 transform;
    
	tShape(void) : shader(0), uvmapper(0) {};
	tShape(const tShape &c) : shader(0) {*this = c; };
	virtual ~tShape(void) {
		if (shader)
			delete shader;
		shader = 0;
	};

	tShape & operator= (const tShape &r) {
		this->setShader(r.shader);
		this->setUVMapper(r.uvmapper);
		transform = r.transform;
		return *this;
	};
	void setShader(tShader *s) { 
		if (s != 0) {
			if (shader)
				delete shader;
			shader = s->clone();	//abstract, so use clone
		}
	};
	void setUVMapper(tUVmapper2D *m) {
		if (m != 0) {
			if (uvmapper)
				delete uvmapper;
			uvmapper = m->clone();
		}
	};
	virtual tShape * clone() const = 0;	//pure virtual
	
	/*
		Returns the closest intersection of the ray
		with this tShape object. Returns -1 if no intersection found.

		If return value is > 0, hit contains the valid info for the intersections
	*/
	virtual double intersection(const tRay &ray, tIntersection &hit) { return -1.0; };

	/*
		A init method that is called every time
		before this object is rendered.

		Perfect for per frame rendering optimizations!
	*/
	virtual void init() { };
	/*
		Returns pointer to shader.
	*/
	tShader * getShader() {return shader; };
	
	
protected:
	tShader *shader;
	tUVmapper2D *uvmapper; //TODO: generalize this to 3D textures also
};

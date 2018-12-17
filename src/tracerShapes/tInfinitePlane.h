#pragma once
#include "tshape.h"
#include <tUVmapper2D.h>
/*
	tInfinitePlane class
		An implementation of tShape class.
		Representation of the linear equation:
			ax +by +cz +d = 0 
*/
class tInfinitePlane :
	public tShape
{
public:
	double a,b,c,d; 
	tInfinitePlane(void) {a=c=d=0; b=1; };
	tInfinitePlane(const tInfinitePlane &p) {
		*this = p;
	};
	tInfinitePlane & operator=(const tInfinitePlane &p) {
		a = p.a;
		b = p.b;
		c = p.c;
		d = p.d;
		this->setShader(p.shader);
		this->setUVMapper(p.uvmapper);
		this->transform = p.transform;
		return *this;
	};
	tInfinitePlane * clone() const {
		return new tInfinitePlane(*this);
	};
	~tInfinitePlane(void) {};
	void planeFromVertices(double4 &v0, double4 &v1, double4 &v2) {
		vec3 p0(v0.x, v0.y, v0.z),p1(v1.x,v1.y,v1.z),p2(v2.x, v2.y,v2.z);
		vec3 N = (p2-p0)*(p1-p0);
		N.normalize();
		a = N[0]; b = N[1]; c = N[2];
		d = - (p0 , N);
	};
	double distanceToPlane(const vec3 &p){
		//normal dot point
		double NdotP = a * p[0] + b*p[1] + c *p[2];
		return NdotP + d;
	};
	double distanceToPlane(const double4 &p){
		//normal dot point
		vec3 P(p.x, p.y,p.z);
		return distanceToPlane(P);
	};

	vec3 normal(vec3 &point) {
		return vec3(a,b,c);
	};
	double intersection(const tRay &ray, tIntersection &hit) {
		//unless Normal(a,b,c) and raydir is perpendicular
		// solve t = -(N dot raystart + d)/(N dot raydir)
		double NdotRayDir = a*ray.dir[0] + b*ray.dir[1] + c*ray.dir[2];
		
		if (fabs(NdotRayDir) > SMALL_VAL) {
			double t = - ( (a*ray.start[0] + b*ray.start[1] + c*ray.start[2]) +d ) / NdotRayDir;
			hit.N.set(a,b,c);
			hit.P = ray.start + ray.dir*t;
			return t;
		}
		else 
			return -1.0;
	};
};

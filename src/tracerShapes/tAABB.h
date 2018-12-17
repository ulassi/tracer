#pragma once
#include "tshape.h"
#include "tPolygon.h"

class tAABB :
	public tShape
{
public:
	double x_width, y_height, z_depth;
	vec3 cen;
	
	tAABB(void);
	tAABB(const tAABB &c);
    tAABB(const vec3 &min, const vec3 &max);
	virtual ~tAABB(void);
	tAABB & operator= (const tAABB &r);
	tAABB* clone() const;
	
	const vec3 & centre(); //returns vector to centre of box;
	bool pointInside(const vec3 &p);	//is the point inside box?
	bool lineIntersects(const vec3 &start, const vec3 &end);
	long facePoint(const vec3 &point);
	long edgePoint(const vec3 &point);
	long cornerPoint(const vec3 &point);
	void reset(const vec3 &min, const vec3 &max);
	bool rayIntersects(const tRay &ray);
	bool polyIntersects(tPolygon *p);
	
	//join two AABB's
	void join(tAABB* b);

	//overloaded from tShape
    virtual double intersection(const tRay &ray, tIntersection &hit);
};

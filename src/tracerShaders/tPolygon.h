#pragma once
#include <vector>
#include "typedefs.h"
#include "tShape.h"
class tPolygon : tShape
{
public:
	tPolygon(void);
	tPolygon(const tPolygon &p);
	virtual ~tPolygon(void);
	tPolygon& operator= (const tPolygon& right);
	tPolygon *clone() const { return new tPolygon(*this);};

	std::vector<double4*>		vertices;		//vertex coordinates
	std::vector<double2>		textureCoords;	//uv texture coordinates, for each vertex
	std::vector<unsigned int>	vertexIndicies; //to keep topology, keep indicies too
	vec3						polyNormal;		//index to polyNormal
	std::vector<double4>		vertexNormals;	//correct vertex normals for this poly
	unsigned int				surfaceIndex;
	/*
		Checks ray triangle
	*/
	int intersectTriangle(double orig[3], double dir[3],
	  double vert0[3], double vert1[3], double vert2[3],
	  double *t, double *u, double *v);
	/*
		Retrieves the geometric normal at a point.
	*/
	virtual vec3 normal(vec3 &point);

	/*
		Returns the closest intersection of the ray
		with this tShape object. Returns -1 if no intersection found.
	*/
	double intersection(const tRay &ray, tIntersection &hit);
};

#include ".\tpolygon.h"

tPolygon::tPolygon(void)
{
}

tPolygon::~tPolygon(void)
{
}
tPolygon::tPolygon(const tPolygon &p) {
	*this = p;
}
tPolygon& tPolygon::operator= (const tPolygon& right) {
	this->polyNormal = right.polyNormal;
	this->vertices = right.vertices;
	this->vertexNormals = right.vertexNormals;
	this->vertexIndicies = right.vertexIndicies;
	this->surfaceIndex = right.surfaceIndex;
	this->setShader(right.shader);
	this->setUVMapper(right.uvmapper);
	this->transform = right.transform;
	return (*this);
}
/*
		Retrieves the geometric normal at a point.
*/
vec3 tPolygon::normal(vec3 &point) {
	return this->polyNormal;	
}
/*
	Returns the closest intersection of the ray
	with this tShape object. Returns -1 if no intersection found.
*/
double tPolygon::intersection(const tRay &ray, tIntersection &hit) {
	double t,u,v;
	double4 *v0 = vertices.at(0);
	double4 *v1 = vertices.at(1);
	double4 *v2 = vertices.at(2);
	if (this->intersectTriangle((double*) &ray.start.elements[0],(double*) &ray.dir.elements[0],
		&v0->x,	//v0
		&v1->x,	//v1
		&v2->x,	//v2
		&t,&u,&v)) 
	{
		vec3 n1, n2, n3;
		double4 tmp;
		tmp = vertexNormals.at(0);
		n1.set(tmp.x, tmp.y, tmp.z);
		tmp = vertexNormals.at(1);
		n2.set(tmp.x, tmp.y, tmp.z);
		tmp = vertexNormals.at(2);
		n3.set(tmp.x, tmp.y, tmp.z);
		hit.N = (1.0-u-v)*n1 + u*n2 + v*n3;
		hit.P = ray.start + ray.dir * t;
		hit.surfaceIndex = this->surfaceIndex;
		
		double4 coords;
		coords.z = coords.w = coords.x = coords.y = 0;
		//at least 2 uv's per vertex, and minimum 3 verts
		if (this->textureCoords.size() > 0 || this->textureCoords.size() >= 3) {
			coords.x = u*textureCoords.at(0).x + v*textureCoords.at(1).x + (1-u-v)*textureCoords.at(1).x;
			coords.y = u*textureCoords.at(0).y + v*textureCoords.at(1).y + (1-u-v)*textureCoords.at(1).y;
			hit.uvs.at(0) = coords;
		}
		return t;
	}
	else
		return -1.0;
}

#define CROSS(dest,v1,v2) \
	dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
	dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
	dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
	dest[0]=v1[0]-v2[0]; \
	dest[1]=v1[1]-v2[1]; \
	dest[2]=v1[2]-v2[2];
#define EPSILON 0.00001
//#define TEST_CULL
  
/*
  Definitions:
		V0 vertex 0
		V1 vertex 1
		V2 vertex 2
		orig	shot ray from
		dir		in direction 
		t = distance to plane
		u,v = position on triangle:
		T(u,v) = (1-u-v)*V0 + u*V1 + v*V2
*/
int tPolygon::intersectTriangle(double orig[3], double dir[3],
	  double vert0[3], double vert1[3], double vert2[3],
	  double *t, double *u, double *v)
{

	double edge1[3], edge2[3], tvec[3],	pvec[3], qvec[3];
	double det,inv_det;
		
	/* find	vectors	for	two	edges	sharing	vert0 */
	SUB(edge1, vert1,	vert0);
	SUB(edge2, vert2,	vert0);

	/* begin calculating determinant - also	used to	calculate	U parameter	*/
	CROSS(pvec,	dir, edge2);

	/* if	determinant	is near	zero, ray lies in plane	of triangle	*/
	det	=	DOT(edge1, pvec);

#ifdef TEST_CULL		   /* define TEST_CULL if culling is desired */
	if (det	<	EPSILON)
		return 0;

	/* calculate distance	from vert0 to ray origin */
	SUB(tvec,	orig, vert0);

	/* calculate U parameter and test	bounds */
	*u = DOT(tvec, pvec);
	if (*u < 0.0 ||	*u > det)
		return 0;

	/* prepare to	test V parameter */
	CROSS(qvec,	tvec,	edge1);

	/* calculate V parameter and test	bounds */
	*v = DOT(dir,	qvec);
	if (*v < 0.0 ||	*u + *v	>	det)
		return 0;

	/* calculate t,	scale	parameters,	ray	intersects triangle	*/
	*t = DOT(edge2,	qvec);
	inv_det	=	1.0	/ det;
	*t *=	inv_det;
	*u *=	inv_det;
	*v *=	inv_det;
#else					 /*	the	non-culling	branch */
	if (det	>	-EPSILON &&	det	< EPSILON)
		return 0;
	inv_det	=	1.0	/ det;

	/* calculate distance	from vert0 to ray origin */
	SUB(tvec,	orig, vert0);

	/* calculate U parameter and test	bounds */
	*u = DOT(tvec, pvec) * inv_det;
	if (*u < 0.0 ||	*u > 1.0)
		return 0;

	/* prepare to	test V parameter */
	CROSS(qvec,	tvec,	edge1);

	/* calculate V parameter and test	bounds */
	*v = DOT(dir,	qvec) *	inv_det;
	if (*v < 0.0 ||	*u + *v	>	1.0)
		return 0;

	/* calculate t,	ray	intersects triangle	*/
	*t = DOT(edge2,	qvec)	* inv_det;
#endif
	return 1;
}
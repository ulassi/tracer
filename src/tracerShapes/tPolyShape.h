#pragma once
/*
	Lowest complexity object is a polygonal object
*/
#include "tPolygon.h"
#include "typedefs.h"
#include <matrix4.h>
#include "tShape.h"
#include "tSphere.h"
#include "tInfinitePlane.h"
#include "tBSPtree.h"
#include "tBVH.h"
#include "tAABB.h"
class tPolyShape : public tShape
{
public:
	//Object To World Transform
	//matrix4 mTransform;
	
	//geometry definitions
	std::vector<unsigned int>	mPolygonSurfaceIndex;	//one per polygon, index into surfaceArray
	std::vector<tPolygon>		mPolygonArray;			
	std::vector<double4>		mVertexArray;
	std::vector<double4>		mVertexNormalArray;
	std::vector<double4>		mPolygonNormalArray;

	tSphere boundingSphere;

	tPolyShape(void);
	tPolyShape(const tPolyShape &o);
	~tPolyShape(void);
	tPolyShape & operator= (const tPolyShape &right);
	tPolyShape * clone() const {return new tPolyShape(*this);};
	
	/*
		Retrieves the geometric normal at a point.
	*/
	virtual vec3 normal(vec3 &point);


	/*
		Returns the closest intersection of the ray
		with this tShape object. Returns -1 if no intersection found.
	*/
	virtual double intersection(const tRay &ray, tIntersection &hit);

	/* 
		Optimizing methods:
	*/

	//Init calls BSP/BVH builders
	virtual void init();
	
private:
	tBSPtree mPolygonTree;
	tBVH	 mBVH;

	//for BSP, planes to choose from
	tInfinitePlane planes[3];
	//debuggo
    unsigned int numPolysBSP;
	unsigned int numPolysBVH;
	/*
		Removes all polys from polyList and inserts them into tree.
	*/
	void addPolysToTree(tBSPtree *tree, std::list<tPolygon> &polyList, unsigned int maxDepth);
	void addPolysToBVH(tBVH &node, tAABB &nodeBox,std::vector<tPolygon*> &primVec) ;
	/*
		Builds a BSPtree for all the polygons in mPolygonArray
	*/
	void buildBSP();

	/*
		Builds a BVH volume
	*/
    void buildBVH();
	
};

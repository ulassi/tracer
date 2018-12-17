#pragma once
#ifndef _TBVH_H_
#define _TBVH_H_

#include <vector>
#include "tShape.h"
#include "tAABB.h"

/*
	BVH class (Bounding Volume Hierachy)

	Really a AABB tree (octree).
*/
class tBVH
{
public:
	//data members
	std::vector<tBVH *>		children;
	std::vector<tPolygon*>	prims;
	tAABB*					BV;
	unsigned int			maxPrimsInNode;
	unsigned int			depth;

    //Construction / destruction
	tBVH(void);
	tBVH(unsigned int _depth);
	tBVH(const tBVH &bvh);
	~tBVH(void);

	tBVH & operator= (const tBVH &r);
	tBVH * clone() const;

	//MEMBERS
	double intersection(const tRay &ray, tIntersection &hit);	
};

#endif
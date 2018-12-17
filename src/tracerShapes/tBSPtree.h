#pragma once
/*
	BSP tree for tPolygons.
*/
#include <tInfinitePlane.h>
#include <tShape.h>
#include <typedefs.h>
#include <list>
#include <vector>
#include <tPolygon.h>
class tBSPtree
{
public:
	//data members
	tInfinitePlane plane;
	tBSPtree *front,*back;
	std::list<tPolygon> shapes;
	unsigned int depth;
	
	tBSPtree(void) : front(0), back(0), depth(0){};
	tBSPtree(const tBSPtree &tree): front(0), back(0), depth(0) {
		*this = tree;
	};
	tBSPtree(unsigned int d): front(0), back(0), depth(0) {
		depth = d;
	};
	tBSPtree & operator= (const tBSPtree &c) {
		plane = c.plane;
		if (c.front)
			front = c.front->clone();
		if (c.back)
			back = c.back->clone();
		shapes = c.shapes;
		return *this;
	};
	tBSPtree * clone() const{
		return new tBSPtree(*this);
	};
	~tBSPtree(void) { 
		if (front)
			delete front;
		if (back)
			delete back;
		front = back = 0;
	};
	inline double intersectWithThis(const tRay &ray, tIntersection &hit)
	{
		//test all polys in this node
		tIntersection tmpHit;
		double closestHit=LARGE_VAL,t;
		bool _hit = false;
		std::list<tPolygon>::iterator pIt;
		pIt = this->shapes.begin();
		while (pIt != this->shapes.end()) {
			t = (*pIt).intersection(ray, tmpHit);
			if (t > 0 && t< closestHit) {
				_hit = true;
				closestHit = t;
			}
			pIt++;
		}
		/*std::list<tPolygon*>::iterator pIt = shapes.begin();
		while (pIt != shapes.end()) {
			t = (*pIt)->intersection(ray, tmpHit);
			if (t > 0 && t< closestHit) {
				_hit = true;
				closestHit = t;
			}
			pIt++;
		}*/
		if (_hit) {
			hit = tmpHit;
			//if ( (tmpHit.N , ray.dir) < 0)
				return closestHit;
			//else 
			//	return -1;
		}
		else
			return -1;
	}
	inline double intersection (const tRay &ray, tIntersection &hit) {
		if (front == 0 && back == 0) {
			//It's a leaf test all polys in this buggar
			return intersectWithThis(ray, hit);
			/*tIntersection tmpHit;
			double closestHit=LARGE_VAL,t;
			bool _hit = false;
			std::list<tPolygon>::iterator pIt = shapes.begin();
			while (pIt != shapes.end()) {
				t = (*pIt).intersection(ray, tmpHit);
				if (t > 0 && t< closestHit) {
					_hit = true;
					closestHit = t;
				}
				pIt++;
			}
			if (_hit) {
				hit = tmpHit;
				return closestHit;
			}
			else
				return -1;*/
		}
		else {
			//classify ray
			int c = this->classify(ray);
			/*
				0	= on plane
				2	= on and in front of plane
				1	= in front of plane
			   -1	= behind plane
			   -2	= on and behind
			  -10	= cuts plane
			*/
			if (c == 0) {
				bool b = true;
			}
			switch (c) {
				case 2:		//on and in front
				case 1:		//in front
					return front->intersection(ray, hit);
					break;
				case -2:	//on and behind
				case -1:	//behind
					return back->intersection(ray, hit);
					break;
				//case 2:
				//case -2:
				case 0:
					return -1;
					break;
				case -10:	//cuts
					{	//test both, and return closest hit
						
						double tf,tb, t= LARGE_VAL;
						bool _front=false, _back=false, _this = false;;
						
						tIntersection fhit, bhit;
						tf = front->intersection(ray, fhit);
						if (tf > 0) {
							//front is best candidate
							_front = true;
							t = tf;
						}

						tb = back->intersection(ray, bhit);
						if (tb > 0 && tb < t) {
							//choose backvalue
							_back = true;
							_front = false;
							t = tb;
						}
						if (_front)
							hit = fhit;
						else if (_back)
							hit = bhit;
						else
							t = -1;
						return t;
					}
					break;
			}
			
		}
	};
	/*
		0 = both
		2 = on and in front of plane
		1 = in front of plane
		-1 = behind plane
		-2 = on and behind
		-10 = cuts plane
	*/
	inline int classify(const tRay &ray) {
		double dist = 0;
		dist = plane.distanceToPlane(ray.start);
		if (dist > SMALL_VAL) {
			//start in front
			double NdotDir = (ray.dir , plane.normal(vec3()));
			if (NdotDir > SMALL_VAL ) {
				//all in front
				return 1;
			}
			else if (NdotDir < -SMALL_VAL)
				//can hit in both
				return -10;
			else 
				return 2;
		}
		else if (dist < -SMALL_VAL) {
			//start in back
			double NdotDir = (ray.dir ,plane.normal(vec3()));
			if (NdotDir < -SMALL_VAL) {
				//all in back
				return -1;
			}
			else if (NdotDir > SMALL_VAL)
				//can cut both
				return -10;
			else
				return -2;
		}
		else {
			double NdotDir = (ray.dir ,plane.normal(vec3()));
			if (NdotDir > SMALL_VAL)
				return 2;
			else if (NdotDir < -SMALL_VAL)
				return -2;
			else
				return 0;
		}
		

	};
	inline int classify(const tPolygon &poly) {
		int frontVerts=0, backVerts=0, tangVerts=0;
		std::vector<double4*>::const_iterator vIt = poly.vertices.begin();
		while (vIt != poly.vertices.end()) {
			double dist = plane.distanceToPlane(*(*vIt));
			if ( dist > SMALL_VAL) {
				//front
				frontVerts++;
			}
			else if (dist < -SMALL_VAL){ 
				//back
				backVerts++;
			}
			else {
				tangVerts++;
			}
			vIt++;
		}
		if (backVerts == 0 && tangVerts == 0) 
			return 1;
		else if (tangVerts == 0 && frontVerts  == 0) 
			return -1;
		else if (frontVerts == 0 && backVerts == 0)
			return 0;// on the buggar
		else if (frontVerts == 0)
			return -2;
		else if (backVerts == 0)
			return 2;
		else
			return -10;
		
	};
	
};

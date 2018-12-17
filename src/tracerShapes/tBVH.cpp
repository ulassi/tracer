#include "tbvh.h"
//CONSTRUCTORS
tBVH::tBVH(void): BV(0), maxPrimsInNode(100),depth(0) {}
tBVH::tBVH(unsigned int _depth): BV(0), maxPrimsInNode(100) { depth = _depth;}
tBVH::tBVH(const tBVH &bvh) {*this = bvh;}

//DESTRUCTOR
tBVH::~tBVH(void) {
	while (!children.empty()) {
		tBVH *child = children.back();
		children.pop_back();
		delete child;
	}
	if (BV)
		delete BV;
	while (!prims.empty()) {
		tPolygon *prim = prims.back();
		prims.pop_back();
		delete prim;
	}
}

//MEMBER METHODS
tBVH & tBVH::operator= (const tBVH &r){
	for(size_t i=0; i<r.children.size(); i++) {
		this->children.push_back(r.children[i]->clone());
	}
	for (size_t i=0; i<r.prims.size(); i++) {
		this->prims.push_back(r.prims[i]->clone());
	}
	if (r.BV)
		BV = r.BV->clone();
	else
		BV = 0;
	maxPrimsInNode = r.maxPrimsInNode;
	depth = r.depth;
	return *this;
};
tBVH * tBVH::clone() const {
	return new tBVH(*this);
};
double tBVH::intersection(const tRay &ray, tIntersection &hit) {
	if (children.size() == 0) {
		//We have a leaf, check prims
		double t=-1, closestDist = LARGE_VAL;
		bool _hit = false;
		tIntersection tmpHit;
		std::vector<tPolygon*>::iterator pIt = 
			prims.begin();
		while(pIt != prims.end()) {
			t = (*pIt)->intersection(ray, tmpHit);
			if (t>0 && t<closestDist) {
				hit = tmpHit;
				closestDist = t;
				_hit = true;
			}
			pIt++;
		}
		if (_hit) {
			return closestDist;
		}
		else 
			return -1;
	}
	else {
		if (BV->rayIntersects(ray)) {
			//we hit the BV, iterate and test children
			std::vector<tBVH*>::iterator bvhIt = 
				children.begin();
			tIntersection tmpHit;
			double closestDist = LARGE_VAL, t = -1;
			bool _hit = false;
			while (bvhIt != children.end()) {
				t = (*bvhIt)->intersection(ray, tmpHit);
				if (t > 0 && t<closestDist) {
					hit = tmpHit;
					closestDist  = t;
					_hit = true;
				}
				bvhIt++;
			}
			if (_hit) {
				return closestDist;
			}
			else 
				return -1;
		}
		else
			return -1;
	}
}
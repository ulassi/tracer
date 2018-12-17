#pragma once
#include <vec3.h>
#include <vector>
#include "typedefs.h"
class tIntersection
{
public:
	tIntersection(void){};
	tIntersection(const tIntersection &i){ *this = i;};
	~tIntersection(void){};
	tIntersection & operator= (const tIntersection &right) {
		P = right.P;
		N = right.N;
		uvs = right.uvs;
		surfaceIndex = right.surfaceIndex;
		return *this;
	};

	//Member attributes
    vec3	P;
	vec3	N;
	std::vector<double4>	uvs;	//pairs of UV's (u,v,s,t), index corresponding to which set number
	unsigned int	surfaceIndex;	//for complex polyshapes, keeps the index to a surface shader
};

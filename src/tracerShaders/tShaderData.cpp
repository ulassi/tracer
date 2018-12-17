#include "tshaderdata.h"
#include "tTracer.h"
//IMPLEMENTATION
tShaderData::tShaderData(
		const vec3 &Li, 
		const tColor &LCol, 
		const vec3 &E, 
		const tTracer &ren, 
		unsigned int d, 
		const tIntersection &isec) 
{
	incidentLightingVec = Li;
	lightColor = LCol;
	viewVec = E;
	renObj = ren;
	depth = d;
	intersection = isec;
};
tShaderData & tShaderData::operator = (const tShaderData &right) {
	incidentLightingVec = right.incidentLightingVec;
	lightColor = right.lightColor;
	viewVec = right.viewVec;
	renObj = right.renObj;
	depth = right.depth;
	intersection = right.intersection;
	return *this;
};
#pragma once
/*
	Data container for all data communicated via tShader::evaluate() / transmit()
	All vectors are pointing AWAY from position.
*/
#include "tColor.h"
#include "vec3.h"
#include "tIntersection.h"
#include "tTracer.h"

class tShaderData {
public:
	//DATA MEMBERS
	vec3 			incidentLightingVec;
	tColor 			lightColor;
	vec3 			viewVec;
   	tTracer 		renObj;
	unsigned int	depth;
	tIntersection 	intersection;

	//METHOD MEMBERS
	tShaderData(void) {};
	tShaderData(const vec3 &Li, 
		const tColor &LCol, 
		const vec3 &E, 
		const tTracer &ren, 
		unsigned int d, 
		const tIntersection &isec);
	tShaderData(const tShaderData &d) { *this = d;};
	tShaderData & operator = (const tShaderData &right);
	~tShaderData(void) {};
};


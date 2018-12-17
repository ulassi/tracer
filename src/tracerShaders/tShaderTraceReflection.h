#pragma once
#include "tshader.h"
#include "tShaderConstant.h"
/*
	A raytracing shader for reflection
*/

class tShaderTraceReflection :
	public tShader
{
public:
	//PLUG LIST
	const static std::string reflectPlug;

	tShaderTraceReflection(void) {
		this->inputPlugs.connect(reflectPlug, &tShaderConstant(tColor(0.5)));
	};
	tShaderTraceReflection(double refl){ 
		this->inputPlugs.connect(reflectPlug,&tShaderConstant(tColor(refl)));
	};
	tShaderTraceReflection(const tShaderTraceReflection &s) { *this = s;};
	tShaderTraceReflection & operator= (const tShaderTraceReflection &s) {
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShaderTraceReflection(void) {};
	tShaderTraceReflection * clone() const { return new tShaderTraceReflection(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		vec3 R;
		data.renObj.reflect(data.intersection.N, data.viewVec,R);
		tRay reflRay(data.intersection.P, R);
		return this->inputPlugs.evaluatePlug(reflectPlug, data)*
			data.renObj.RayTrace(reflRay, data.depth+1);
	};

	virtual tColor transmit(const tShaderData &data) {
		return tColor(0);
	};
};

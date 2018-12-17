#pragma once
#include "tshader.h"
#include "tShaderConstant.h"
/*
	A raytracing shader for refractive material
*/

class tShaderTraceRefraction :
	public tShader
{
public:
	//PLUG LIST
	const static std::string plugTransparency;
	const static std::string plugIor;

	tShaderTraceRefraction(void) {
		this->inputPlugs.connect(plugTransparency, &tShaderConstant(tColor(0.7)));
		this->inputPlugs.connect(plugIor, &tShaderConstant(tColor(1.3)));
	};
	tShaderTraceRefraction(double transp, double ior){ 
		this->inputPlugs.connect(plugTransparency, &tShaderConstant(tColor(transp))); 
		this->inputPlugs.connect(plugIor, &tShaderConstant(tColor(ior))); 
	};
	tShaderTraceRefraction(const tShaderTraceRefraction &s) { *this = s;};
	tShaderTraceRefraction & operator= (const tShaderTraceRefraction &s) {
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShaderTraceRefraction(void) {};
	tShaderTraceRefraction * clone() const { return new tShaderTraceRefraction(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		vec3 R;
		data.renObj.refract(data.intersection.N, data.viewVec,R,(double) this->inputPlugs.evaluatePlug(plugIor,data));
		tRay refrRay(data.intersection.P, R);
		return this->inputPlugs.evaluatePlug(plugTransparency,data)*
			data.renObj.RayTrace(refrRay, data.depth+1);
	};

	virtual tColor transmit(const tShaderData &data) {
		return inputPlugs.transmitPlug(plugTransparency,data);
	};
};

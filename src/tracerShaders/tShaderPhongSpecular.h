#pragma once
#include "tshader.h"
#include "tTracer.h"
#include "tShaderConstant.h"
/*
	A shader that returns phong specular:
		result = LightCol * SpecularCol * (R,E)^n
*/


class tShaderPhongSpecular :
	public tShader
{
public:
	//PLUG LIST
	const static std::string specColPlug;
	const static std::string expPlug;

	tShaderPhongSpecular(void){
		this->inputPlugs.connect(specColPlug, &tShaderConstant(tColor(1)));
		this->inputPlugs.connect(expPlug, &tShaderConstant(tColor(32)));
	};
	tShaderPhongSpecular(const tColor &_color, unsigned int exponent) { 
		this->inputPlugs.connect(specColPlug, &tShaderConstant(_color));
		this->inputPlugs.connect(expPlug, &tShaderConstant(tColor(exponent)));
	};
	virtual ~tShaderPhongSpecular(void){};
	tShaderPhongSpecular(const tShaderPhongSpecular &s) { *this = s;};
	tShaderPhongSpecular & operator= (const tShaderPhongSpecular &s) {
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	tShaderPhongSpecular * clone() const { return new tShaderPhongSpecular(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		vec3 R;
		data.renObj.reflect(data.intersection.N, data.incidentLightingVec, R);
		double EdotR = max(0,(data.viewVec, R));
		
        return data.lightColor*
			this->inputPlugs.evaluatePlug(specColPlug,data)*
			pow(EdotR,(double)this->inputPlugs.evaluatePlug(expPlug,data)) ;
	};

	virtual tColor transmit(const tShaderData &data) {
		return tColor(0);
	};
};

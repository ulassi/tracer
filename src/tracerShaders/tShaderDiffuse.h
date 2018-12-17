#pragma once
#include "tshader.h"
#include "tShaderConstant.h"
#include <string>
/*
	Simple shader that does lambertian dot product shading
		result = DiffuseColor*max(0,dot(N,L))
*/

class tShaderDiffuse :
	public tShader
{
public:
	//PLUG LIST
	const static std::string plugDiffuseColor;	//DiffuseColor

	tShaderDiffuse(void);
	tShaderDiffuse(const tColor &col);
	tShaderDiffuse(const tShaderDiffuse &s);
	tShaderDiffuse & operator= (const tShaderDiffuse &s);
	virtual ~tShaderDiffuse(void);
	tShaderDiffuse * clone() const;

	virtual tColor evaluate(const tShaderData &data)
	{
		return data.lightColor*
			this->inputPlugs.evaluatePlug(plugDiffuseColor, data)*
			max(0,(data.intersection.N, data.incidentLightingVec));
	};
	virtual tColor transmit(const tShaderData &data) {
		return tColor(0);
	};
};


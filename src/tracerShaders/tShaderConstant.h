#pragma once
#include "tshader.h"
/*
	A constant shader color
		result = constColor;

	This is a typical source shader, does not
	have any plugs. Just a constant
*/
class tShaderConstant :
	public tShader
{
public:
	tColor constColor;
	tShaderConstant(void) {};
	tShaderConstant(const tColor &c){ constColor = c;};
	tShaderConstant(const tShaderConstant &s) { *this = s;};
	tShaderConstant & operator= (const tShaderConstant &s) {
		this->constColor = s.constColor;
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShaderConstant(void) {};
	tShaderConstant * clone() const { return new tShaderConstant(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		return constColor;
	};

	virtual tColor transmit(const tShaderData &data) {
		return constColor;;
	};
};

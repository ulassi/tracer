#pragma once
#include "tshader.h"
/*
	A 3d texture generator, sucks, Karls first attempt.
	Don't use this for anything other than maybe to start 
	from when implementing a real class

	This is a typical source shader, does not have any 
	plugs. generates shit.
*/
class tShader3DChecker :
	public tShader
{
public:
	tColor color1, color2;
	tShader3DChecker(void): color1(tColor(0.0f, 0.0f, 0.0f)), color2(tColor(1.0f, 1.0f, 1.0f)) {};
	tShader3DChecker(const tColor &c1, const tColor &c2): color1(c1), color2(c2) {};
	tShader3DChecker(const tShaderConstant &s) { *this = s;};
	tShader3DChecker & operator= (const tShader3DChecker &s) {
		this->color1 = s.color1;
		this->color2 = s.color2;
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShader3DChecker(void) {};
	tShader3DChecker * clone() const { return new tShader3DChecker(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		
		vec3 P = data.intersection.P;
		int x = static_cast<int>(floor(P.elements[0]));
		int y = static_cast<int>(floor(P.elements[1]));
		int z = static_cast<int>(floor(P.elements[2]));
		if(x%2)
			if(z%2)	
				if(y%2)return color1;
				else return color2;
			else if(y%2)return color2;
				else return color1;
		else 
			if(z%2)	
				if(y%2)return color2;
				else return color1;
			else if(y%2)return color1;
				else return color2;
	};

	virtual tColor transmit(const tShaderData &data) {
		return color1;
	};
};
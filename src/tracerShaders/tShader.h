#pragma once
/*
	Abstract Base class for all shaders.

	transmit(),shade() may be recursive methods, depending on the shader

	Sources typically return a specific value directly, where filers
	get colors from internal shaders and compose a new color.
	
*/
#include "tShaderPlugs.h"
#include "tShaderData.h"

class tShader
{
public:
	tShader(void){};
	tShader(const tShader &s) { *this = s;};
	virtual tShader & operator= (const tShader &right) { 
		inputPlugs = right.inputPlugs;
		return *this; 
	};
	virtual tShader * clone() const = 0;
	virtual ~tShader(void) {};

	/*
		Shades a point using this shader (-network)
		Returns a color.
	*/
    virtual tColor evaluate(const tShaderData &data)  = 0;
	/*
		Returns a color value used for shadows.
	*/
	virtual tColor transmit(const tShaderData &data)  = 0;

	/*
		Connect this plug (if it exists) to a copy
		of the supplied shader.
	*/
	virtual void connect(const std::string &plugName, tShader *shader) {
		inputPlugs.connect(plugName, shader);
	};
protected:
	//Holds all plugs for a single shader
	tShaderPlugs	inputPlugs;	
};


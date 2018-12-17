#pragma once

#include <map>
#include <string>
//#include "tShader.h"
class tColor;
class tShader;
class tShaderData;

class tShaderPlugs {
public:
	tShaderPlugs(void) {};
	~tShaderPlugs(void);
	tShaderPlugs(const tShaderPlugs &p) {*this = p;};
	tShaderPlugs & operator= (const tShaderPlugs& right);

	/*
		Connects a copy of the shader to the plugName key in plugMap.

		Only connects already added plugs.
	*/
	void connect(const std::string &plugName, tShader *shader);

	/*
		Methods to retrieve data from the plugs
	*/
	tColor evaluatePlug(const std::string &plugName, const tShaderData &data);
	tColor transmitPlug(const std::string &plugName, const tShaderData &data);

	/*
		Adds a plug
	*/
	void addPlug(const std::string &name);
private:
	std::map<std::string,tShader *>	plugMap;
};

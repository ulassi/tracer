#pragma once
#include "tshader.h"
#include <algorithm>
class tShaderOperator :
	public tShader
{
public:
	tShaderOperator(void);
	tShaderOperator(const tShaderOperator &c);
	virtual ~tShaderOperator(void);

	tShaderOperator & operator= (const tShaderOperator &right);
	virtual tShaderOperator * clone() const = 0;

	virtual tColor evaluate(const tShaderData &data) = 0;
	virtual tColor transmit(const tShaderData &data) = 0;
	
	virtual void connect(const std::string &plugName, tShader *shader) {
		std::vector<std::string>::iterator plug;
		plug = std::find(plugNames.begin(), plugNames.end(), plugName);
		if (plug == plugNames.end()) {
			plugNames.push_back(plugName);
		}
		inputPlugs.connect(plugName, shader);
	};
protected:
	//hols all names for all the operands i the operator
	std::vector<std::string>	plugNames;
};

#pragma once
#include "tshaderoperator.h"
/*
	The shader operator *, for any plugs

	Add a plug / shader by calling connect with a 
	custom string (must be unique for the shader node).
*/
class tShaderOpMult :
	public tShaderOperator
{
public:
	tShaderOpMult(void);
	tShaderOpMult(const tShaderOpMult &c);
	virtual ~tShaderOpMult(void);

	tShaderOpMult & operator=(const tShaderOpMult &right);
	tShaderOpMult *clone() const;

	virtual tColor evaluate(const tShaderData &data);
	virtual tColor transmit(const tShaderData &data);
};

#pragma once
#include <tshader.h>
#include <tTexture2d.h>
/*
	Simple texture sampler shader

	This is of course a source shader
*/
class tShaderTextureImage2D :
	public tShader
{
public:
	tShaderTextureImage2D(void);
	tShaderTextureImage2D(const tImage &img);
	tShaderTextureImage2D(const tShaderTextureImage2D &s);
	tShaderTextureImage2D & operator= (const tShaderTextureImage2D &right);
	virtual tShaderTextureImage2D * clone() const;
	virtual ~tShaderTextureImage2D(void);

	/*
		Returns a color for a point.
	*/
	virtual tColor evaluate(const tShaderData &data);
	/*
		Returns a color value used for shadows.
	*/
	virtual tColor transmit(const tShaderData &data);

	void setImage(const tImage &img);
private:
	tTexture2d	texture;
};

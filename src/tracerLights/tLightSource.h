#pragma once

#include <vec3.h>
#include <tColor.h>

/*
	Abstract baseclass for all lightsources
*/
class tLightSource
{
public:
	static enum lightType {
		POINT_LIGHT=0,
		HEMISPHERE=1,
		AREALIGHT
	};
	tLightSource(void){};
	virtual ~tLightSource(void){};
	virtual tLightSource * clone() const = 0;
	/*
		Returns a attenuated color at a point in world space
	*/
	virtual tColor	intensity(const vec3 &worldPoint) = 0;
	/*
		Return world space position to a sample point for light.
		This method should account for averaging when sampling the
		light multiple times. 
	*/
	virtual void sampleLight(vec3 &lightSamplePoint_worldcoords, tColor &colorAtPoint) = 0;
	lightType type() { return lType; };
	unsigned int getNumberOfSamples() { return noSamples; };
protected:
	lightType lType;
	unsigned int	noSamples;
};

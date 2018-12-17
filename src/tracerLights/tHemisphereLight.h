#pragma once
#include "tlightsource.h"
/*
	A lightsource representing a whole sphere
	light environment.
*/
class tHemisphereLight :
	public tLightSource
{
public:
	//TODO: add texture samples ey len!
	tColor constantColor;
	tHemisphereLight(void);
	tHemisphereLight(const tHemisphereLight &l) ;
	virtual ~tHemisphereLight(void);
	virtual tHemisphereLight * clone() const;
	tHemisphereLight & operator= (const tHemisphereLight &l);
	/*
		Returns a attenuated color at a point in world space
	*/
	virtual tColor	intensity(const vec3 &worldPoint);
	/*
		Return world space position to a sample point for light
	*/
	void sampleLight(vec3 &lightSamplePoint_worldcoords, tColor &colorAtPoint);
	void setNumberOfSamples(unsigned int N);
private:
	
	unsigned int		 sampled;	//index last sampled
};

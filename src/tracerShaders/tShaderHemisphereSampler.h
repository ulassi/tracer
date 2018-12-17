#pragma once
#include "tshader.h"
#include "tRay.h"
#include <matrix3.h>
#include <vector>
#include <math.h>
/*
	Shader that samples the hemisphere in the normal direction.

		result = dot(N,S)*incidentRadiance(sampleDir)
*/
class tShaderHemisphereSampler :
	public tShader
{
public:
	//TODO: add tSampler2D member or tSampler3D, sample theta, phi from 2d sampler or xyz from 3D
	
	tShaderHemisphereSampler(void){
		this->setNumberOfSamples(300);
	};
	tShaderHemisphereSampler(const tShaderHemisphereSampler &s) { *this = s;};
	tShaderHemisphereSampler & operator= (const tShaderHemisphereSampler &s) {
		this->noSamples = s.noSamples;
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShaderHemisphereSampler(void) {};
	tShaderHemisphereSampler * clone() const { return new tShaderHemisphereSampler(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		//jitter using Normal direction
		double phi0 = acos(data.intersection.N[2]), phi;
		double theta0 = atan(fdiv(data.intersection.N[1], data.intersection.N[0])), theta;

		//jitter theta, phi
		//whole hemisphere
		double phiHalf = PI/2.0;	
		double thetaHalf = PI;		
		
		tColor result(0);
		unsigned int samplesPerSide = (unsigned int) sqrt((double)noSamples);
		double one_over_nsamples = 1.0/double(samplesPerSide);

		for (unsigned int s = 0; s< samplesPerSide*samplesPerSide; s++) {
			unsigned int y_offset = s/samplesPerSide, x_offset = s%samplesPerSide; 
			double x = (double(x_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			double y = (double(y_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			phi = phi0 + -phiHalf + 2*y*phiHalf;
			theta = theta0 + -thetaHalf + 2*x*thetaHalf;
			vec3 sampleDir(-sin(phi) * cos(theta),
				-sin(phi) * sin(theta),
				cos(phi));
			tRay newRay(data.intersection.P, sampleDir);
			double NdotS = max(0,(sampleDir, data.intersection.N));
			if (NdotS > 1)
				bool bajs = 0;
			result += NdotS*data.renObj.RayTrace(newRay, data.depth+1);
		}
		result *= one_over_nsamples;
		//double EdotR = max(0,(viewDirection, R));
        return result;
		
	};

	virtual tColor transmit(const tShaderData &data) {
		return tColor(0);
	};
	/*
		Sets number of samples to : sqrt(N)*sqrt(N)
		TODO: replace this with a sampler2D
	*/
	void setNumberOfSamples(unsigned int N) {
		noSamples = N;
	};
private:
	unsigned int			noSamples;
};

#pragma once
#include "tshader.h"
/*
	shader to sample glossy incident lighting
*/


class tShaderGlossSampler :
	public tShader
{
public:
	//PLUG LIST
	const static std::string plugGlossCol;
	const static std::string plugExponent;

	tShaderGlossSampler(void) {
		this->inputPlugs.addPlug(plugGlossCol);
		this->inputPlugs.addPlug(plugExponent);
	};
	tShaderGlossSampler(const tShaderGlossSampler &s) { *this = s;};
	tShaderGlossSampler & operator= (const tShaderGlossSampler &s) {
		this->noSamples = s.noSamples;
		this->inputPlugs = s.inputPlugs;
		return *this;
	};
	virtual ~tShaderGlossSampler(void) {};
	tShaderGlossSampler * clone() const { return new tShaderGlossSampler(*this); };
	virtual tColor evaluate(const tShaderData &data)
	{
		vec3 R;
		data.renObj.reflect(data.intersection.N, data.viewVec,R);
						
		//jitter R
		//convert R to theta phi:
		double phi0 = acos(R[2]), phi;
		double theta0 = atan(fdiv(R[1], R[0])), theta;

		//jitter theta, phi
		double deltaHalf = PI/((double)inputPlugs.evaluatePlug(tShaderGlossSampler::plugExponent, data));
		//if (sampled >= noSamples -1) 
		//	sampled = 0;
		tColor result(0);
		unsigned int samplesPerSide = (unsigned int) sqrt((double)noSamples);
		double one_over_nsamples = 1.0/double(samplesPerSide);

		for (unsigned int s = 0; s< samplesPerSide*samplesPerSide; s++) {
			unsigned int y_offset = s/samplesPerSide, x_offset = s%samplesPerSide; 
			double x = (double(x_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			double y = (double(y_offset) + (rand()/(double)RAND_MAX))*one_over_nsamples;
			phi = phi0 + -deltaHalf + 2*y*deltaHalf;
			theta = theta0 + -deltaHalf + 2*x*deltaHalf;
			vec3 sampleDir(-sin(phi) * cos(theta),
				-sin(phi) * sin(theta),
				cos(phi));
			tRay newRay(data.intersection.P, sampleDir);
			double EdotR = pow((double)inputPlugs.evaluatePlug(plugExponent, data),max(0,(sampleDir, data.viewVec*-1)));
			if (EdotR > 1)
				bool bajs = 0;
			result += EdotR*data.renObj.RayTrace(newRay, data.depth+1);
		}
		result *= one_over_nsamples;
		//double EdotR = max(0,(viewDirection, R));
		return result*inputPlugs.evaluatePlug(plugGlossCol, data);// pow(EdotR,(double)n) ;
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
	unsigned int noSamples;
};


const std::string tShaderGlossSampler::plugGlossCol = std::string("specCol");
const std::string tShaderGlossSampler::plugExponent = std::string("exp");

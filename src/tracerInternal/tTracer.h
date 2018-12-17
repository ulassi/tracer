#pragma once
/*
	The Raytracer that does all the work

	Takes a scene and produces an image  / images

*/
#include <fstream>
#include <vector>
//#include "tScene.h"
class tScene;
#include "tColor.h"
#include "tRay.h"
#include <tImage.h>
extern std::ofstream gLog;

struct tFrame {
	/*
		All pixels Stored from top left to bottom right.
		Top row from left to right first, then second row
		from left to right and so on.
	*/
	tImage	image;
};

class tTracer
{
public:
	double gRaysShot;			//for statistics
	static enum renderMode {
		TRACER_SHADOWS=2,
	};

	tTracer(void);
	tTracer & operator =(const tTracer &right);
	~tTracer(void);
	/*
		Renders rScene into rTarget in resolution 
		found in rTarget.
	*/
	int render(tFrame &rTarget, tScene &rScene, unsigned int cameraIndex, unsigned int samplesPerPixel);

	/*
		Sets the render mode
		Use tTracer::TRACER_XXX | tTracer::TRACER_XXX | ... 
		to set a combination of modes
	*/
	void setRenderMode(int mode);
	void setMaxRecursiveDepth(unsigned int depth);

	//Utility members

	/*
		Reflects I in N, result is put in reflection
	*/
	void reflect(const vec3 &N, const vec3 &I, vec3 &reflection) const;
	/*
		Refracts I in N based on ior, result is put in refraction
	*/
	void refract(const vec3 &N, const vec3 &I, vec3 &refraction, double ior) const;
	/*
		Does the actual raytracing

		Returns a color corresponding to scene's respons
		along ray.
	*/
	tColor	tTracer::RayTrace(const tRay &ray, unsigned int depth) const;
private:
	tColor			mBackgroundCol;
	int				mRenMode;
	unsigned int	mMaxDepth;
	tScene			*currentScene;
};

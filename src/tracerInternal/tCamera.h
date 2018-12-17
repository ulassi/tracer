#pragma once
/*
	tCamera class:
		Represents a camera in a tScene

		Features:
			- uses jittered stratification, sampleGridSize is the sqrt of number of 
				samples on the grid
			
*/
#include <vec3.h>
#include "tRay.h"

class tCamera
{
public:
	tCamera(void);
	tCamera(const tCamera &c);
	tCamera & operator= (const tCamera &right);
	~tCamera(void);	
	/*
		Creates a new ray to sample pixel at position _x and _y
	*/
	tRay getRay(unsigned int _x, unsigned int _y);
	/*
	*/
	void setEyePoint(const vec3 &eye);
	/*
		Supply orientation in Degrees.

		Heading:	clockwise rotation around Y, 0 = -Z axis
		Pitch:		rotation down towards -Y axis, 0 = ZX plane
		Bank:		not implemented yet
	*/
	//TODO: implement tCamera Bank
	void setOrientation(double heading, double pitch, double bank);
	void setAspectRatio(double width_over_height_ratio);
    void setDistanceToImagePlane(double d);
	void setFieldOfView(double angle);
	void setSampleGridSize(unsigned int number_of_points);
	void setImageWidth(unsigned int number_of_pixels);
	void setImageHeight(unsigned int number_of_pixels);

	double getAspectRatio();
	double getDistanceToImagePlane();
	double getFieldOfView();
	unsigned int getSampleGridSize();
	unsigned int getImageWidth();
	unsigned int getImageHeight();
private:
	//matrix4		worldToViewTransform;	//transform from world to view
	vec3 eyePoint,		//center of projection and origin of eye space
		viewDir,		//view Direction from COP, view plane normal
		upVec,			//up vector
		rightVec;		//right vec
	//matrix4		viewToScreenTransform;	
	unsigned int samplesInThisPixel;
	double aspectRatio, //width / height
		focalLength,	//distance to image plane
		fov;			//field of view, in degrees
	unsigned int pixelWidth, pixelHeight, sampleGridSize;
	
};

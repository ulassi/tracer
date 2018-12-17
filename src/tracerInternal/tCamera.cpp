#include "stdafx.h"
#include ".\tcamera.h"
#include <stdlib.h>

tCamera::tCamera(void) : viewDir(vec3(0,0,-1)), focalLength(1.0), aspectRatio(4.0/3.0),
pixelHeight(480), pixelWidth(640), fov(90), upVec(0,1,0), sampleGridSize(1), samplesInThisPixel(0)
{
}
tCamera::tCamera(const tCamera &c) {
	*this = c;
}
tCamera & tCamera::operator= (const tCamera &right) {
	this->aspectRatio = right.aspectRatio;
	this->eyePoint = right.eyePoint;
	this->focalLength = right.focalLength;
	this->fov = right.fov;
	this->pixelHeight = right.pixelHeight;
	this->pixelWidth = right.pixelWidth;
	this->rightVec = right.rightVec;
	this->upVec = right.upVec;
	this->viewDir = right.viewDir;
	this->samplesInThisPixel = right.samplesInThisPixel;
	this->sampleGridSize = right.sampleGridSize;
	return *this;
}
tCamera::~tCamera(void)
{
}


void tCamera::setEyePoint(const vec3 &eye) {
	this->eyePoint = eye;
}


void tCamera::setOrientation(double heading, double pitch, double bank){
	//set view dir according to heading and pitch,
	double theta, phi;
	theta = PI*(heading-90)/180.0;
	phi = PI*(pitch+90)/180.0;

	this->viewDir.set(
		cos(theta)*sin(phi),	//x
		cos(phi),				//y
		sin(theta)*sin(phi));	//z
	phi -= PI/4.0;

	//update upVec
	this->upVec.set(
		cos(theta)*sin(phi),	//x
		cos(phi),				//y
		sin(theta)*sin(phi));	//z
    rightVec = viewDir*upVec;
	upVec = rightVec*viewDir;
	rightVec.normalize(); upVec.normalize();
}
tRay tCamera::getRay(unsigned int _x, unsigned int _y) {
	
	//TODO: probably an error in this part
	tRay ray;

    ray.start = this->eyePoint;	
	
	//find direction
	ray.dir = this->viewDir*this->focalLength;	//step to mid of image plane

	//find viewport size in world
    double xSize, ySize;
	
	xSize = 2*focalLength*tan(PI*fov/(180.0*2.0));
	ySize = xSize/aspectRatio;

	double pixelSizeX = xSize / (double) this->pixelWidth;
	double pixelSizeY = ySize / (double) this->pixelHeight;

	//step to upper left corner
	vec3 left = rightVec*-1;//upVec* viewDir;
	ray.dir += left*(xSize/2.0);	//step to left side, mid of image plane
	ray.dir += upVec *(ySize/2.0);  //step to top left corner of image plane

	//step to right value _x
    ray.dir -= (left * //flip to right vector
		_x*pixelSizeX);		   //distance to leftside of pixel

	//step down to right value _y
	ray.dir -= ( upVec * _y * pixelSizeY);

	//find ray in pixel space
	double psW, psH;
	unsigned int stepX, stepY;
	//jittered stratification in pixel
	psW = pixelSizeX/((double) this->sampleGridSize); //width and
	psH = pixelSizeY/((double) this->sampleGridSize); //height of each cell in pixel

	stepX = this->samplesInThisPixel % sampleGridSize;
	stepY = this->samplesInThisPixel / sampleGridSize;

	ray.dir -= (upVec * (psH * ((double)stepY + rand()/(double) RAND_MAX )) );
	ray.dir -= (left * (psW * ((double)stepX + rand()/(double) RAND_MAX )) );
	samplesInThisPixel++;
	
	if (samplesInThisPixel >= sampleGridSize*sampleGridSize) {
		samplesInThisPixel = 0;
	}
	
	/*	just random value in pixel for now*/
	//ray.dir -= (upVec * pixelSizeY * (rand()/(double) RAND_MAX));
	//ray.dir -= (left * pixelSizeX * (rand()/(double) RAND_MAX));
	
	//set startpoint to point chosen
	//ray.start += ray.dir;
	ray.dir.normalize();
	return ray;
}
void tCamera::setAspectRatio(double width_over_height_ratio){this->aspectRatio = width_over_height_ratio;}
void tCamera::setDistanceToImagePlane(double d) { this->focalLength = d;}
void tCamera::setFieldOfView(double angle){ this->fov = angle; }
void tCamera::setSampleGridSize(unsigned int number_of_points) { this->sampleGridSize = number_of_points; }
void tCamera::setImageWidth(unsigned int number_of_pixels) { this->pixelWidth = number_of_pixels; }
void tCamera::setImageHeight(unsigned int number_of_pixels){ this->pixelHeight = number_of_pixels; }

double tCamera::getAspectRatio() {
	return this->aspectRatio;
}
double tCamera::getDistanceToImagePlane() {
	return this->focalLength;
}
double tCamera::getFieldOfView() {
	return this->fov;
}
unsigned int tCamera::getSampleGridSize() {
	return this->sampleGridSize;
}
unsigned int tCamera::getImageWidth() {
	return this->pixelWidth;
}
unsigned int tCamera::getImageHeight() {
	return this->pixelHeight;
}
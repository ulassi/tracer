#pragma once
#include <tColor.h>
#include <vector>
/*
	2D image class
*/
class tImage
{
public:
	tImage(void) {};
	tImage(unsigned int _width, unsigned int _height, unsigned int _channels) {
		width = _width;
		height = _height; 
		channels = _channels;

		//init pixels
		pixels.resize(width*height,tColor(0));
	};
	tImage(const tImage &i) { *this = i; };
	~tImage(void) {};

	tImage & operator= (const tImage &i) {
		this->width = i.width;
		this->height = i.height;
		this->pixels = i.pixels;
		this->channels = i.channels;
		return *this;
	};

	const tColor & samplePixel(unsigned int x, unsigned int y) {
		return pixels.at(y*width + x);
	};
	const tColor & samplePixel(unsigned int i) {
		return pixels.at(i);
	};
	void setPixel(unsigned int x, unsigned int y, const tColor &value) {
		pixels.at(y*width +x) = value;
		//else ERROR
	}
   
	//Get members
	unsigned int getWidth() { return width; };
	unsigned int getHeight() { return height; };
	unsigned int getChannels() { return channels; };
	unsigned int getNumberOfPixels() { return (width * height); };

private:
	unsigned int width, height, channels;
	std::vector<tColor> pixels;
};

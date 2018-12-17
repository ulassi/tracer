#pragma once
#include <tImage.h>
/*
	Wrapper for any 2D tImage data that is to be sampled as a 
	2D texture.
*/
class tTexture2d
{
public:
	tTexture2d(void);
	tTexture2d(const tImage &img);
	tTexture2d(const tTexture2d &tex); 
	~tTexture2d(void);
	tTexture2d & operator= (const tTexture2d &right);

	/*
		Samples this texture.

		Wraps if repeat* is true, else resets to border texel.
	*/
	const tColor & sample(const double &s, const double &t);

	//Data manipulation
	void setImage(const tImage &img);
	const tImage & getImage();
private:
	tImage image;
};

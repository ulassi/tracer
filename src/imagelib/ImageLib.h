// ImageLib.h: interface for the ImageLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGELIB_H__273AC697_FA78_44F8_BD57_BE35AE8B5954__INCLUDED_)
#define AFX_IMAGELIB_H__273AC697_FA78_44F8_BD57_BE35AE8B5954__INCLUDED_



#if _MSC_VER > 1000
#pragma once
//#include "CommonTypes.h"
#include <stdio.h>
#endif // _MSC_VER > 1000
//#include "extgl.h"
//#include <GL/gl.h>
struct ILimage {
	int width, height;
	int channels;
	int type;	//0 = float, 1 = integer(8 bits)
	unsigned char *data;
};
struct TGAHeader
{
	unsigned char Header[12];									// TGA File Header
};

//##ModelId=3DFB64C6012B
struct TGA
{
	unsigned char		header[6];								// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;									//Height of Image
	unsigned int		Width;									//Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
};
class ImageLib  
{
public:
	unsigned char * getData();
	int getChannels();
	int getHeight();
	int getWidth();
	void flushLastImage();
	
	int loadImage(char * filename);
	int saveImage(char *filename, unsigned char *data, uint w, uint h, uint channels);

	
	ImageLib();
	virtual ~ImageLib();
private:
	bool LoadUncompressedBWTGA(char *filename, FILE *fTGA);
	bool LoadUncompressedTGA(char * filename, FILE * fTGA);
	int loadTGAFile(char *filename);
	ILimage lastLoadedImage;
	int loadFloatFile(char *filename);
	int saveFloatFile(char *filename, unsigned char *data, uint w, uint h, uint channels);

	int loadFloatHED(char *filename);
	//TGA specific
	TGAHeader tgaheader;									// TGA header
	TGA tga;												// TGA image data
	unsigned char uTGAcompare[12];// = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header, RGB file
	unsigned char cTGAcompare[12];// = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header, RGB file
	unsigned char bwTGAcompare[12]; // = {0,0,3,0,0,0,0,0,0,0,0,0};	// Black and white TGC file
};

#endif // !defined(AFX_IMAGELIB_H__273AC697_FA78_44F8_BD57_BE35AE8B5954__INCLUDED_)

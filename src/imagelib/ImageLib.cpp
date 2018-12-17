// ImageLib.cpp: implementation of the ImageLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <ImageLib.h>
#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageLib::ImageLib()
{
	this->lastLoadedImage.channels = 0;
	this->lastLoadedImage.data = 0;
	this->lastLoadedImage.height = 0;
	this->lastLoadedImage.width = 0;
	this->lastLoadedImage.type  = -1;
	uTGAcompare[0] = uTGAcompare[1] = 0;
	uTGAcompare[2] = 2;
	uTGAcompare[3] = uTGAcompare[4] = uTGAcompare[5] = 
	uTGAcompare[6] = uTGAcompare[7] = uTGAcompare[8] = 
	uTGAcompare[9] = uTGAcompare[10] = uTGAcompare[11] = 0;//{0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	memcpy(&cTGAcompare, &uTGAcompare, 12);
	cTGAcompare[2] = 10;
	memcpy(&bwTGAcompare, &uTGAcompare, 12);
	bwTGAcompare[2] = 3;
}

ImageLib::~ImageLib()
{
	flushLastImage();
}
/*
	Loads an image from file. Data is stored in lastLoadedImage
	returns :	1 if succesful
				0 if failed
*/
int ImageLib::loadImage(char *filename)
{
	//find '.' char in string
	char *suffix = strchr(filename, '.');
	suffix++;
	/*suffix[0] = tolower(suffix[0]);
	suffix[1] = tolower(suffix[1]);
	suffix[2] = tolower(suffix[2]);*/
	if (suffix)
	{

		if ( (!strcmp(suffix, "float")) || (!strcmp(suffix, "raw")))
		{	//load HDR float image
			flushLastImage();
			if (loadFloatFile(filename))
				return 1;
			else 
				return 0;
		}
		else if ( (!strcmp(suffix, "tga")) )
		{	//load TGA image
			flushLastImage();
			if (loadTGAFile(filename) )
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	return 0;
}

/*Save an image to file
*/
int ImageLib::saveImage(char *filename, unsigned char *data, uint w, uint h, uint channels)
{
	//find '.' char in string
	char *suffix = strchr(filename, '.');
	suffix++;
	if (suffix)
	{

		if ( (!strcmp(suffix, "float")) || (!strcmp(suffix, "raw")))
		{	//load HDR float image
			flushLastImage();
			if (saveFloatFile(filename, data, w, h, channels))
				return 1;
			else 
				return 0;
		}
		/*
		else if ( (!strcmp(suffix, "tga")) )
		{	//load TGA image
			flushLastImage();
			if (loadTGAFile(filename) )
				return 1;
			else
				return 0;
		}
		else
			return 0;
		*/
	}
	return 0;
}

/*
	Load a RAW file of float data, suffix .raw or .float
		the loader needs to have an .hed file with same name as 
		image file to determine width and height
 */
int ImageLib::loadFloatFile(char *filename)
{
	std::ifstream file(filename, std::ios_base::binary);
	
		
	char *headFile = new char[strlen(filename)+1];
	strncpy(headFile, filename, strlen(filename)+1);
	char *tmp = strchr(headFile, '.');
	tmp[1] = 'h';
	tmp[2] = 'e';
	tmp[3] = 'd';
	tmp[4] = '\0';

	//load hed file into image info struct
	if (!loadFloatHED(headFile))
	{
		delete[] headFile;
		return 0;
	}
	delete[] headFile;
	
	if (file.is_open())
	{
		//read float info
		// get length of file:
		file.seekg (0, std::ios::end);
		int length = file.tellg();
		file.seekg (0, std::ios::beg);
		if (length != lastLoadedImage.channels*sizeof(float)*lastLoadedImage.height*lastLoadedImage.width)
		{
			file.close();
			return 0;
		}
		
		this->lastLoadedImage.data = (unsigned char*) malloc(length);
		file.read((char*)lastLoadedImage.data, length);
		file.close();

		lastLoadedImage.type = 0; //float type

		return 1;
	}
	else
		return 0;
}


int ImageLib::saveFloatFile(char *filename, unsigned char *data, uint w, uint h, uint channels){
	
	std::ofstream file(filename, std::ios_base::binary);
		
	if (file.is_open())
	{
		
		file.write((char*)data, w*h*channels*sizeof(float));
		file.close();

		return 1;
	}
	else
		return 0;

}


//Frees up memory from last image loading
void ImageLib::flushLastImage()
{
	this->lastLoadedImage.channels = 0;
	if (this->lastLoadedImage.data)
		free(this->lastLoadedImage.data);
	this->lastLoadedImage.data = 0;
	this->lastLoadedImage.height = 0;
	this->lastLoadedImage.width = 0;
}
//Returns width in pixels
int ImageLib::getWidth()
{
	return lastLoadedImage.width;
}
//Returns height in pixels
int ImageLib::getHeight()
{
	return lastLoadedImage.height;
}
//Returns number of channels in data
int ImageLib::getChannels()
{
	return lastLoadedImage.channels;
}
//Returns raw data bytes loaded, the actual type is specifiec by lastLoadedImage.type
unsigned char * ImageLib::getData()
{
	return lastLoadedImage.data;
}
//Loads an .hed associated with a .float or .raw file
int ImageLib::loadFloatHED(char *filename)
{
	std::ifstream head;
	head.open(filename);
	if (head.is_open())
	{
		//read head info
		head.seekg(0, std::ios::end);
		int length = head.tellg();
		head.seekg(0, std::ios::beg);
		
		char *buffer = new char[length];
		head.read(buffer, length);
		head.close();
		
		char *tag;
		tag = strstr( buffer, "width");
		if (tag)
			sscanf(tag+5,"%d", &this->lastLoadedImage.width);
		else
		{
			delete[] buffer;
			return 0;
		}

		tag = strstr(buffer,"height");
		if (tag)
			sscanf(tag+6,"%d", &this->lastLoadedImage.height);
		else
		{
			delete[] buffer;
			return 0;
		}

		tag = strstr(buffer, "channels");
		if (tag)
			sscanf(tag+8,"%d", &this->lastLoadedImage.channels);
		else
		{
			delete[] buffer;
			return 0;
		}
		delete[] buffer;
		return 1;
	}
	else 
		return 0;
}

int ImageLib::loadTGAFile(char *filename)
{
	FILE * fTGA;												// File pointer to texture file
	fTGA = fopen(filename, "rb");								// Open file for reading
	this->lastLoadedImage.type = 1;	//8 bit integers
	if(fTGA == NULL)											// If it didn't open....
	{
		MessageBox(NULL, "Could not open texture file", "ERROR", MB_OK);	// Display an error message
		return false;														// Exit function
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
	{
		MessageBox(NULL, "Could not read file header", "ERROR", MB_OK);		// If it fails, display an error message 
		if(fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(filename, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		//LoadCompressedTGA(filename, fTGA);
		fclose(fTGA);
		return false;
	}
	else if(memcmp(bwTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// BW image
		LoadUncompressedBWTGA(filename, fTGA);
	}
	else																	// If header matches neither type
	{
		MessageBox(NULL, "TGA file be type 2, 3 or type 10 ", "Invalid Image", MB_OK);	// Display an error
		fclose(fTGA);
		return false;																// Exit function
	}
	return true;	
}

bool ImageLib::LoadUncompressedTGA(char * filename, FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
{																			// TGA Loading code nehe.gamedev.net)
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{										
		MessageBox(NULL, "Could not read info header", "ERROR", MB_OK);		// Display error
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	this->lastLoadedImage.width = tga.header[1] * 256 + tga.header[0];		// Determine The TGA Width	(highbyte*256+lowbyte)
	this->lastLoadedImage.height = tga.header[3] * 256 + tga.header[2];		// Determine The TGA Height	(highbyte*256+lowbyte)
	tga.Width		= this->lastLoadedImage.width;							// Copy width into local structure						
	tga.Height		= this->lastLoadedImage.height;							// Copy height into local structure
	tga.Bpp			= tga.header[4];										// Copy BPP into local structure

	if((this->lastLoadedImage.width <= 0) || (this->lastLoadedImage.height <= 0) || ((tga.Bpp != 24) && (tga.Bpp !=32)))	// Make sure all information is valid
	{
		MessageBox(NULL, "Invalid texture information", "ERROR", MB_OK);	// Display Error
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}

		
	this->lastLoadedImage.channels = tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	this->lastLoadedImage.data	= (unsigned char *) malloc(tga.imageSize);					// Allocate that much memory

	if(this->lastLoadedImage.data == NULL)											// If no space was allocated
	{
		MessageBox(NULL, "Could not allocate memory for image", "ERROR", MB_OK);	// Display Error
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(this->lastLoadedImage.data, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		MessageBox(NULL, "Could not read image data", "ERROR", MB_OK);		// Display Error
		if(this->lastLoadedImage.data != NULL)										// If imagedata has data in it
		{
			flushLastImage();										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(unsigned int cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		this->lastLoadedImage.data[cswap] ^= this->lastLoadedImage.data[cswap+2] ^=
		this->lastLoadedImage.data[cswap] ^= this->lastLoadedImage.data[cswap+2];
	}

	fclose(fTGA);															// Close file
	return true;															// Return success
}
bool ImageLib::LoadUncompressedBWTGA(char *filename, FILE *fTGA)
{
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{										
		MessageBox(NULL, "Could not read info header","ImageLib ERROR",  MB_OK);		// Display error
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	this->lastLoadedImage.width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	this->lastLoadedImage.height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	tga.Width		= this->lastLoadedImage.width;										// Copy width into local structure						
	tga.Height		= this->lastLoadedImage.height;										// Copy height into local structure
	tga.Bpp			= tga.header[4];										// Copy BPP into local structure
	if((this->lastLoadedImage.width <= 0) || (this->lastLoadedImage.height <= 0) || (tga.Bpp != 8))	// Make sure all information is valid
	{
		MessageBox(NULL, "Invalid texture information", "ImageLib ERROR", MB_OK);	// Display Error
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}
	/*
	if(tga.Bpp == 24)													//If the BPP of the image is 24...
	{
		texture->type	= GL_RGB;											// Set Image type to GL_RGB
	}
	else																	// Else if its 32 BPP
	{
		texture->type	= GL_RGBA;											// Set image type to GL_RGBA
	}
	*/
	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	this->lastLoadedImage.data	= (unsigned char *)malloc(tga.imageSize);					// Allocate that much memory

	if(this->lastLoadedImage.data == NULL)											// If no space was allocated
	{
		MessageBox(NULL, "Couldnt allocate memory for image", "ImageLib ERROR", MB_OK);	// Display Error
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(this->lastLoadedImage.data, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
		MessageBox(NULL, "Could not read image data", "ImageLib ERROR", MB_OK);		// Display Error
		if(this->lastLoadedImage.data != NULL)										// If imagedata has data in it
		{
			flushLastImage();										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	/*for(unsigned int cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		this->lastLoadedImage.data[cswap] ^= this->lastLoadedImage.data[cswap+2] ^=
		this->lastLoadedImage.data[cswap] ^= this->lastLoadedImage.data[cswap+2];
	}*/

	fclose(fTGA);															// Close file
	return true;
}

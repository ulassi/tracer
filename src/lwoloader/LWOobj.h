#pragma once
#include <vector>
#include <string>
/*
	Single VMAP object
*/
class LWOobj
{
public:
	LWOobj(void);
	~LWOobj(void);
	static struct ImageClip {
		std::string		stillImage;	//only one
		unsigned int	index;		
	};
	static struct VertexMapElement{
		unsigned int		vertexNo;
		std::vector<float>	values;
	};
	static struct DiscontVertexMapElement {
		unsigned int		vertexNo;
		unsigned int		polygonNo;
		std::vector<float>	values;
	};
	static struct VertexMap{
		std::vector<VertexMapElement>	elements;
		std::string						name;
		unsigned int					dimensions;
	};
	static struct DVertexMap {
		std::vector<DiscontVertexMapElement>	elements;
		std::string						name;
		unsigned int					dimensions;
	};
	static struct Texture {
		std::string			type; //IMAP, PROC, GRAD, SHDR
		std::string			channel; //COLR, DIFF, LUMI, SPEC, GLOS, REFL, TRAN, RIND, TRNL, or BUMP, 
		std::string			vmap; //name of vmap
		std::string			projectionType;	//UV, PLANAR, CYL, SPHERE, FRONT, CUBE
		std::vector<float>	size;		//x,y,z
		std::vector<float>	rotation;	//h,p,b
		std::vector<float>	center;		//x,y,z
		unsigned int		imageIndex;
		std::string			wrap[2];	//[wrap width, wrap height] = [RESET, REPEAT, MIRROR, EDGE, ...]
		float				wrapAmount[2];	//for CYL and SPHERE only, [width, height]
		float				amplitude;
		float				gradientStart;	//only GRAD
		float				gradientEnd;	//only GRAD
		std::string			procAlgoName;	//only for PROC
	};
	static struct Surface {
		std::string  name;
		/* Intensities */
		float diffuse;
		float luminance;
		float specular;
		float reflectivity;
		float gloss;
		float transparency;
		float translucency;
		float diffuseSharpness;
		float bumpiness;
		float reflectionMapImageSeamAngle; //radians
		float indexOfRefraction;
		float specularColourHighlight;
		float colourFilter;
		float additiveTransparency;
		
		/* Attributes */
		unsigned int	sidedness;	//1=front, 3 = doublesided
		float			maxSmoothingAngle;
		/*  Alpha mode
		0 - Unaffected by Surface
			The surface has no effect on the alpha channel when rendered.
		1 - Constant Value
			The alpha channel will be written with the constant value following the mode in the subchunk.
		2 - Surface Opacity
			The alpha value is derived from surface opacity, which is the default if the ALPH chunk is missing.
		3 - Shadow Density
		*/
		unsigned int	alphaMode;	
		float			alphaValue;

		std::vector<Texture>		vecTextures;
		/* Colour */
		float		baseColour[3];
	};
	std::vector<float>		mBbox;			//min, max (6 floats)
	std::vector<float>	mVertices;		// Vertices, x,y,z
	/*
		When writing POLS, the vertex list for each polygon should 
		begin at a convex vertex and proceed clockwise as seen from 
		the visible side of the polygon. LightWave® polygons are single-
		sided (although double-sidedness is a possible surface property), 
		and the normal is defined as the cross product of the first and last edges.
	*/
	std::vector<unsigned int> mPolygons;	// Polygons N, v0, ..., vN, flags
	std::vector<unsigned int> mPolySurface;	// One surface index for each polygon: PolyIndex, SurfaceTag
	std::vector<LWOobj::VertexMap>			mVertexMaps;
	std::vector<LWOobj::DVertexMap>			mDVertexMaps;
	std::vector<LWOobj::Surface>			mSurfaces;
	std::vector<LWOobj::ImageClip>			mImages;
	std::vector<std::string>				mTags;	//names associated with a polygon
};

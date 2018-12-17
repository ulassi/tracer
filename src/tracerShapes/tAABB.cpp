// tAABB.cpp: implementation of the tAABB class.
//
//////////////////////////////////////////////////////////////////////

#include "tAABB.h"
//#include "tMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3DFB64CE023B
tAABB::tAABB(const tAABB &box) {
	*this = box;
}
tAABB & tAABB::operator= (const tAABB &r) {
	cen = r.cen;
	x_width = r.x_width;
	y_height = r.y_height;
	z_depth = r.z_depth;
	return *this;
}
tAABB::tAABB(const vec3 &min, const vec3 &max)
{
	
	cen = min + ( (max-min)/2);
	x_width = max[0] - min[0];
	y_height = max[1] - min[1];
	z_depth = max[2] - min[2];

	//make AABB cube
	if ( (x_width > y_height) & (x_width > z_depth))
		y_height = z_depth = x_width;
	else if ( (y_height > x_width) & (y_height > z_depth))
		x_width = z_depth = y_height;
	else if ( (z_depth > x_width) & (z_depth > y_height ))
		x_width =  y_height = z_depth;
}
//##ModelId=3DFB64CE0231
tAABB::tAABB(void)
{
	//default: box at x,y,z: -1.0 to 1.0 (centre: 0,0,0)
	x_width = 1;
	y_height = 1;
	z_depth = 1;
}

//##ModelId=3DFB64CE024F
tAABB::~tAABB()
{

}
tAABB* tAABB::clone() const {
	return new tAABB(*this);
}

//Public member methods

//Checks if this AABB is intersecting the supplied AABB
// Return: True if it is, else False
//##ModelId=3DFB64CE02A0
/*bool tAABB::AABBintersects(const tAABB &b)
{
	//vec3 delta;
	//delta = *(b->cen) - *(this->cen);
	if ( ((fabs(b.cen[0] - cen[0]) - x_width/2 - (b.x_width)/2 ) <= 0) &&
		 ((fabs(b.cen[1] - cen[1]) - y_height/2 - (b.y_height)/2 ) <= 0) &&
		 ((fabs(b.cen[2] - cen[2]) - z_depth/2 - (b.z_depth)/2 ) <= 0) )
		  return true;
	return false;
}*/
//returns the centre vector
//##ModelId=3DFB64CE0259
const vec3 & tAABB::centre()
{
	return cen;
}
//check a point vs AABB's corner vectors
// AABB MUST BE CUBE, not a block of any kind
//##ModelId=3DFB64CE02E6
long tAABB::cornerPoint(const vec3 &p)
{
	long ret = 0;
	
	vec3 point;
	point = p - cen;
	float sqrt_of_3 = 1.7320508f;
	if ( (point[0] + point[1] + point[2] ) > sqrt_of_3) ret |= 0x01;
	if ( (point[0] + point[1] - point[2] ) > (x_width/2)*3) ret |= 0x02;
	if ( (point[0] - point[1] + point[2] ) > (x_width/2)*3) ret |= 0x04;
	if ( (point[0] - point[1] - point[2] ) > (x_width/2)*3) ret |= 0x08;
	if ( (-point[0] + point[1] + point[2] ) > (x_width/2)*3) ret |= 0x10;
	if ( (-point[0] + point[1] - point[2] ) > (x_width/2)*3) ret |= 0x20;
	if ( (-point[0] - point[1] + point[2] ) > (x_width/2)*3) ret |= 0x40;
	if ( (-point[0] - point[1] - point[2] ) > (x_width/2)*3) ret |= 0x80;
	return ret;
}
//check a point vs AABB's edge vectors
// AABB MUST BE CUBE, not a block of any kind
//##ModelId=3DFB64CE02D2
long tAABB::edgePoint(const vec3 &p)
{
	long ret = 0;
	vec3 point;
	point = p - cen;
	if ( point[0] + point[1] > x_width) ret |= 0x001;
	if ( point[0] - point[1] > x_width) ret |= 0x002;
	if (-point[0] + point[1] > x_width) ret |= 0x004;
	if (-point[0] - point[1] > x_width) ret |= 0x008;
	if ( point[0] + point[2] > x_width) ret |= 0x010;
	if ( point[0] - point[2] > x_width) ret |= 0x020;
	if (-point[0] + point[2] > x_width) ret |= 0x040;
	if (-point[0] - point[2] > x_width) ret |= 0x080;
	if ( point[1] + point[2] > x_width) ret |= 0x100;
	if ( point[1] - point[2] > x_width) ret |= 0x200;
	if (-point[1] + point[2] > x_width) ret |= 0x400;
	if (-point[1] - point[2] > x_width) ret |= 0x800;
	return ret;   
}
//checks if point is inside AABB's six faceplanes
// returns a long integer as follows
// bit.no = 1		0
//	0		in		out		pos x
//	1		in		out		neg x
//	2		in		out		pos y
//	3		in		out		neg y
//	4		in		out		pos z
//	5		in		out		neg z
//##ModelId=3DFB64CE02C8
long tAABB::facePoint(const vec3 &p)
{
	long ret;
	ret = 0;
	vec3 point = p - cen;
	if (point[0] >  x_width/2) ret |= 0x01;
	if (point[0] < -x_width/2) ret |= 0x02;
	if (point[1] >  y_height/2) ret |= 0x04;
	if (point[1] < -y_height/2) ret |= 0x08;
	if (point[2] >  z_depth/2) ret |= 0x10;
	if (point[2] < -z_depth/2) ret |= 0x20;
	return ret;
   
}
//returns 0 : if AABB outside frustum
//returns 1 : if intersects frustum
//returns 2 : if totally inside frusum
//##ModelId=3DFB64CE02B3
/*int tAABB::frustumIntersects(aPlane* frustum)
{
	 
	//SHOULD be faster, but somethings wrong!

	//First trivially accept or reject the AABB (accept: inside, reject: outside) 
	// vs all six frustum planes
	int plane_no, inside_planes_no;
	
	//used to store the nearest point in AABB, relative to 
	// a frustum planes normal. (farthest = far_point * -1)
	vec3 near_point;
	bool direction[3];

	inside_planes_no = 0;
	//iterate six frustum planes
	for(plane_no = 0; plane_no<6; plane_no++)
	{
		if (frustum[plane_no].n[0] > 0)	//POS n[0]
		{
			near_point[0] = cen[0] + x_width/2;
			direction[0] = true;
		}
		else	//NEG n[0]
		{
			near_point[0] = cen[0] - x_width/2; 
			direction[0] = false;
		}
		if (frustum[plane_no].n[1] > 0)	//POS n[1]
		{
			near_point[1] = cen[1] + y_height/2; 
			direction[1] = true;
		}
		else	//NEG n[1]
		{
			near_point[1] = cen[1] - y_height/2;
			direction[1] = false;
		}
		if (frustum[plane_no].n[2] > 0)	//POS n[2]
		{
			near_point[2] = cen[2] + z_depth/2; 
			direction[2] = true;
		}
		else	//NEG n[2]
		{
			near_point[2] = cen[2] - z_depth/2; 
			direction[1] = false;
		}
		if (frustum[plane_no].n[0] * near_point[0] +
			frustum[plane_no].n[1] * near_point[1] +
			frustum[plane_no].n[2] * near_point[2] +
			frustum[plane_no].d < 0)
			return 0;	//Near outside plane
		else
		{	//near inside plane
			if (direction[0])
				near_point[0] = cen[0] - x_width/2;
			else
				near_point[0] = cen[0] + x_width/2;
			if (direction[1])
				near_point[1] = cen[1] - y_height/2;
			else
				near_point[1] = cen[1] + y_height/2;
			if (direction[2])
				near_point[2] = cen[2] - z_depth/2;
			else
				near_point[2] = cen[2] + z_depth/2;

			//is far point inside plane
			if ( (frustum[plane_no].n[0] * near_point[0]) +
				frustum[plane_no].n[1] * near_point[1] +
				frustum[plane_no].n[2] * near_point[2] +
				frustum[plane_no].d >= 0)
				inside_planes_no++;		//one AABB totally inside a this plane
		}
	}
	if (inside_planes_no == 6)
		return 2;	//AABB inside all planes -> totally inside frustum
	return 1;
	
}*/
//check if the polygon p intersects this AABB
/*bool tAABB::polyIntersects(aPolygon *p)
{
	unsigned int v;
	unsigned int no_P =p->getNoOfVertices();
	
	//TEST 1: test polygon AABB against box AABB
	vec3 max,min;
	vec3* temp = 0;
	min = max = *(p->getVertex(1));

	//determine min and max points of polygon
	for (v=2; v<=no_P; v++)
	{
		temp = (vec3*) p->getVertex(v);
		if (min[0]> temp->x) min[0] = temp->x;
		if (min[1]> temp->y) min[1] = temp->y;
		if (min[2]> temp->z) min[2] = temp->z;
		if (max[0]< temp->x) max[0] = temp->x;
		if (max[1]< temp->y) max[1] = temp->y;
		if (max[2]< temp->z) max[2] = temp->z;
	}

	//create an AABB of the min and max vectors
	//tAABB polybox(&min, &max);
	//p->bbox.reset(&min, &max);
	
	//test AABB vs AABB
	//if (!this->AABBintersects(&p->bbox))
	//	return false;
	//TEST 2: if the poly's box intersects, test if AABB intersects the triangle plane
	//			if it does, the polygon overlaps the AABB
	aPlane tri_plane;
	vec3 tmp;
	min = *(vec3*)p->getVertex(2);
	max = *(vec3*)p->getVertex(1);
	min = min - max;			//first edge
	tmp = *(vec3*)p->getVertex(no_P);
	max = *(vec3*)p->getVertex(1);
	max = tmp - max;	//last edge
	
	tri_plane.n = min * max;
	tri_plane.n.normalize();
	tri_plane.d = -1.0f*(tri_plane.n , *p->getVertex(1));
	if (!this->planeIntersects(&tri_plane))
		return false;

	return true; //passes as inide box!
	
	//if no points inside, check if all points outside any plane then it is outside
	//test all six AABB planes, if all points are outside
	/*vec3* vertex;
	short int reject;
	
	// +Y
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( (vertex->y - cen[1]) + y_height/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;
	//-Y
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( ((vertex->y - cen[1])*-1) + y_height/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;
	//+X
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( (vertex->x - cen[0]) + x_width/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;
	//-X
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( ((vertex->x - cen[0])*-1) + x_width/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;
	//+Z
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( (vertex->z - cen[2]) + z_depth/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;
	//-Z
	for (v=0, reject=0; v<no_P; v++)
	{
		vertex = (vec3*) p->getVertex(v+1);
		if ( ( ((vertex->z - cen[2])*-1) + z_depth/2) <= 0)
			reject++;
		else break;
	}
	//if all points are outside this plane, it is outside AABB, so return false
	if (reject==2)
		return false;*/
	//----------COULD BE REMOVED?
	//if all outside, check the edges vs this AABB
	/*for (v=0; v<no_P; v++)
	{
		if (lineIntersects((vec3*) p->getVertex( ((v+1)%no_P)+1),(vec3*) p->getVertex( ((v+2)%no_P)+1)))
			return true;
	}*/
	//----------COULD BE REMOVED?
	//one case left:
	
	// if the AABB is totally inside the points and its edges!
	// TODO: test AABB's planes vs edge-lines in poly instead

	//it is outside the AABB
	
//}
/*bool tAABB::polyIntersects(aPolygon *p)
{
	//setup plane constants
	float A,B,C,D;
	float planes[] = {0,0,-1,-z_depth/2,		//front
		0,0,1,-z_depth/2,					//back
		0,1,0,-y_height/2,					//up
		0,-1,0,-y_height/2,					//down
		1,0,0,-x_width/2,					//right
		-1,0,0,-x_width/2};					//left
	vec3 vert;
	
	unsigned int f, v;
	for (f=0; f<6; f++)
	{
		A = planes[f*4];
		B = planes[f*4+1];
		C = planes[f*4+2];
		D = planes[f*4+3];
			
		//if all points outside one plane, poly outside box!
		for (v=0; v<p->getNoOfVertices(); v++)
		{
			vert = *p->getVertex(v+1);
			vert = vert - *cen;
					
			//point inside plane?
			if (A*vert[0] + B*vert[2] + C*vert[2] + D <= 0)
				break;	//one point inside plane
		}
		//if all points outside box, poly outside
		if (v == p->getNoOfVertices())
			return false;
	}
	return true;
}*/

//reset the aabb to a box round min and max vecs
//##ModelId=3DFB64CE02F0
void tAABB::reset(const vec3 &min, const vec3 &max)
{
	cen = min + ( (max-min)/2);
	x_width = max[0] - min[0];
	y_height = max[1] - min[1];
	z_depth = max[2] - min[2];

	//make AABB cube
	if ( (x_width > y_height) & (x_width > z_depth))
		y_height = z_depth = x_width;
	else if ( (y_height > x_width) & (y_height > z_depth))
		x_width = z_depth = y_height;
	else if ( (z_depth > x_width) & (z_depth > y_height ))
		x_width =  y_height = z_depth;
}
//##ModelId=3DFB64CE0304
void tAABB::join(tAABB *b)
{
	vec3 hi;
	vec3 lo;
	vec3 hi2;
	vec3 lo2;

	vec3 new_hi;
	vec3 new_lo;
	//b's hi and lo
	hi.set(b->cen[0] + b->x_width/2, b->cen[1] + b->y_height/2, b->cen[2] + b->z_depth/2);
	lo.set(b->cen[0] - b->x_width/2, b->cen[1] - b->y_height/2, b->cen[2] - b->z_depth/2);
	
	//this's hi and lo
	hi2.set( cen[0] +  x_width/2,  cen[1] +  y_height/2,  cen[2] +  z_depth/2);
	lo2.set( cen[0] -  x_width/2,  cen[1] -  y_height/2,  cen[2] -  z_depth/2);

	//set new AABB's hi and lo vecs
	new_hi.set((hi[0] > hi2[0]) ? hi[0] : hi2[0],
		(hi[1] > hi2[1]) ? hi[1] : hi2[1],
		(hi[2] > hi2[2]) ? hi[2] : hi2[2]);
	
	new_lo.set((lo[0] < lo2[0]) ? lo[0] : lo2[0],
		(lo[1] < lo2[1]) ? lo[1] : lo2[1],
		(lo[2] < lo2[2]) ? lo[2] : lo2[2]);
	
	//make new_hi and new_lo to new AABB
	this->reset(new_lo, new_hi);

}
//Check a linesegmnet vs AABB
//##ModelId=3DFB64CE0282
/*bool tAABB::lineIntersects(const aLineSegment &line)
{
	return this->lineIntersects(line.getStartPoint(), line.getEndPoint());
}*/
//Check if the line, defined by end and start points, intersects the box. 
// If the closest point on the line (to the box mid point) is
// inside the AABB, it intersects the AABB. Thus returning TRUE.
//	
// submit start and end in WORLD SPACE!
//##ModelId=3DFB64CE026D
/*bool tAABB::lineIntersects(const vec3 &start, const vec3 &end)
{
	// [world space]
	// closest_point = start + line_dir * (line_dir , ( AABB_mid_p - start));
	// line_dir = (end - start).normalize()
	if (pointInside(start))
		return true;
	else if (pointInside(end))
		return true;

	float val, dot;
	vec3 line = end - start;

	//length of line
	val = line.length();
	line.normalize();

	//projection of center of AABB on line_direction vector
	dot = ((cen - start), line );
	
	if ( (dot < 0) | (dot >= val) )
		return false;	//closest point is not on line!
	return pointInside( (line * dot) );
	//return pointInside( &(*start + ( (*end-*start).normalize() , (*cen - *start))) );
}*/
/*bool tAABB::planeIntersects(const aPlane &plane)
{
	vec3 min;
	//find min and max of AABB in Normals direction of plane
	if (plane.n[0] > 0)	//POS n[0]
	{
		min[0] = cen[0] + x_width/2;
	}
	else	//NEG n[0]
	{
		min[0] = cen[0] - x_width/2; 
	}
	if (plane.n[1] > 0)	//POS n[1]
	{
		min[1] = cen[1] + y_height/2; 
	}
	else	//NEG n[1]
	{
		min[1] = cen[1] - y_height/2;
	}
	if (plane.n[2] > 0)	//POS n[2]
	{
		min[2] = cen[2] + z_depth/2; 
	}
	else	//NEG n[2]
	{
		min[2] = cen[2] - z_depth/2; 
	}
	//check distance to closest point
	if (plane.n[0] * min[0] +
		plane.n[1] * min[1] +
		plane.n[2] * min[2] +
		plane.d < 0)
		return 0;	//Near outside plane
	
	return true;
}*/
//is point p insisde this AABB, check the coords
// if it lies inside or on any plane, return true
// if it is outside any plane, return false
bool tAABB::pointInside(const vec3 &p)
{ 
	//check distance to center with half of width, height and depth
	if (  (fabs( p[0] - cen[0] ) < (x_width/2.0))  &
		(fabs( p[1] - cen[1] ) < (y_height/2.0)) &
		(fabs( p[2] - cen[2] ) < (z_depth/2.0)))
		return true;
	return false;
}

/*bool tAABB::sphereIntersects(aBoundingSphere *s)
{
	//if center is inside AABB
	
	//TODO: aBoundingSphere acts like a BITCH when const aB.. &s is used

	vec3 p = s->getTranslation();
	if ( pointInside( p ) )
		return true;
	
	//check distance to midpoint of AABB
	vec3 d;
	float r = s->getRadii();
	d = s->getTranslation() - cen;

	// set D to a point on the sphere
	d = d*-(r/d.length());
	
	if (pointInside(d))
		return true;
	else
		return false;

}*/

//Fast boolean intersection test
bool tAABB::rayIntersects(const tRay &ray) {
	// [world space]
	// closest_point = start + line_dir * (line_dir , ( AABB_mid_p - start));
	// line_dir = (end - start).normalize()
	/*if (pointInside(ray.start))
		return true;
	
	//projection of center of AABB on line_direction vector
	double dot = ((cen - ray.start), ray.dir );
	
	//check closest point
	return pointInside( ray.start + (ray.dir * dot) );	*/
	tIntersection hit;
	return intersection(ray, hit) > 0;
}
/*
	Finds actual intersection point
*/
//#define POSITIVE -1;
//#define NEGATIVE 1;
//#define INSIDE 0;
double tAABB::intersection(const tRay &ray, tIntersection &hit) { 
	//TODO: implement tAABB::intersection(..)
	static const int POSITIVE = 1, NEGATIVE = -1, INSIDE = 0;
	//Algo based on Andrew Woo's algo in graphics gems 1
	int classification[3];	//Classify x,y,z as +,- or middle
	double candidatePlane[3];
	double maxDist[3];				//max distances
	bool inside = true;
	int choosenPlane = 0;
	vec3 min,max;
	min = cen - vec3(x_width*0.5, y_height*0.5, z_depth*0.5);
	max = cen + vec3(x_width*0.5, y_height*0.5, z_depth*0.5);
	/*
		Find three closest planes relative ray.start
	*/
	for (int i=0; i<3; i++) {
		if (ray.start[i] < min[i]) {
			classification[i] = NEGATIVE;
			candidatePlane[i] = min[i];
			inside = false;
		}
		else if (ray.start[i] > max[i]) {
			classification[i] = POSITIVE;
			candidatePlane[i] = max[i];
			inside = false;
		}
		else {
			classification[i] = INSIDE;
		}
	}
	/* Origin inside */
	if (inside) {
		//TODO: if inside, find intersect from inside to outside
		hit.P = ray.start;
		vec3 bert;// = max - vec3(x_width*0.5, y_height*0.5, z_depth*0.5);
		bert = ray.start - cen;
		double maxVals[3] = {fabs(bert[0]), fabs(bert[1]), fabs(bert[2])};
		if (maxVals[0] > maxVals[1] && maxVals[0] > maxVals[2]) {
			hit.N.set(maxVals[0],0,0);
		}
		else if (maxVals[1] > maxVals[0] && maxVals[1] > maxVals[2]) {
			hit.N.set(0, maxVals[1],0);
		}
		else {
			hit.N.set(0, 0,maxVals[2]);
		}
		hit.N.normalize();
		return SMALL_VAL;
	}
	/*
		find intersection with these 3 planes
	*/
	for (int i=0; i<3; i++) {
		if (classification[i] != INSIDE) {
			if (fabs(ray.dir[i]) > SMALL_VAL)
				maxDist[i] = (candidatePlane[i] - ray.start[i])/ ray.dir[i];
			else
				maxDist[i] = signum(ray.dir[i])*LARGE_VAL;
		}
		else {
			maxDist[i] = -1.0;
		}
	}
	
	/*
		farthest -> candidate plane
	*/
	//choosenPlane=0; //set to first
	for (int i=1; i<3; i++)
		if (maxDist[choosenPlane] < maxDist[i])
			choosenPlane = i;
	/*
		test intersection point on candidate plane
			vs extent of that plane
	*/
	if (maxDist[choosenPlane] < 0 ) 
		return -1;
	double t = -1;
	double coord[3];
	for (int i=0; i<3; i++) {
		if (choosenPlane != i) {
			coord[i] = ray.start[i] + maxDist[choosenPlane]*ray.dir[i];
			if ((classification[i] == POSITIVE && coord[i] < min[i]) ||
				(classification[i] == NEGATIVE && coord[i] > max[i])) 
			{
				//check all dimensions
                return -1;
			}
		}
		else {
			coord[i] = candidatePlane[i];
		}
	}
	hit.P.set(coord[0], coord[1], coord[2]);
	vec3 Norm;
	Norm[choosenPlane] = classification[choosenPlane]; 
	/*switch (choosenPlane) {
		case 0:
			
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}*/
		
	return maxDist[choosenPlane];
}
bool tAABB::polyIntersects(tPolygon *p){
	vec3 pelle;
	for (size_t i=0; i<p->vertices.size(); i++) {
		pelle.set(p->vertices.at(i)->x, p->vertices.at(i)->y, p->vertices.at(i)->z);
		if (this->pointInside(pelle))
			return true;
	}
	return false;
}
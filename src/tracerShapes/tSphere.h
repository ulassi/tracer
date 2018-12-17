#pragma once
#include <vec3.h>
#include "tshape.h"
#include "typedefs.h"

//tMath stuff
extern int sign(double);
extern double fdiv(double, double);
extern void solveLinear(double &a, double &b, double &x1);
extern void solveQuadraticREAL(double &a, double &b, double &c, double &x1, double &x2, int &realVals );

/*
	tSphere class
		An implementation of a sphere shape
		
		(x-center[0])^2 + (y-center[1])^2 + (z-center[2])^2 = r^2
*/
class tSphere :	public tShape
{
public:
	vec3 center;
	double r;
	tSphere(void){ r = 0.5; }
	tSphere(const tSphere &c) { *this = c; }
	tSphere & operator= (const tSphere &right) { 
		this->center = right.center;
		this->r = right.r;
		this->setShader(right.shader);
		this->setUVMapper(right.uvmapper);
		this->transform = right.transform;
		return *this; 
	};
	tSphere(const vec3 &_center, const double &_r) {center = _center; r = _r;}; 
	virtual ~tSphere(void) {};
	tSphere * clone() const { return new tSphere(*this);};
	/*
		Assumes point is an intersection
	*/
	virtual vec3 normal(vec3 &point) {
		double one_over_r = fdiv(1,r);
		vec3 centerTF = this->transform* center;
		vec3 N = (point - centerTF)*one_over_r;
		return N;

	};
	virtual double intersection(const tRay &ray, tIntersection &hit) { 
		//solve quadratic given in Adv rendering and animation pp 225
		double a,b,c,t=-1,x1,x2;

		//fast rejection
		if (!rayIntersects(ray)) {
			return -1;
		}
		a = 1;
		vec3 centerTF = this->transform*center;
		vec3 SminusC = ray.start - centerTF;
		b = 2*(ray.dir , SminusC);
		c = (SminusC, SminusC) - r*r;
		//# If B2-4AC<0, the ray and sphere do not intersect
		//# If B2-4AC=0, ray grazes sphere
		//# If B2-4AC>0, the smallest positive t corresponds to the intersection 
		int vals = 0;
		solveQuadraticREAL(a,b,c,x1,x2, vals);
		if (vals == 0)
			t = -1;
		else if (vals == 1){
			t = x1;
		}
		else {
			if (x1 > 0 && x2 > 0) {
				if (x1 < x2)
					t =  x1;
				else 
					t =  x2;
			}
			else {
				if (fabs(x1) < fabs(x2))
					t =  x1;
				else
					t =  x2;
			}
		}
		if (t > 0) {
			vec3 p = ray.start + ray.dir * t;
			hit.P = p;
			hit.N = this->normal(p);
			//query texturemapper, if it exist, for uv coords
			if (this->uvmapper) {
				double4 uv;
				std::vector<double> data;
				data.push_back(p[0] - centerTF[0]); 
				data.push_back(p[1]- centerTF[1]); 
				data.push_back(p[2]- centerTF[2]);
				double2 uv2d = this->uvmapper->getMappingCoords(data);
				uv.x = uv2d.x;	uv.y = uv2d.y;	uv.z = 0; uv.w = 0;
				hit.uvs.push_back(uv);
			}
		}
		
        return t;
	};

	/*
		Does a fast ray vs sphere test, to find if
		the ray intersects the sphere at all.
	*/
	bool rayIntersects(const tRay &ray) {
		vec3 centerTF = this->transform*this->center;
		vec3 StoC = centerTF - ray.start;	//start to center
		double len = StoC.lengthSquared();
		if (len <= r*r) 
			return true; //start inside sphere
		
		//project center onto line
		double dot = (StoC, ray.dir); //dist to closest point	
		vec3 closest_point = centerTF - (ray.start + ray.dir * dot);
		if (closest_point.lengthSquared() <= r*r) 
			return true;
		else
			return false;
	};
	
};

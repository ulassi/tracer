//Type definitions and useful structures
#pragma once
#ifndef LARGE_VAL
#define LARGE_VAL 1e10
#endif
#ifndef SMALL_VAL
#define SMALL_VAL 1e-5
#endif
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
#ifndef PI_HALF
#define PI_HALF 1.5707963267948966192313216916398
#endif
#ifndef ONE_OVER_PI
#define ONE_OVER_PI 0.31830988618379067153776752674503
#endif
typedef unsigned int uint;
#define ERRSTR "ERROR at " << __FILE__ << " function: " << __FUNCTION__ << " line: " << __LINE__ << std::endl
//#define signum(f)( f >= 0 ? 1: -1)
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
struct float2{
	float x, y;
};
struct float3{
	float x,y,z;
};
struct float4{
	float x,y,z,w;
};
struct double2{
	double x, y;
};
struct double3{
	double x,y,z;
};
struct double4{
	double x,y,z,w;
};

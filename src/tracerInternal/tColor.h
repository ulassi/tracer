#pragma once
#include <math.h>
/*
	R,G,B color vector
*/
class tColor
{
public:
	tColor(void) : R(0), G(0), B(0) { };
	tColor(const tColor &col) { R = col.R; G = col.G; B = col.B;};
	tColor(double _r, double _g, double _b) { R = _r; G = _g; B = _b;};
	tColor(double _i) {R = G = B = _i;};
	~tColor(void) {};

	double R,G,B;
	inline double length() const { 
		return sqrt(R*R + G*G + B*B);
	};
	inline bool operator== (const tColor &r) const {
		return (r.R == this->R) &&
			(r.G == this->G) &&
			(r.B == this->B);
	};
	inline bool operator!= (const tColor &r) const {
		return !(*this == r);
	};
	inline tColor & operator+= (const tColor &r) {
		*this = *this + r;
		return *this;
	};
	inline tColor & operator*= (const double &r) {
		*this = *this *r;
		return *this;
	};
	inline tColor & operator*= (const tColor &c) {
		*this = *this * c;
		return *this;
	};
	inline tColor & operator/= (const double &r) {
		*this = *this /r;
		return *this;
	};
	inline tColor & operator= (const tColor &r) {
		R= r.R; G= r.G; B=r.B;
		return *this;
	};
	inline tColor & operator= (const double &i) {
		R= G= B=i;
		return *this;
	};
	inline tColor operator+ (const tColor &v) const{
		tColor col(this->R + v.R, this->G + v.G, this->B + v.B);
		return col;		
	};
	inline tColor operator- (const tColor &v) const{
		tColor col(this->R - v.R, this->G - v.G, this->B - v.B);
		return col;		
	};
	inline tColor operator* (const double &v) const{
		tColor col(this->R *v, this->G *v, this->B *v);
		return col;		
	};
	inline tColor operator* (const tColor &c) const{
		tColor col(this->R *c.R, this->G *c.G, this->B *c.B);
		return col;		
	};
	inline tColor operator/ (const double &v) const{
		tColor col(this->R /v, this->G /v, this->B /v);
		return col;		
	};
	inline operator double () const;
};
inline tColor operator* (const double &m, const tColor &c) {
	return tColor(c.R * m, c.G*m, c.B*m);
};
inline tColor::operator double () const {
	return this->R;
}
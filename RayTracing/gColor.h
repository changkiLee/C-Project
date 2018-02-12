#pragma once
#include <cassert>

class GColor
{
	friend GColor operator +(const GColor &lhs, const GColor &rhs);
	friend GColor operator *(const GColor &lhs, const GColor &rhs);
	friend GColor operator *(const GColor &lhs, const double &s);
	friend GColor operator *(const double &s, const GColor &rhs);
	friend GColor cast_color(const double *c);
public:
	GColor(double r = 0.0, double g = 0.0, double b = 0.0);
	GColor(double* c);
	GColor(const GColor &cpy);
	virtual ~GColor();
	GColor &operator =(const GColor &rhs);
	GColor &operator +=(const GColor &rhs);
	GColor &operator *=(const double &s);
	GColor &operator *=(const GColor &lhs);

	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;
	void setColor(const double &r = 0.0, const double &g = 0.0, const double &b = 0.0);
	unsigned char getRed() const;
	unsigned char getGreen() const;
	unsigned char getBlue() const;
private:
	double color[3];
};
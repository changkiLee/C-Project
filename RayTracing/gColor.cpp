#include "gColor.h"

GColor::GColor(double r, double g, double b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

GColor::GColor(double *c)
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}


GColor::GColor(const GColor &cpy)
{
	color[0] = cpy.color[0];
	color[1] = cpy.color[1];
	color[2] = cpy.color[2];
}

 GColor::~GColor()
{
}

 GColor & GColor::operator=(const GColor & rhs)
{
	color[0] = rhs.color[0];
	color[1] = rhs.color[1];
	color[2] = rhs.color[2];
	return *this;
}


GColor operator +(const GColor &lhs, const GColor &rhs)
{
	return GColor(lhs.color[0] + rhs.color[0], lhs.color[1] + rhs.color[1], lhs.color[2] + rhs.color[2]);
}


GColor operator *(const GColor &lhs, const GColor &rhs)
{
	return GColor(lhs.color[0] * rhs.color[0], lhs.color[1] * rhs.color[1], lhs.color[2] * rhs.color[2]);
}


GColor &GColor::operator +=(const GColor &rhs)
{
	color[0] += rhs.color[0];
	color[1] += rhs.color[1];
	color[2] += rhs.color[2];
	return *this;
}


GColor operator *(const GColor &lhs, const double &s)
{
	GColor ret(lhs);
	ret *= s;
	return ret;
}


GColor operator *(const double &s, const GColor &rhs)
{
	GColor ret(rhs);
	ret *= s;
	return ret;
}

 GColor cast_color(const double * c)
{
	return GColor(c[0], c[1], c[2]);
}


GColor &GColor::operator *=(const double &s)
{
	color[0] *= s;
	color[1] *= s;
	color[2] *= s;
	return *this;
}

GColor &GColor::operator *=(const GColor &lhs)
{
	color[0] *= lhs.color[0];
	color[1] *= lhs.color[1];
	color[2] *= lhs.color[2];
	return *this;
}

double &GColor::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return color[idx];
}


const double &GColor::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return color[idx];
}


void GColor::setColor(const double &r, const double &g, const double &b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

unsigned char GColor::getRed() const
{
	if (color[0] < 1.0)
		return unsigned char(color[0] * 255.0);
	else
		return 255;
}
unsigned char GColor::getGreen() const
{
	if (color[1] < 1.0)
		return unsigned char(color[1] * 255.0);
	else
		return 255;
}
unsigned char GColor::getBlue() const
{
	if (color[2] < 1.0)
		return unsigned char(color[2] * 255.0);
	else
		return 255;
}
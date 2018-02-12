#pragma once

#include "gObject.h"

class GSphere : public GObject
{
public:
	GSphere();
	virtual ~GSphere();
	GSphere(e_ObjectType _type, GColor _ambient, GColor _diffuse, GColor _specular, double _shn,
		double _reflection, double _refraction, double _idxr,
		GPoint3 _position, double _radious);
	GPoint3 getPosition() const;
	double getRadious() const;
private:
	GPoint3 position;
	double radious;
};
inline GSphere::GSphere()
{
	GObject();
	position.Set(0, 0, 0);
	radious = 0;
}

inline
GSphere::GSphere(e_ObjectType _type, GColor _ambient, GColor _diffuse, GColor _specular, double _shn, 
	double _reflection, double _refraction, double _idxr,
	GPoint3 _position, double _radious)
	: GObject(_type, _ambient, _diffuse, _specular, _shn, _reflection, _refraction, _idxr), position(_position), radious(_radious)
{
}
inline GSphere::~GSphere()
{
}

inline GPoint3 GSphere::getPosition() const
{
	return position;
}

inline double GSphere::getRadious() const
{
	return radious;
}

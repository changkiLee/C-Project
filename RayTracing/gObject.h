#pragma once

#include "gMath.h"
#include "gColor.h"
class GObject
{
public:
	enum e_ObjectType { SPHERE, TRIANGLE, VERTEX };
	GObject();
	GObject(e_ObjectType _type, GColor _ambient, GColor _diffuse, GColor _specular, double _shn, double _reflection, double _refraction, double _idxr);
	virtual ~GObject();

	GColor getAmbient() const;
	GColor getDiffuse() const;
	GColor getSpecular() const;
	double getShiness() const;
	double getReflection() const;
	double getRefraction() const;
	double getIdxr() const;

	void setAmbient(const GColor &amb);
	void setDiffuse(const GColor &diff);
	void setSpecular(const GColor &spec);
	void setShiness(const double shn);
	void setReflection(const double reflect);
	void setRefraction(const double refract);
	void setIdxr(const double idxr);

	e_ObjectType type;
protected:
	GColor ambient;
	GColor diffuse;
	GColor specular;
	double shn;
	double reflection;
	double refraction;
	double idxr;
};
inline GObject::GObject()
{
	ambient.setColor(0, 0, 0);
	diffuse.setColor(0, 0, 0);
	specular.setColor(0, 0, 0);
	shn = 0.0;
	reflection = 0.0;
	refraction = 0.0;
	idxr = 0.0;
}
inline
GObject::GObject(e_ObjectType _type, GColor _ambient, GColor _diffuse, GColor _specular, double _shn, double _reflection, double _refraction, double _idxr)
	: type(_type), ambient(_ambient), diffuse(_diffuse), specular(_specular), shn(_shn), reflection(_reflection), refraction(_refraction), idxr(_idxr)
{

}

inline GObject::~GObject()
{
}

inline double GObject::getReflection() const
{
	return reflection;
}

inline double GObject::getRefraction() const
{
	return refraction;
}

inline double GObject::getIdxr() const
{
	return idxr;
}

inline void GObject::setAmbient(const GColor & amb)
{
	ambient = amb;
}

inline void GObject::setDiffuse(const GColor & diff)
{
	diffuse = diff;
}

inline void GObject::setSpecular(const GColor & spec)
{
	specular = spec;
}

inline void GObject::setShiness(const double shn)
{
	this->shn = shn;
}

inline void GObject::setReflection(const double reflect)
{
	reflection = reflect;
}

inline void GObject::setRefraction(const double refract)
{
	refraction = refract;
}

inline void GObject::setIdxr(const double idxr)
{
	this->idxr = idxr;
}

inline GColor GObject::getAmbient() const
{
	return ambient;
}

inline GColor GObject::getDiffuse() const
{
	return diffuse;
}

inline GColor GObject::getSpecular() const
{
	return specular;
}

inline double GObject::getShiness() const
{
	return shn;
}

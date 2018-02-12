#pragma once

#include "gMath.h"
#include "gColor.h"
class GLight
{
public:
	enum e_LightType { POINT, DIRECTIONAL, SPOT };
	GLight();
	GLight(e_LightType _type, GColor _ambient, GColor _diffuse, GColor _specular);
	virtual ~GLight();
	GColor getAmbient() const;
	GColor getDiffuse() const;
	GColor getSpecular() const;
	e_LightType type;
private:
	GColor ambient;
	GColor diffuse;
	GColor specular;
};

inline GLight::GLight()
{
	type = e_LightType::POINT;
	ambient.setColor(0, 0, 0);
	diffuse.setColor(0, 0, 0);
	specular.setColor(0, 0, 0);
}

inline
GLight::GLight(e_LightType _type, GColor _ambient, GColor _diffuse, GColor _specular)
	: type(_type), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{

}

inline GLight::~GLight()
{
}

inline GColor GLight::getAmbient() const
{
	return ambient;
}

inline GColor GLight::getDiffuse() const
{
	return diffuse;
}

inline GColor GLight::getSpecular() const
{
	return specular;
}

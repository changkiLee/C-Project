#pragma once
#include "gLight.h"

class GPointLight : public GLight
{
public:
	GPointLight();
	GPointLight(e_LightType _type, GColor _ambient, GColor _diffuse, GColor _specular,
		GPoint3 _position);
	virtual ~GPointLight();

	GPoint3 getPosition() const;
private:
	GPoint3 position;
};
inline GPointLight::GPointLight()
{
	position.Set(0, 0, 0);
}
inline
GPointLight::GPointLight(e_LightType _type, GColor _ambient, GColor _diffuse, GColor _specular, GPoint3 _position)
	: GLight(_type, _ambient, _diffuse, _specular), position(_position)
{

}

inline GPointLight::~GPointLight()
{
}

inline GPoint3 GPointLight::getPosition() const
{
	return position;
}

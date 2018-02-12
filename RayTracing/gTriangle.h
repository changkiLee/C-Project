#pragma once
#include "gObject.h"

class GVertex;
class GTriangle;

class GVertex : public GObject
{
public:
	GVertex();
	GVertex(const GVertex &cpy);
	GVertex(e_ObjectType _type,	GColor _ambient, GColor _diffuse, GColor _specular,	double _shn, double _reflection, double _refraction, double _idxr,
		GPoint3 _position, GVector3 _normal);
	virtual ~GVertex();
	GVertex &operator =(const GVertex &rhs);

	GPoint3 getPosition() const;
	GVector3 getNormal() const;

private:
	GPoint3 position;
	GVector3 normal;
};

inline GVertex::GVertex()
{
	position.Set(0, 0, 0);
	normal.Set(0, 0, 0);
}

inline
GVertex::GVertex(e_ObjectType _type, GColor _ambient, GColor _diffuse, GColor _specular, double _shn, double _reflection, double _refraction, double _idxr,
	GPoint3 _position, GVector3 _normal) :
	GObject(_type, _ambient, _diffuse, _specular, _shn, _reflection, _refraction, _idxr), position(_position), normal(_normal)
{

}

inline GVertex::GVertex(const GVertex & cpy)
{
	type = cpy.type;
	ambient = cpy.ambient;
	diffuse = cpy.diffuse;
	specular = cpy.specular;	
	shn = cpy.shn;
	reflection = cpy.reflection;
	refraction = cpy.refraction;
	idxr = cpy.idxr;
	position = cpy.position;
	normal = cpy.normal;
}

inline GVertex::~GVertex()
{
}

inline GVertex & GVertex::operator=(const GVertex & rhs)
{
	type = rhs.type;
	ambient = rhs.ambient;
	diffuse = rhs.diffuse;
	specular = rhs.specular;
	shn = rhs.shn;
	idxr = rhs.idxr;
	reflection = rhs.reflection;
	refraction = rhs.refraction;
	position = rhs.position;
	normal = rhs.normal;
	return *this;
}

inline GPoint3 GVertex::getPosition() const
{
	return position;
}

inline GVector3 GVertex::getNormal() const
{
	return normal;
}

class GTriangle : public GObject
{
public:
	GTriangle();
	GTriangle(e_ObjectType _type, GVertex v0, GVertex v1, GVertex v2);
	GTriangle(const GTriangle &cpy);
	virtual ~GTriangle();

	GTriangle &operator =(const GTriangle &rhs);
	GVertex getVertex(int idx) const;
	GVector3 getNormal() const;

	void setNormal(const GVector3& normal);

private:
	GVertex verts[3];
	GVector3 faceNormal;
};

inline
GTriangle::GTriangle()
{
	type = GObject::TRIANGLE;
	faceNormal.Set(0, 0, 0);
}

inline
GTriangle::GTriangle(e_ObjectType _type, GVertex v0,  GVertex v1, GVertex v2)
{
	type = _type;
	verts[0] = v0;
	verts[1] = v1;
	verts[2] = v2;
	faceNormal = (v0.getNormal() + v1.getNormal() + v2.getNormal()) / 3;
	faceNormal.Normalize();
}

inline GTriangle::~GTriangle()
{
}

inline GTriangle::GTriangle(const GTriangle & cpy)
{
	type = cpy.type;
	verts[0] = cpy.verts[0];
	verts[1] = cpy.verts[1];
	verts[2] = cpy.verts[2];
	faceNormal = cpy.faceNormal;

	ambient = cpy.ambient;
	diffuse = cpy.diffuse;
	specular = cpy.specular;
	shn = cpy.shn;
	reflection = cpy.reflection;
	refraction = cpy.refraction;
	idxr = cpy.idxr;
}

inline GTriangle & GTriangle::operator=(const GTriangle & rhs)
{
	type = rhs.type;
	verts[0] = rhs.verts[0];
	verts[1] = rhs.verts[1];
	verts[2] = rhs.verts[2];
	faceNormal = rhs.faceNormal;

	ambient = rhs.ambient;
	diffuse = rhs.diffuse;
	specular = rhs.specular;
	shn = rhs.shn;
	reflection = rhs.reflection;
	refraction = rhs.refraction;
	idxr = rhs.idxr;
	return *this;
}

inline GVertex GTriangle::getVertex(int idx) const
{
	assert(idx >= 0 && idx < 3);
	return verts[idx];
}

inline GVector3 GTriangle::getNormal() const
{
	return faceNormal;
}

inline void GTriangle::setNormal(const GVector3 & normal)
{
	faceNormal = normal;
	faceNormal.Normalize();
}

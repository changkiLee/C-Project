#pragma once

#include "gTriangle.h"
#include "gSphere.h"
#include "gPointLight.h"

struct IntersectVertex
{
	int obj;
	double idxr;
	GPoint3 collisionPoint;
	GVector3 surfaceNormal;
};

class GRayTracer
{
public:
	GRayTracer();
	virtual ~GRayTracer();
	void setCamera(const GPoint3& _cameraPos, const GPoint3& _lookAt, const GVector3& upVector, const double fov = 60.0);
	void setResolution(const int _width = 640, const int _height = 480);
	void loadScene(const char* argv);

	IntersectVertex closestIntersection(GLine& tempRay);
	GColor trace(GLine& tempRay, int level);
	GColor singlePixelTrace(float x, float y);
	GColor shade(IntersectVertex& tempResult, GLine& tempRay, int level);
	GColor localIllumination(IntersectVertex& tempResult, GLine& tempRay, int rightSource);
	double shadowRay(IntersectVertex& tempResult, GLine& tempRay, int rightSource);
	GLine reflectionRay(IntersectVertex& tempResult, GLine& tempRay);
	GLine refractionRay(IntersectVertex& tempResult, GLine& tempRay);
	void setMaxLevel(int _level);
private:
	int width;
	int height;
	int maxLevel;
	double shadowRate;
	GColor backGroundColor;
	GColor invalidColor;
	GMatrix camera;
	GPoint3 cameraPos;
	GPoint3 lookAt;
	GVector3 lookVec;
	std::vector<GLight*> vLight;
	std::vector<GObject*> vObject;
};
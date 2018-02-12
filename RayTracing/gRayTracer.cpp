#include "gRayTracer.h"
#include <string.h>
#ifdef WIN32
#define strcasecmp _stricmp
#endif

struct Vertex
{
	double position[3];
	double normal[3];
	double color_diffuse[3];
	double color_specular[3];
	double shininess;
};

struct Triangle
{
	Vertex v[3];
};

struct Sphere
{
	double position[3];
	double radius;
	double color_diffuse[3];
	double color_specular[3];
	double shininess;
};

struct Light
{
	double position[3];
	double color[3];
};

GRayTracer::GRayTracer()
	: camera(3, 3), shadowRate(0.1), backGroundColor(1.0, 1.0, 1.0), invalidColor(0.0, 0.0, 0.0)
{
	width = 640;
	height = 480;
	maxLevel = 1;
}

GRayTracer::~GRayTracer()
{
}

void GRayTracer::setResolution(int _width, int _height)
{
	width = _width;
	height = _height;
}

void GRayTracer::setCamera(const GPoint3& _cameraPos, const GPoint3& _lookAt, const GVector3& _upVector, const double fov)
{
	cameraPos = _cameraPos;
	lookAt = _lookAt;
	GVector3 upVector = _upVector;
	upVector.Normalize();
	lookVec = _lookAt - _cameraPos;
	GVector3 _lookVec = lookVec;
	GVector3 _uVector = (_lookVec ^ upVector).Normalize();
	GVector3 _viewVec = (_uVector ^_lookVec).Normalize();
	GVector3 _oVector =
		(GVector3(_lookVec).Normalize() * ((double)width * 0.5 / tan(DEG2RAD(fov*0.5))))
		- ((width * 0.5) * _uVector) - (((double)height * 0.5) * _viewVec);

	camera.SetColVec(0, cast_vec(_uVector));
	camera.SetColVec(1, cast_vec(_viewVec));
	camera.SetColVec(2, cast_vec(_oVector));
}

void parse_check(const char *expected, char *found)
{
	if (strcasecmp(expected, found))
	{
		printf("Expected '%s ' found '%s '\n", expected, found);
		printf("Parse error, abnormal abortion\n");
		exit(0);
	}
}

void parse_doubles(FILE* file, const char *check, double p[3])
{
	char str[100];
	fscanf(file, "%s", str);
	parse_check(check, str);
	fscanf(file, "%lf %lf %lf", &p[0], &p[1], &p[2]);
	//printf("%s %lf %lf %lf\n", check, p[0], p[1], p[2]);
}

void parse_rad(FILE *file, double *r)
{
	char str[100];
	fscanf(file, "%s", str);
	parse_check("rad:", str);
	fscanf(file, "%lf", r);
	//printf("rad: %f\n", *r);
}

void parse_shi(FILE *file, double *shi)
{
	char s[100];
	fscanf(file, "%s", s);
	parse_check("shi:", s);
	fscanf(file, "%lf", shi);
	//printf("shi: %f\n", *shi);
}

void GRayTracer::loadScene(const char* argv)
{
	FILE * file = fopen(argv, "r");
	int number_of_objects;
	char type[50];
	double ambient_light[3];
	
	//
	double reflection = 0.5;
	double refraction = 0.5;
	//
	double idxr = 1.03;	// coefficient of triangles
	double idxr2 = 1.0;	// coefficient of spheres
	
	Triangle t;
	Sphere s;
	Light l;

	fscanf(file, "%i", &number_of_objects);
	printf("number of objects: %i\n", number_of_objects);
	parse_doubles(file, "amb:", ambient_light);

	for (int i = 0; i<number_of_objects; i++)
	{
		fscanf(file, "%s\n", type);
		//printf("%s\n", type);
		if (strcasecmp(type, "triangle") == 0)
		{
			//printf("found triangle\n");
			for (int j = 0; j < 3; j++)
			{
				parse_doubles(file, "pos:", t.v[j].position);
				parse_doubles(file, "nor:", t.v[j].normal);
				parse_doubles(file, "dif:", t.v[j].color_diffuse);
				parse_doubles(file, "spe:", t.v[j].color_specular);
				parse_shi(file, &t.v[j].shininess);
			}
			GVertex v0(GObject::VERTEX, cast_color(ambient_light), cast_color(t.v[0].color_diffuse), cast_color(t.v[0].color_specular), t.v[0].shininess,
						reflection, refraction, idxr, cast_pt3(t.v[0].position), cast_vec3(t.v[0].normal));
			GVertex v1(GObject::VERTEX, cast_color(ambient_light), cast_color(t.v[1].color_diffuse), cast_color(t.v[1].color_specular), t.v[1].shininess,
						reflection, refraction, idxr, cast_pt3(t.v[1].position), cast_vec3(t.v[1].normal));
			GVertex v2(GObject::VERTEX, cast_color(ambient_light), cast_color(t.v[2].color_diffuse), cast_color(t.v[2].color_specular), t.v[2].shininess,
						reflection, refraction, idxr, cast_pt3(t.v[2].position), cast_vec3(t.v[2].normal));
			vObject.push_back(new GTriangle(GObject::TRIANGLE, v0, v1, v2));
		}
		else if (strcasecmp(type, "sphere") == 0)
		{
			//printf("found sphere\n");

			parse_doubles(file, "pos:", s.position);
			parse_rad(file, &s.radius);
			parse_doubles(file, "dif:", s.color_diffuse);
			parse_doubles(file, "spe:", s.color_specular);
			parse_shi(file, &s.shininess);
			vObject.push_back(new GSphere(GObject::SPHERE, cast_color(ambient_light), cast_color(s.color_diffuse), cast_color(s.color_specular), s.shininess,
				reflection, refraction, idxr2, cast_pt3(s.position), s.radius));
			idxr2 += 0.2;
		}
		else if (strcasecmp(type, "light") == 0)
		{
			//printf("found light\n");
			parse_doubles(file, "pos:", l.position);
			parse_doubles(file, "col:", l.color);

			vLight.push_back(new GPointLight(GLight::POINT, cast_color(ambient_light), cast_color(l.color), GColor(1, 1, 1),
				cast_pt3(l.position)));
		}
		else
		{
			printf("unknown type in scene description:\n%s\n", type);
			exit(0);
		}
	}
	fclose(file);
}

GColor GRayTracer::trace(GLine& tempRay, int level)
{
	if (level > maxLevel)
		return invalidColor;

	IntersectVertex tempResult = closestIntersection(tempRay);

	if (tempResult.obj == -1)	// no intersection
	{
		if (level == 0)			// havn't collision 
			return backGroundColor;
		else
			return invalidColor;
	}
	else
	{
		return shade(tempResult, tempRay, level);
	}
}

GVector3 barycentric(const GPoint3& p0, const GPoint3& p1, const GPoint3& p2, const GPoint3& innerP)
{
	double areaS = norm((p1 - p0)^(p2 - p0));
	double areaA = norm((p1 - innerP) ^ (p2 - innerP));
	double areaB = norm((p2 - innerP) ^ (p0 - innerP));
	double areaC = norm((p0 - innerP) ^ (p1 - innerP));
	double a = areaA / areaS;
	double b = areaB / areaS;
	double c = areaC / areaS;

	return GVector3(a, b, c);
}

IntersectVertex GRayTracer::closestIntersection(GLine& tempRay)
{
	double far = 999999.0;
	IntersectVertex tempResult;
	tempResult.obj = -1;	// -1 : no intersection

	for (unsigned int q = 0; q < vObject.size(); ++q)
	{
		if (vObject[q]->type == GObject::SPHERE)
		{
			GSphere* tempSphere = (GSphere*)vObject[q];
			GVector3 distanceVec3 = tempSphere->getPosition() - tempRay.GetPt();
			double t = distanceVec3 * tempRay.GetDir();

			if (tempRay.outside)	// ray is outside
				if (t < 1.0e-5)
					continue;		// skip

			// collision check
			double Bsqr2 = SQR(norm(distanceVec3)) - SQR(t);
			if (SQRT(Bsqr2) <= tempSphere->getRadious() + 1.0e-5) // collision
			{
				double tu = SQRT(SQR(tempSphere->getRadious()) - Bsqr2);

				if (tempRay.outside)
					t -= tu;		// ray origin outside sphere
				else
					t += tu;		// ray origin inside sphere

				GPoint3 collisionPoint = tempRay.Eval(t);

				if (far > abs(norm(collisionPoint - tempRay.GetPt())))
				{
					far = abs(norm(collisionPoint - tempRay.GetPt()));
					tempResult.obj = q;
					tempResult.collisionPoint = collisionPoint;
					tempResult.surfaceNormal = collisionPoint - tempSphere->getPosition();
					tempResult.surfaceNormal.Normalize();
					if (!tempRay.outside)
						tempResult.surfaceNormal = -tempResult.surfaceNormal;
					tempResult.idxr = tempSphere->getIdxr();
				}
			}
		}
		else if (vObject[q]->type == GObject::TRIANGLE)
		{
			GTriangle* tempTriangle = (GTriangle*)vObject[q];
			if (tempRay.outside)
			{
				GVertex v0 = tempTriangle->getVertex(0);
				GVertex v1 = tempTriangle->getVertex(1);
				GVertex v2 = tempTriangle->getVertex(2);

				GPoint3 collisionPoint;
				if (intersect_line_triangle(collisionPoint, tempRay, v0.getPosition(), v1.getPosition(), v2.getPosition()))
				{
					if (far > abs(norm(collisionPoint - tempRay.GetPt())))
					{
						// set color of triangle
						// Interpolation using barycentry coords
						GVector3 abc = barycentric(v0.getPosition(), v1.getPosition(), v2.getPosition(), collisionPoint);
						vObject[q]->setAmbient(abc[0] * v0.getAmbient() + abc[1] * v1.getAmbient() + abc[2] * v2.getAmbient());
						vObject[q]->setDiffuse(abc[0] * v0.getDiffuse() + abc[1] * v1.getDiffuse() + abc[2] * v2.getDiffuse());
						vObject[q]->setSpecular(abc[0] * v0.getSpecular() + abc[1] * v1.getSpecular() + abc[2] * v2.getSpecular());
						vObject[q]->setShiness(abc[0] * v0.getShiness() + abc[1] * v1.getShiness() + abc[2] * v2.getShiness());
						vObject[q]->setReflection(abc[0] * v0.getReflection() + abc[1] * v1.getReflection() + abc[2] * v2.getReflection());
						vObject[q]->setRefraction(abc[0] * v0.getRefraction() + abc[1] * v1.getRefraction() + abc[2] * v2.getRefraction());
						vObject[q]->setIdxr(abc[0] * v0.getIdxr() + abc[1] * v1.getIdxr() + abc[2] * v2.getIdxr());

						far = abs(norm(collisionPoint - tempRay.GetPt()));
						tempResult.obj = q;
						tempResult.collisionPoint = collisionPoint;
						tempResult.surfaceNormal = tempTriangle->getNormal();
						//tempResult.surfaceNormal = abc[0] * v0.getNormal() + abc[1] * v1.getNormal() + abc[2] * v2.getNormal();
						tempResult.surfaceNormal.Normalize();
						tempResult.idxr = tempTriangle->getIdxr();
					}
				}
			}
		}
	}
	return tempResult;
}

GColor GRayTracer::shade(IntersectVertex& tempResult, GLine& tempRay, int level)
{
	GColor total(0, 0, 0);

	if (tempRay.outside)
	{
		for (unsigned int i = 0; i < vLight.size(); ++i)	// applying every light
		{
			total += shadowRay(tempResult, tempRay, i) * localIllumination(tempResult, tempRay, i);
		}
	}

	total += vObject[tempResult.obj]->getReflection() * trace(reflectionRay(tempResult, tempRay), level + 1);

	total += vObject[tempResult.obj]->getRefraction() * trace(refractionRay(tempResult, tempRay), level + 1);

	return total;
}

GColor GRayTracer::localIllumination(IntersectVertex& tempResult, GLine& tempRay, int RightSource)
{
	GLight* tempLight = vLight[RightSource];
	GVector3 viewVec = -tempRay.GetDir();
	viewVec.Normalize();
	double attenu = 1.0;
	double attenuation[3] = { 0.5, 0.5, 0.5 };	// distance

	GColor total(0, 0, 0);
	GVector3 lightVec;
	if (vLight[RightSource]->type == GLight::POINT)
	{
		GPointLight* pLight = (GPointLight*)tempLight;

		lightVec = pLight->getPosition() - tempResult.collisionPoint;
		double distance = abs(norm(lightVec));
		//attenu = 1.0 / (attenuation[0] + distance * attenuation[1] + distance * distance * attenuation[2]);
	}

	GVector3 perpendicular = lightVec - proj(lightVec, tempResult.surfaceNormal);
	GVector3 reflectVec = lightVec - 2 * perpendicular;
	reflectVec.Normalize();
	lightVec.Normalize();
	double specF = 1;

	if (lightVec * tempResult.surfaceNormal <= 1.0e-5)
		specF = 0;

	GColor ambient = vObject[tempResult.obj]->getAmbient() * tempLight->getAmbient();
	GColor diffuse = MAX((tempResult.surfaceNormal * lightVec), 0) * vObject[tempResult.obj]->getDiffuse() * tempLight->getDiffuse();
	GColor specular = specF * pow(MAX(viewVec * reflectVec, 0), vObject[tempResult.obj]->getShiness()) * tempLight->getSpecular() * vObject[tempResult.obj]->getSpecular();

	total += (attenu * (ambient + diffuse + specular));

	return total;
}

double GRayTracer::shadowRay(IntersectVertex& tempResult, GLine& tempRay, int RightSource)
{
	if (vLight[RightSource]->type == GLight::POINT)
	{
		GPointLight* pLight = (GPointLight*)vLight[RightSource];
		GVector3 shadowRaydir = pLight->getPosition() - tempResult.collisionPoint;
		double coll_to_Light_dis = norm(shadowRaydir);
		GLine shadowLine(tempResult.collisionPoint, shadowRaydir.Normalize(), tempRay.outside, tempRay.idxr);
		IntersectVertex shadowResult = closestIntersection(shadowLine);

		if (shadowResult.obj != -1)	// collision
		{
			GVector3 coll_to_obj = shadowResult.collisionPoint - tempResult.collisionPoint;

			if (norm(coll_to_obj) < coll_to_Light_dis)
			{
				return shadowRate;
			}
		}
		return 1.0;
	}
	return 0.0;		// black
}

GLine GRayTracer::reflectionRay(IntersectVertex& tempResult, GLine& tempRay)
{
	GVector3 perpendicular = -tempRay.GetDir() - proj(-tempRay.GetDir(), tempResult.surfaceNormal);
	GVector3 reflectVec = -tempRay.GetDir() - 2 * perpendicular;
	reflectVec.Normalize();

	return GLine(tempResult.collisionPoint, reflectVec, tempRay.outside, tempRay.idxr);
}

GLine GRayTracer::refractionRay(IntersectVertex& tempResult, GLine& tempRay)
{
	double n1n2 = tempRay.idxr / tempResult.idxr;
	double cos1 = tempResult.surfaceNormal * (-tempRay.GetDir());
	double cos2 = sqrt(1.0 - SQR(n1n2)*(1.0 - cos1*cos1));
	GVector3 refractVec = n1n2*tempRay.GetDir() + (n1n2*cos1 - cos2) * tempResult.surfaceNormal;

	if (vObject[tempResult.obj]->type == GObject::TRIANGLE)
	{
		tempRay.outside = !tempRay.outside;
	}

	return GLine(tempResult.collisionPoint, refractVec.Normalize(), !tempRay.outside, tempResult.idxr);
}

GColor GRayTracer::singlePixelTrace(float x, float y)
{
	GVector3 rayDir = cast_vec3(camera * cast_vec(GVector3(x, y, 1)));
	GPoint3 rayOrigin = cameraPos;
	GLine tempRay(rayOrigin, rayDir.Normalize(), true, false);

	return trace(tempRay, 0);
}

void GRayTracer::setMaxLevel(int _level)
{
	maxLevel = _level;
}
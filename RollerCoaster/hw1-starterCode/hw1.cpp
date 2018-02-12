/*
CSCI 420 Computer Graphics, USC
Assignment 2: Roller Coaster
C++ starter code

Student username: changkil@usc.edu
*/
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "openGLHeader.h"
#include "glutHeader.h"
#include "imageIO.h"
#include "openGLMatrix.h"
#include "advancePipelineProgram.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "glew32d.lib")
#else
#pragma comment(lib, "glew32.lib")
#endif
#endif

#ifdef WIN32
char shaderBasePath[1024] = SHADER_BASE_PATH;
#else
char shaderBasePath[1024] = "../openGLHelper-starterCode";
#endif

using namespace std;
int mousePos[2]; // x,y coordinate of the mouse position

int leftMouseButton = 0; // 1 if pressed, 0 if not 
int middleMouseButton = 0; // 1 if pressed, 0 if not
int rightMouseButton = 0; // 1 if pressed, 0 if not

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROL_STATE;
CONTROL_STATE controlState = ROTATE;

// state of the world
float landRotate[3] = { 0.0f, 0.0f, 0.0f };
float landTranslate[3] = { 0.0f, 0.0f, 0.0f };
float landScale[3] = { 1.0f, 1.0f, 1.0f };
int windowWidth = 1280;
int windowHeight = 720;
char windowTitle[128] = "CSCI 420 homework 2";

// represents one control point along the spline 
struct Point
{
	double x;
	double y;
	double z;

	Point operator*(float scaler)
	{
		Point p = { x * scaler, y * scaler, z * scaler };
		return p;
	}
	Point operator+(Point &q)
	{
		Point p = { x + q.x, y + q.y, z + q.z };
		return p;
	}
	Point operator-(Point &q)
	{
		Point p = { x - q.x, y - q.y, z - q.z };
		return p;
	}
	Point operator-()
	{
		Point p = { -x, -y, -z };
		return p;
	}
};

struct UV
{
	float u;
	float v;
};
// spline struct 
// contains how many control points the spline has, and an array of control points 
struct Spline
{
	int numControlPoints;
	Point *points;
};

// the spline array 
Spline *splines;
vector <Point> splinePoints;
// total number of splines 
int numSplines;

// matrix handler
OpenGLMatrix* matrixHandler;
// pipeline handler
AdvancePipelineProgram* advancePipelineHandler;

// Track
vector <float> trackPos;
vector <float> trackNormal;
vector <float> trackUVs;
GLuint trackVBO, trackVAO, trackTexture;

// Terrain
vector <float> terrainPos;
vector <float> terrainNormal;
vector <float> terrainUVs;
GLuint terrainVBO, terrainVAO, terrainTexture;

// Skybox
vector <float> skyboxPos;
vector <float> skyboxNormal;
vector <float> skyboxUVs;
GLuint skyboxVBO, skyboxVAO, skyboxTexture;

// Camera
float cameraZ = 3.0f;
float cameraVelocity = 0.0f;
float maxHeight = 0.0;
int cameraStep = 0;
Point lastBinormal = { 0, 0, 1 };

// Screenshot
int screenShotCount = 0;
const int MAX_SCREENSHOTS = 1000;
bool isRecording = false;

// write a screenshot to the specified filename
void saveScreenshot(const char * filename)
{
	unsigned char * screenshotData = new unsigned char[windowWidth * windowHeight * 3];
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, screenshotData);

	ImageIO screenshotImg(windowWidth, windowHeight, 3, screenshotData);

	if (screenshotImg.save(filename, ImageIO::FORMAT_JPEG) == ImageIO::OK)
		cout << "File " << filename << " saved successfully." << endl;
	else cout << "Failed to save file " << filename << '.' << endl;

	delete[] screenshotData;
}

int loadSplines(char * argv)
{
	char * cName = (char *)malloc(128 * sizeof(char));
	FILE * fileList;
	FILE * fileSpline;
	int iType, i = 0, j, iLength;

	// load the track file 
	fileList = fopen(argv, "r");
	if (fileList == NULL)
	{
		printf("can't open file\n");
		exit(1);
	}

	// stores the number of splines in a global variable 
	fscanf(fileList, "%d", &numSplines);

	splines = (Spline*)malloc(numSplines * sizeof(Spline));

	// reads through the spline files 
	for (j = 0; j < numSplines; j++)
	{
		i = 0;
		fscanf(fileList, "%s", cName);
		fileSpline = fopen(cName, "r");

		if (fileSpline == NULL)
		{
			printf("can't open file\n");
			exit(1);
		}

		// gets length for spline file
		fscanf(fileSpline, "%d %d", &iLength, &iType);

		// allocate memory for all the points
		splines[j].points = (Point *)malloc(iLength * sizeof(Point));
		splines[j].numControlPoints = iLength;

		// saves the data to the struct
		while (fscanf(fileSpline, "%lf %lf %lf",
			&splines[j].points[i].x,
			&splines[j].points[i].z,
			&splines[j].points[i].y) != EOF)
		{
			i++;
		}
	}

	free(cName);

	return 0;
}

int initTexture(const char * imageFilename, GLuint textureHandle)
{
	// read the texture image
	ImageIO img;
	ImageIO::fileFormatType imgFormat;
	ImageIO::errorType err = img.load(imageFilename, &imgFormat);

	if (err != ImageIO::OK)
	{
		printf("Loading texture from %s failed.\n", imageFilename);
		return -1;
	}

	// check that the number of bytes is a multiple of 4
	if (img.getWidth() * img.getBytesPerPixel() % 4)
	{
		printf("Error (%s): The width*numChannels in the loaded image must be a multiple of 4.\n", imageFilename);
		return -1;
	}

	// allocate space for an array of pixels
	int width = img.getWidth();
	int height = img.getHeight();
	unsigned char * pixelsRGBA = new unsigned char[4 * width * height]; // we will use 4 bytes per pixel, i.e., RGBA

																		// fill the pixelsRGBA array with the image pixels
	memset(pixelsRGBA, 0, 4 * width * height); // set all bytes to 0
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++)
		{
			// assign some default byte values (for the case where img.getBytesPerPixel() < 4)
			pixelsRGBA[4 * (h * width + w) + 0] = 0; // red
			pixelsRGBA[4 * (h * width + w) + 1] = 0; // green
			pixelsRGBA[4 * (h * width + w) + 2] = 0; // blue
			pixelsRGBA[4 * (h * width + w) + 3] = 255; // alpha channel; fully opaque

													   // set the RGBA channels, based on the loaded image
			int numChannels = img.getBytesPerPixel();
			for (int c = 0; c < numChannels; c++) // only set as many channels as are available in the loaded image; the rest get the default value
				pixelsRGBA[4 * (h * width + w) + c] = img.getPixel(w, h, c);
		}
	}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// initialize the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsRGBA);

	// generate the mipmaps for this texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// query support for anisotropic texture filtering
	float fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	printf("Max available anisotropic samples: %f\n", fLargest);
	// set anisotropic texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.5f * fLargest);

	// query for any errors
	GLenum errCode = glGetError();
	if (errCode != 0)
	{
		printf("Texture initialization error. Error code: %d.\n", errCode);
		return -1;
	}

	// de-allocate the pixel array -- it is no longer needed
	delete[] pixelsRGBA;

	return 0;
}
//rv = m*v	3x3 matrix
void matrixMultiply(float* resM, const float* m, const float* v)
{
	for (int i = 0; i < 3; i++)
	{
		resM[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			resM[i] += m[4 * i + j] * v[j];
		}
	}
}

void bindProjectionMatrix()
{
	// upload the projection matrix to GPU
	float p[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::Projection);
	matrixHandler->GetMatrix(p);
	advancePipelineHandler->SetProjectionMatrix(p);
}

void bindModelViewMatrix()
{
	// upload the mdoelview matrix to GPU
	float m[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->GetMatrix(m);
	advancePipelineHandler->SetModelViewMatrix(m);
}

void bindNormalMatrix()
{
	// normal
	float n[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->GetNormalMatrix(n);	// get normal matrix
	advancePipelineHandler->SetNormalMatrix(n);
}

void bindLightProperties()
{
	float m[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->GetMatrix(m);

	// light direction(view)
	float lightDirection[3] = { 0.0f, 1.0f, 0.0f }; // the “Sun” at noon
	float viewLightDirection[3]; // light direction in the view space
	matrixMultiply(viewLightDirection, m, lightDirection);
	advancePipelineHandler->SetViewLightDirection(viewLightDirection);

	// light properties(white)
	float lightAmbient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	advancePipelineHandler->SetLightAmbient(lightAmbient);
	advancePipelineHandler->SetLightDiffuse(lightDiffuse);
	advancePipelineHandler->SetLightSpecular(lightSpecular);
}
void bindMaterialProperties()
{
	// material properties
	float matKa[4] = { 0.6f, 0.6f, 0.2f, 1.0f };	// 
	float matKd[4] = { 0.6f, 0.6f, 0.2f, 1.0f };	// 
	float matKs[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// white
	float matKsExp = 20;
	advancePipelineHandler->SetMaterialAmbient(matKa);
	advancePipelineHandler->SetMaterialDiffuse(matKd);
	advancePipelineHandler->SetMaterialSpecular(matKs);
	advancePipelineHandler->SetMaterialExponent(matKsExp);
}

void bindTexture()
{
	// texture
	// select the active texture unit
	advancePipelineHandler->SetTextureUnit(GL_TEXTURE0);
}

void bindAdvanceProgram()
{
	bindProjectionMatrix();
	bindModelViewMatrix();
	bindNormalMatrix();
	bindLightProperties();
	bindMaterialProperties();

	// bind the pipeline
	advancePipelineHandler->Bind();
}

void renderObject(GLenum mode, GLuint &vao, GLsizei numVertex, GLuint &texture)
{
	// select the texture to use
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawArrays(mode, 0, numVertex / 3);	// vertex has x, y, z
	glBindVertexArray(0);	// unbind the VAO
}

void printPoint(const Point &p)
{
	cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << endl;
}

void setupCamera()
{
	// set a camera
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->LoadIdentity();
	// camera z-position
	matrixHandler->LookAt(0, 0, cameraZ, 0, 0, -1, 0, 1, 0);
}

float distance(const Point& p, const Point& q)
{
	float x2 = (p.x - q.x)*(p.x - q.x);
	float y2 = (p.y - q.y)*(p.y - q.y);
	float z2 = (p.z - q.z)*(p.z - q.z);
	return sqrt(x2 + y2 + z2);
}

float pointLength(const Point &p)
{
	return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

void normalize(Point &p)
{
	float len = pointLength(p);
	if (len == 0) {
		cout << "Point size is 0" << endl;
		exit(0);
	}
	p.x /= len;
	p.y /= len;
	p.z /= len;
}

Point crossProduct(const Point &p, const Point &q)
{
	Point res;
	res.x = p.y * q.z - p.z * q.y;
	res.y = p.z * q.x - p.x * q.z;
	res.z = p.x * q.y - p.y * q.x;
	return res;
}

float computeDifferentialCRSEquation(float x0, float x1, float x2, float x3, float u)
{
	const float s = 0.5;
	float res = s * ((-x0 + x2)
		+ (2 * x0 - 5 * x1 + 4 * x2 - x3) * (2 * u)
		+ (-x0 + 3 * x1 - 3 * x2 + x3) * (3 * u*u));
	return res;
}

float computeVelocityDelta(Point &cp, Point &dp)
{
	float speed = 0.001f;
	float gravity = 9.8f;
	float numerator = speed * sqrt(2 * gravity * (maxHeight - cp.y));
	float denominator = pointLength(dp);
	return numerator / denominator;
}

Point computeTangentVector(Point &cp0, Point &cp1, Point &cp2, Point &cp3, float u)
{
	Point unitTanVec;
	unitTanVec.x = computeDifferentialCRSEquation(cp0.x, cp1.x, cp2.x, cp3.x, u);
	unitTanVec.y = computeDifferentialCRSEquation(cp0.y, cp1.y, cp2.y, cp3.y, u);
	unitTanVec.z = computeDifferentialCRSEquation(cp0.z, cp1.z, cp2.z, cp3.z, u);
	normalize(unitTanVec);

	return unitTanVec;
}

Point computeUpVector(Point &tanVec)
{
	// choose a binormal vector as an up-vector
	Point normalVec = crossProduct(lastBinormal, tanVec);
	normalize(normalVec);

	Point binormalVec = crossProduct(tanVec, normalVec);
	normalize(binormalVec);
	lastBinormal = binormalVec;	// update binormal vector

	return binormalVec;
}

void computeCameraPos(Point &cp0, Point &cp1, Point &cp2, Point &cp3)
{
	Point tanVec = computeTangentVector(cp0, cp1, cp2, cp3, cameraVelocity);
	Point binormalVec = computeUpVector(tanVec);
	Point normalVec = crossProduct(binormalVec, tanVec);
	cameraVelocity = computeVelocityDelta(cp0, tanVec);
	// setup camera
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->LoadIdentity();
	matrixHandler->LookAt(cp0.x, cp0.y, cp0.z,
		cp0.x + tanVec.x, cp0.y + tanVec.y, cp0.z + tanVec.z,
		normalVec.x, normalVec.y, normalVec.z);
}

void setCameraPos(int idx)
{
	if (idx > splinePoints.size() - 4)
	{
		cout << "CameraStep is out of the range" << endl;
		exit(0);
	}

	Point cp0, cp1, cp2, cp3;
	cp0 = splinePoints.at(idx + 0);
	cp1 = splinePoints.at(idx + 1);
	cp2 = splinePoints.at(idx + 2);
	cp3 = splinePoints.at(idx + 3);

	computeCameraPos(cp0, cp1, cp2, cp3);
}

void resetCameraPos()
{
	cameraStep = 0;
	cameraVelocity = 0.0f;
	lastBinormal = { 0, 0, 1 };
	setCameraPos(0);
}

void cameraTransformations()
{
	matrixHandler->Translate(landTranslate[0], landTranslate[1], landTranslate[2]);
	matrixHandler->Rotate(landRotate[0], 1.0f, 0.0f, 0.0f);
	matrixHandler->Rotate(landRotate[1], 0.0f, 1.0f, 0.0f);
	matrixHandler->Rotate(landRotate[2], 0.0f, 0.0f, 1.0f);
	matrixHandler->Scale(landScale[0], landScale[1], landScale[2]);
}

void displayFunc()
{
	// clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setupCamera();

	// transformations
	//cameraTransformations();

	// setup camera for rollercoaster
	setCameraPos(cameraStep);

	// binding
	bindAdvanceProgram();

	// render objects
	// track
	renderObject(GL_TRIANGLES, trackVAO, trackPos.size(), trackTexture);
	matrixHandler->Translate(0.0, -5.0f, 0.0f);
	bindModelViewMatrix();

	// terrain : triangle
	renderObject(GL_TRIANGLES, terrainVAO, terrainPos.size(), terrainTexture);
	// skybox : triangle
	renderObject(GL_TRIANGLE_STRIP, skyboxVAO, skyboxPos.size(), skyboxTexture);

	glutSwapBuffers();
}

void idleFunc()
{
	// TODO: comment all
	if (isRecording)
	{
		if (screenShotCount < MAX_SCREENSHOTS)
		{
			stringstream stringStream;
			stringStream << "../screenshots/" << screenShotCount << ".jpg";
			string name = stringStream.str();
			saveScreenshot(name.c_str());
			screenShotCount++;
		}
		else
		{
			//screenShotCount = 0;
		}
	}

	// camera movement
	if (cameraStep < splinePoints.size() - 4)
	{
		cameraStep++;
		//cout << cameraStep << endl;
	}

	// make the screen update 
	glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
	glViewport(0, 0, w, h);

	// setup perspective matrix
	float aspect = (float)w / (float)h;
	matrixHandler->SetMatrixMode(OpenGLMatrix::Projection);
	matrixHandler->LoadIdentity();
	matrixHandler->Perspective(45.0f, aspect, 0.01f, 1000.0f);
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
}

void mouseMotionDragFunc(int x, int y)
{
	// mouse has moved and one of the mouse buttons is pressed (dragging)

	// the change in mouse position since the last invocation of this function
	int mousePosDelta[2] = { x - mousePos[0], y - mousePos[1] };

	switch (controlState)
	{
		// translate the landscape
	case TRANSLATE:
		if (leftMouseButton)
		{
			// control x,y translation via the left mouse button
			landTranslate[0] += mousePosDelta[0] * 0.01f;
			landTranslate[1] -= mousePosDelta[1] * 0.01f;
		}
		if (middleMouseButton)
		{
			// control z translation via the middle mouse button
			landTranslate[2] += mousePosDelta[1] * 0.01f;
		}
		break;

		// rotate the landscape
	case ROTATE:
		if (leftMouseButton)
		{
			// control x,y rotation via the left mouse button
			landRotate[0] += mousePosDelta[1];
			landRotate[1] += mousePosDelta[0];
		}
		if (middleMouseButton)
		{
			// control z rotation via the middle mouse button
			landRotate[2] += mousePosDelta[1];
		}
		break;

		// scale the landscape
	case SCALE:
		if (leftMouseButton)
		{
			// control x,y scaling via the left mouse button
			landScale[0] *= 1.0f + mousePosDelta[0] * 0.01f;
			landScale[1] *= 1.0f - mousePosDelta[1] * 0.01f;
		}
		if (middleMouseButton)
		{
			// control z scaling via the middle mouse button
			landScale[2] *= 1.0f - mousePosDelta[1] * 0.01f;
		}
		break;
	}

	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void mouseMotionFunc(int x, int y)
{
	// mouse has moved
	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void mouseButtonFunc(int button, int state, int x, int y)
{
	// a mouse button has has been pressed or depressed

	// keep track of the mouse button state, in leftMouseButton, middleMouseButton, rightMouseButton variables
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		leftMouseButton = (state == GLUT_DOWN);
		break;

	case GLUT_MIDDLE_BUTTON:
		middleMouseButton = (state == GLUT_DOWN);
		break;

	case GLUT_RIGHT_BUTTON:
		rightMouseButton = (state == GLUT_DOWN);
		break;
	}

	// keep track of whether CTRL and SHIFT keys are pressed
	switch (glutGetModifiers())
	{
	case GLUT_ACTIVE_CTRL:
		controlState = TRANSLATE;
		break;

	case GLUT_ACTIVE_SHIFT:
		controlState = SCALE;
		break;

		// if CTRL and SHIFT are not pressed, we are in rotate mode
	default:
		controlState = ROTATE;
		break;
	}

	// store the new mouse position
	mousePos[0] = x;
	mousePos[1] = y;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // ESC key
		exit(0); // exit the program
		break;
	case ' ':
		cout << "You pressed the spacebar." << endl;
		cout << "Recording..." << endl;
		isRecording = !isRecording;
		break;
	case 'x':
		// take a screenshot
		saveScreenshot("../screenshots/1000.jpg");
		break;
	case 'r':
		// reset the camera
		resetCameraPos();
		break;
	//case 'a':
	//	cameraStep++;
	//	break;
	//case 's':
	//	cameraStep--;
	//	break;
	}
}

void initHandler()
{
	// initialize openGLMatrix
	matrixHandler = new OpenGLMatrix();
	// initialize shader pipeline program
	advancePipelineHandler = new AdvancePipelineProgram();
	advancePipelineHandler->Init("../openGLHelper-starterCode");
}

void initVBO(GLuint& vbo, vector <float> &pos, vector <float> &normal, vector <float> &uvs)
{
	// reserve a name for the buffer object
	glGenBuffers(1, &vbo);
	// bind it to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Allocate space for it
	glBufferData(GL_ARRAY_BUFFER, (pos.size() + normal.size() + uvs.size()) * sizeof(float), NULL, GL_STATIC_DRAW);
	// upload position data
	glBufferSubData(GL_ARRAY_BUFFER, 0, pos.size() * sizeof(float), pos.data());
	// upload normal data
	glBufferSubData(GL_ARRAY_BUFFER, pos.size() * sizeof(float), normal.size() * sizeof(float), normal.data());
	// upload uv data
	glBufferSubData(GL_ARRAY_BUFFER, (pos.size() + normal.size()) * sizeof(float), uvs.size() * sizeof(float), uvs.data());
}

void initVAO(GLuint& vao, int posSize, int normSize)
{
	// generate and bind the vertex arrays
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// get a handle to the program
	GLuint program = advancePipelineHandler->GetProgramHandle();
	// position
	GLuint loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(loc);
	const void* offset = (const void*)0;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, offset);

	// normal
	GLuint loc2 = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(loc2);
	const void* offset2 = (const void*)(posSize * sizeof(float));
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, offset2);

	// uvs
	GLuint loc3 = glGetAttribLocation(program, "texCoord");
	glEnableVertexAttribArray(loc3);	// enable the normal attribute
	const void* offset3 = (const void*)((posSize + normSize) * sizeof(float));
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, offset3);

	glBindVertexArray(0);	// unbind the VAO
}

void insertPoint(vector <float> &pos, Point &p)
{
	pos.push_back(p.x);
	pos.push_back(p.y);
	pos.push_back(p.z);
}

void createTriangle(vector <float> &pos, Point &p1, Point &p2, Point &p3)
{
	// Insert points into the vector array
	insertPoint(pos, p1);
	insertPoint(pos, p2);
	insertPoint(pos, p3);
}

void insertUV(vector <float> &uv, UV &tc)
{
	uv.push_back(tc.u);
	uv.push_back(tc.v);
}

void createUV(vector <float> &uv, UV &tc1, UV &tc2, UV &tc3)
{
	// Insert UVs into the vector array
	insertUV(uv, tc1);
	insertUV(uv, tc2);
	insertUV(uv, tc3);
}

void createTexture(const char* file, GLuint &texHandle)
{
	// create an integer handle for the texture
	glGenTextures(1, &texHandle);
	int code = initTexture(file, texHandle);
	if (code != 0)
	{
		printf("Error loading the texture image.\n");
		exit(EXIT_FAILURE);
	}
}
// compute catmull-rom spline equation
float computeCRSEquation(float x0, float x1, float x2, float x3, float u)
{
	const float s = 0.5;
	float res = s * ((2 * x1) + (-x0 + x2) * u
		+ (2 * x0 - 5 * x1 + 4 * x2 - x3) * (u*u)
		+ (-x0 + 3 * x1 - 3 * x2 + x3) * (u*u*u));
	return res;
}

// compute catmull-rom spline
Point computeCRS(Point &cp0, Point &cp1, Point &cp2, Point &cp3, float u)
{
	Point resP;
	resP.x = computeCRSEquation(cp0.x, cp1.x, cp2.x, cp3.x, u);
	resP.y = computeCRSEquation(cp0.y, cp1.y, cp2.y, cp3.y, u);
	resP.z = computeCRSEquation(cp0.z, cp1.z, cp2.z, cp3.z, u);

	return resP;
}

void drawLine(Point &p0, Point &p1)
{
	// draw a line
	splinePoints.push_back(p0);
	splinePoints.push_back(p1);

	// find max height
	if (p0.y > maxHeight)
		maxHeight = p0.y;
	if (p1.y > maxHeight)
		maxHeight = p1.y;
}

void subDivide(Point &cp0, Point &cp1, Point &cp2, Point &cp3, float u0, float u1, float maxlineLength)
{
	float umid = (u0 + u1) / 2.0f;
	Point p0 = computeCRS(cp0, cp1, cp2, cp3, u0);
	Point p1 = computeCRS(cp0, cp1, cp2, cp3, u1);
	if (distance(p0, p1) > maxlineLength)
	{
		subDivide(cp0, cp1, cp2, cp3, u0, umid, maxlineLength);
		subDivide(cp0, cp1, cp2, cp3, umid, u1, maxlineLength);
	}
	else
	{
		drawLine(p0, p1);
	}
}

void createSpline(float interval = 0.1f)
{
	for (int i = 0; i < numSplines; i++)
	{
		for (int j = 0; j < splines[i].numControlPoints - 3; j++)
		{
			// control points : cp0, cp1, cp2, cp3
			Point cp0 = splines[i].points[j];
			Point cp1 = splines[i].points[j + 1];
			Point cp2 = splines[i].points[j + 2];
			Point cp3 = splines[i].points[j + 3];

			subDivide(cp0, cp1, cp2, cp3, 0.0f, 1.0f, interval);
		}
	}
}

void initTrackUVs(float uvSize = 1.0)
{
	// Create corners
	UV topLeft = { 0.0f, uvSize };
	UV topRight = { uvSize, uvSize };
	UV bottomLeft = { 0.0f, 0.0f };
	UV bottomRight = { uvSize, 0.0f };

	for (int i = 0; i < 6; i++)
	{
		// Create UVs : clock wise
		createUV(trackUVs, topLeft, topRight, bottomLeft);
		createUV(trackUVs, bottomLeft, topRight, bottomRight);
	}
}

void initTrackRail(Point& p0, Point& p1, Point& tanVec, Point& binormalVec, Point& normalVec, float trackSize, bool isLeft)
{
	// Create coners
	Point topLeft;
	Point topRight;
	Point bottomLeft;
	Point bottomRight;
	if (isLeft) {
		topLeft = (-normalVec*1.5 - binormalVec*2) * trackSize;
		topRight = (-normalVec*1.5 - binormalVec) * trackSize;
		bottomLeft = (-normalVec*2 - binormalVec*2) * trackSize;
		bottomRight = (-normalVec*2 - binormalVec) * trackSize;
	}
	else {
		topLeft = (-normalVec*1.5 + binormalVec) * trackSize;
		topRight = (-normalVec*1.5 + binormalVec*2) * trackSize;
		bottomLeft = (-normalVec*2 + binormalVec) * trackSize;
		bottomRight = (-normalVec*2 + binormalVec*2) * trackSize;
	}

	// Create triangles : clock wise
	// front
	createTriangle(trackPos, p0 + topLeft, p0 + topRight, p0 + bottomLeft);
	createTriangle(trackPos, p0 + bottomLeft, p0 + topRight, p0 + bottomRight);
	// back
	createTriangle(trackPos, p1 + topLeft, p1 + topRight, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p1 + topRight, p1 + bottomRight);
	// left
	createTriangle(trackPos, p1 + topLeft, p0 + topLeft, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p0 + topLeft, p0 + bottomLeft);
	// right
	createTriangle(trackPos, p0 + topRight, p1 + topRight, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + topRight, p1 + bottomRight);
	// top
	createTriangle(trackPos, p0 + topLeft, p1 + topLeft, p0 + topRight);
	createTriangle(trackPos, p0 + topRight, p1 + topLeft, p1 + topRight);
	// bottom
	createTriangle(trackPos, p0 + bottomLeft, p1 + bottomLeft, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + bottomLeft, p1 + bottomRight);

	// normal
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, normalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -normalVec);

	// uvs
	initTrackUVs(1.0f);
}

void initTrackPillar(Point& p0, Point& tanVec, Point& binormalVec, Point& normalVec, float pillarSize, bool isLeft)
{
	// Create coners
	Point topLeft;
	Point topRight;
	Point bottomLeft;
	Point bottomRight;

	if (isLeft)
	{
		topLeft = (- binormalVec*2) * pillarSize;
		topRight = (- binormalVec) * pillarSize;
		bottomLeft = (- binormalVec*2) * pillarSize;
		bottomRight = (- binormalVec) * pillarSize;
		bottomLeft.y -= (p0.y + 5);
		bottomRight.y -= (p0.y + 5);
	}
	else
	{
		topLeft = (binormalVec) * pillarSize;
		topRight = (binormalVec*2) * pillarSize;
		bottomLeft = (binormalVec) * pillarSize;
		bottomRight = (binormalVec*2) * pillarSize;
		bottomLeft.y -= (p0.y + 5);
		bottomRight.y -= (p0.y + 5);
	}

	Point p1 = p0 + tanVec * pillarSize;
	// Create triangles : clock wise
	// front
	createTriangle(trackPos, p0 + topLeft, p0 + topRight, p0 + bottomLeft);
	createTriangle(trackPos, p0 + bottomLeft, p0 + topRight, p0 + bottomRight);
	// back
	createTriangle(trackPos, p1 + topLeft, p1 + topRight, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p1 + topRight, p1 + bottomRight);
	// left
	createTriangle(trackPos, p1 + topLeft, p0 + topLeft, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p0 + topLeft, p0 + bottomLeft);
	// right
	createTriangle(trackPos, p0 + topRight, p1 + topRight, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + topRight, p1 + bottomRight);
	// top
	createTriangle(trackPos, p0 + topLeft, p1 + topLeft, p0 + topRight);
	createTriangle(trackPos, p0 + topRight, p1 + topLeft, p1 + topRight);
	// bottom
	createTriangle(trackPos, p0 + bottomLeft, p1 + bottomLeft, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + bottomLeft, p1 + bottomRight);

	// normal
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, normalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -normalVec);

	// uvs
	initTrackUVs(1.0f);
}

void initTrackCross(Point& p0, Point& tanVec, Point& binormalVec, Point& normalVec, float crossSize)
{
	// Create coners
	Point topLeft = (- binormalVec - normalVec*1.5) * crossSize;
	Point topRight = (binormalVec - normalVec*1.5) * crossSize;
	Point bottomLeft = (-binormalVec - normalVec * 2) * crossSize;
	Point bottomRight = (binormalVec - normalVec * 2) * crossSize;

	Point p1 = p0 + tanVec * crossSize * 3;

	// Create triangles : clock wise
	// front
	createTriangle(trackPos, p0 + topLeft, p0 + topRight, p0 + bottomLeft);
	createTriangle(trackPos, p0 + bottomLeft, p0 + topRight, p0 + bottomRight);
	// back
	createTriangle(trackPos, p1 + topLeft, p1 + topRight, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p1 + topRight, p1 + bottomRight);
	// left
	createTriangle(trackPos, p1 + topLeft, p0 + topLeft, p1 + bottomLeft);
	createTriangle(trackPos, p1 + bottomLeft, p0 + topLeft, p0 + bottomLeft);
	// right
	createTriangle(trackPos, p0 + topRight, p1 + topRight, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + topRight, p1 + bottomRight);
	// top
	createTriangle(trackPos, p0 + topLeft, p1 + topLeft, p0 + topRight);
	createTriangle(trackPos, p0 + topRight, p1 + topLeft, p1 + topRight);
	// bottom
	createTriangle(trackPos, p0 + bottomLeft, p1 + bottomLeft, p0 + bottomRight);
	createTriangle(trackPos, p0 + bottomRight, p1 + bottomLeft, p1 + bottomRight);

	// normal
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, tanVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, binormalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, normalVec);
	for (int j = 0; j < 6; j++)
		insertPoint(trackNormal, -normalVec);

	// uvs
	initTrackUVs(1.0f);
}

void initTrack()
{
	// transfer vertex value x, y, z to track position from spline points
	for (int i = 0; i < splinePoints.size() - 3; i++)
	{
		Point p0 = splinePoints.at(i);
		Point p1 = splinePoints.at(i + 1);
		Point p2 = splinePoints.at(i + 2);
		Point p3 = splinePoints.at(i + 3);
		
		// compute tangent vector, binormal vector, normal vector
		Point tanVec = computeTangentVector(p0, p1, p2, p3, 0.01f);
		Point binormalVec = computeUpVector(tanVec);
		Point normalVec = crossProduct(binormalVec, tanVec);

		const float trackSize = 0.01f;
		initTrackRail(p0, p1, tanVec, binormalVec, normalVec, trackSize, true);
		initTrackRail(p0, p1, tanVec, binormalVec, normalVec, trackSize, false);
		if (i % 8 == 0)
		{
			initTrackCross(p0, tanVec, binormalVec, normalVec, trackSize);
		}
		if (i % (splines->numControlPoints * 2) == 0)
		{
			initTrackPillar(p0, tanVec, binormalVec, normalVec, trackSize, true);
			initTrackPillar(p0, tanVec, binormalVec, normalVec, trackSize, false);
		}
	}
}

void initTerrainPos(float terrainWidth = 256.0f, float terrainHeight = 256.0f)
{
	// Create coners
	Point topLeft = { -terrainWidth / 2.0, 0.0, terrainHeight / 2.0 };
	Point topRight = { terrainWidth / 2.0, 0.0, terrainHeight / 2.0 };
	Point bottomLeft = { -terrainWidth / 2.0, 0.0, -terrainHeight / 2.0 };
	Point bottomRight = { terrainWidth / 2.0, 0.0, -terrainHeight / 2.0 };

	// Create triangles : clock wise
	createTriangle(terrainPos, topLeft, topRight, bottomLeft);
	createTriangle(terrainPos, bottomLeft, topRight, bottomRight);
}

void initTerrainNormal()
{
	for (int i = 0; i < terrainPos.size() / 3; i++)
	{
		terrainNormal.push_back(0.0);
		terrainNormal.push_back(1.0);
		terrainNormal.push_back(0.0);
	}
}

void initTerrainUVs(float uvSize = 1.0)
{
	// Create corners
	UV topLeft = { 0.0f, uvSize };
	UV topRight = { uvSize, uvSize };
	UV bottomLeft = { 0.0f, 0.0f };
	UV bottomRight = { uvSize, 0.0f };

	// Create UVs : clock wise
	createUV(terrainUVs, topLeft, topRight, bottomLeft);
	createUV(terrainUVs, bottomLeft, topRight, bottomRight);
}

void initSkybox(float x, float y, float z, float radius, float interval = 0.1f)
{
	/*
	a center of the sphere : x, y, z
	draw a hemisphere
	*/

	// draw hemisphere
	for (float angleY = 0.0f; angleY <= M_PI_2; angleY += (M_PI_2 * interval))	// draw a half of the sphere(PI/2)
	{
		y = radius * sin(angleY);     // y-axis
		float nextY = angleY + (M_PI_2 * interval);  // next y
		for (float angleX = 0.0f; angleX <= (2.0f*M_PI); angleX += (M_PI * interval))	// draw a circle
		{
			x = radius*cos(angleY)*sin(angleX);
			z = radius*cos(angleY)*cos(angleX);

			// cut the triangle strip
			if (angleX == 0.0f && angleY != 0.0f)
			{
				// position
				skyboxPos.push_back(x);
				skyboxPos.push_back(y);
				skyboxPos.push_back(z);
				// normal : directions is the inner of the hemisphere
				skyboxNormal.push_back(-cos(angleY)*sin(angleX));
				skyboxNormal.push_back(-sin(angleY));
				skyboxNormal.push_back(-cos(angleY)*cos(angleX));
				// uvs
				skyboxUVs.push_back(0.0f);
				skyboxUVs.push_back(angleY / M_PI_2 + interval);
			}

			// position
			skyboxPos.push_back(x);
			skyboxPos.push_back(y);
			skyboxPos.push_back(z);
			// normal : directions is the inner of the hemisphere
			skyboxNormal.push_back(-cos(angleY)*sin(angleX));
			skyboxNormal.push_back(-sin(angleY));
			skyboxNormal.push_back(-cos(angleY)*cos(angleX));
			// uvs
			skyboxUVs.push_back(angleX / M_PI);
			skyboxUVs.push_back(angleY / M_PI_2);

			x = radius*cos(nextY)*sin(angleX);
			z = radius*cos(nextY)*cos(angleX);
			// next position
			skyboxPos.push_back(x);
			skyboxPos.push_back(radius*sin(nextY));
			skyboxPos.push_back(z);
			// next normal
			skyboxNormal.push_back(-cos(nextY)*sin(angleX));
			skyboxNormal.push_back(-sin(nextY));
			skyboxNormal.push_back(-cos(nextY)*cos(angleX));
			// uvs
			skyboxUVs.push_back(angleX / M_PI);
			skyboxUVs.push_back(angleY / M_PI_2 + interval);
		}

		// connect last triangle strip to the first one
		x = radius*cos(angleY)*sin(0.0f);
		z = radius*cos(angleY)*cos(0.0f);
		// position
		skyboxPos.push_back(x);
		skyboxPos.push_back(y);
		skyboxPos.push_back(z);
		// normal : directions is the inner of the hemisphere
		skyboxNormal.push_back(-cos(angleY)*sin(0.0f));
		skyboxNormal.push_back(-sin(angleY));
		skyboxNormal.push_back(-cos(angleY)*cos(0.0f));
		// uvs
		skyboxUVs.push_back(2.0f);
		skyboxUVs.push_back(angleY / M_PI_2);

		x = radius*cos(nextY)*sin(0.0f);
		z = radius*cos(nextY)*cos(0.0f);
		// next position
		skyboxPos.push_back(x);
		skyboxPos.push_back(radius*sin(nextY));
		skyboxPos.push_back(z);
		// next normal
		skyboxNormal.push_back(-cos(nextY)*sin(0.0f));
		skyboxNormal.push_back(-sin(nextY));
		skyboxNormal.push_back(-cos(nextY)*cos(0.0f));
		// uvs
		skyboxUVs.push_back(2.0f);
		skyboxUVs.push_back(angleY / M_PI_2 + interval);

		// cut the triangle strip
		// next position
		skyboxPos.push_back(x);
		skyboxPos.push_back(radius*sin(nextY));
		skyboxPos.push_back(z);
		// next normal
		skyboxNormal.push_back(-cos(nextY)*sin(0.0f));
		skyboxNormal.push_back(-sin(nextY));
		skyboxNormal.push_back(-cos(nextY)*cos(0.0f));
		// uvs
		skyboxUVs.push_back(0.0f);
		skyboxUVs.push_back(angleY / M_PI_2 + interval);
	}
}

void createTrack()
{
	createSpline(0.1f);

	initTrack();
	createTexture("textures/track.jpg", trackTexture);
	initVBO(trackVBO, trackPos, trackNormal, trackUVs);
	initVAO(trackVAO, trackPos.size(), trackNormal.size());
}

void createTerrain()
{
	initTerrainPos(256.0f, 256.0f);
	initTerrainNormal();
	initTerrainUVs(16.0f);
	createTexture("textures/terrain.jpg", terrainTexture);
	initVBO(terrainVBO, terrainPos, terrainNormal, terrainUVs);
	initVAO(terrainVAO, terrainPos.size(), terrainNormal.size());
}

void createSkybox()
{
	initSkybox(0, 0, 0, 128.0f, 0.1f);	// center(x, y, z), radius, interval
	createTexture("textures/sky.jpg", skyboxTexture);
	initVBO(skyboxVBO, skyboxPos, skyboxNormal, skyboxUVs);
	initVAO(skyboxVAO, skyboxPos.size(), skyboxNormal.size());
}

void createMeshes()
{
	createTrack();
	createTerrain();
	createSkybox();
}

void initSpline(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: %s <trackfile>\n", argv[0]);
		exit(0);
	}
	// load the splines from the provided filename
	loadSplines(argv[1]);

	printf("Loaded %d spline(s).\n", numSplines);
	for (int i = 0; i<numSplines; i++)
		printf("Num control points in spline %d: %d.\n", i, splines[i].numControlPoints);
}

void initScene(int argc, char *argv[])
{
	// background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// set properties
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LINE_SMOOTH);

	// initialize the system(handler)
	initHandler();
	// initialize variables
	initSpline(argc, argv);
	createMeshes();
	resetCameraPos();
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "The arguments are incorrect." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Initializing GLUT..." << endl;
	glutInit(&argc, argv);

	cout << "Initializing OpenGL..." << endl;

#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
#else
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
#endif

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(windowTitle);

	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
	cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// tells glut to use a particular display function to redraw 
	glutDisplayFunc(displayFunc);
	// perform animation inside idleFunc
	glutIdleFunc(idleFunc);
	// callback for mouse drags
	glutMotionFunc(mouseMotionDragFunc);
	// callback for idle mouse movement
	glutPassiveMotionFunc(mouseMotionFunc);
	// callback for mouse button changes
	glutMouseFunc(mouseButtonFunc);
	// callback for resizing the window
	glutReshapeFunc(reshapeFunc);
	// callback for pressing the keys on the keyboard
	glutKeyboardFunc(keyboardFunc);

	// init glew
#ifdef __APPLE__
	// nothing is needed on Apple
#else
	// Windows, Linux
	GLint result = glewInit();
	if (result != GLEW_OK)
	{
		cout << "error: " << glewGetErrorString(result) << endl;
		exit(EXIT_FAILURE);
	}
#endif

	// do initialization
	initScene(argc, argv);

	// sink forever into the glut loop
	glutMainLoop();
}
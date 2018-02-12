/*
CSCI 420 Computer Graphics, USC
Assignment 1: Height Fields
C++ starter code

Student username: changkil@usc.edu
*/

#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "glutHeader.h"
#include "imageIO.h"
#include "openGLMatrix.h"
#include "basicPipelineProgram.h"

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
char windowTitle[512] = "CSCI 420 homework I";

ImageIO * heightmapImage;

// matrix handler
OpenGLMatrix* matrixHandler;
// pipeline handler
BasicPipelineProgram* pipelineHandler;
// buffers(VBO, VAO)
GLuint vbo[3], vao[3];
// # of vertex
int numPointVertices = 0;
int numLineVertices = 0;
int numTriangleVertices = 0;
// vertex positions
GLfloat *pointPositions = NULL;
GLfloat *linePositions = NULL;
GLfloat *trianglePositions = NULL;
// vertex colors
GLfloat *pointColors = NULL;
GLfloat *lineColors = NULL;
GLfloat *triangleColors = NULL;
// state of the draw
typedef enum { POINT_MODE, WIREFRAME_MODE, SOLID_MODE } DRAW_STATE;
DRAW_STATE drawState = POINT_MODE;
// animation
bool stop = true;
GLfloat delta = 2.0f;
int nameIdx = 0;
char* name[300] = { "000.jpg", "001.jpg", "002.jpg", "003.jpg", "004.jpg", "005.jpg", "006.jpg", "007.jpg", "008.jpg", "009.jpg",
					"010.jpg", "011.jpg", "012.jpg", "013.jpg", "014.jpg", "015.jpg", "016.jpg", "017.jpg", "018.jpg", "019.jpg", 
					"020.jpg", "021.jpg", "022.jpg", "023.jpg", "024.jpg", "025.jpg", "026.jpg", "027.jpg", "028.jpg", "029.jpg",
					"030.jpg", "031.jpg", "032.jpg", "033.jpg", "034.jpg", "035.jpg", "036.jpg", "037.jpg", "038.jpg", "039.jpg",
					"040.jpg", "041.jpg", "042.jpg", "043.jpg", "044.jpg", "045.jpg", "046.jpg", "047.jpg", "048.jpg", "049.jpg",
					"050.jpg", "051.jpg", "052.jpg", "053.jpg", "054.jpg", "055.jpg", "056.jpg", "057.jpg", "058.jpg", "059.jpg",
					"060.jpg", "061.jpg", "062.jpg", "063.jpg", "064.jpg", "065.jpg", "066.jpg", "067.jpg", "068.jpg", "069.jpg",
					"070.jpg", "071.jpg", "072.jpg", "073.jpg", "074.jpg", "075.jpg", "076.jpg", "077.jpg", "078.jpg", "079.jpg",
					"080.jpg", "081.jpg", "082.jpg", "083.jpg", "084.jpg", "085.jpg", "086.jpg", "087.jpg", "088.jpg", "089.jpg",
					"090.jpg", "091.jpg", "092.jpg", "093.jpg", "094.jpg", "095.jpg", "096.jpg", "097.jpg", "098.jpg", "099.jpg",
					"100.jpg", "101.jpg", "102.jpg", "103.jpg", "104.jpg", "105.jpg", "106.jpg", "107.jpg", "108.jpg", "109.jpg",
					"110.jpg", "111.jpg", "112.jpg", "113.jpg", "114.jpg", "115.jpg", "116.jpg", "117.jpg", "118.jpg", "119.jpg",
					"120.jpg", "121.jpg", "122.jpg", "123.jpg", "124.jpg", "125.jpg", "126.jpg", "127.jpg", "128.jpg", "129.jpg",
					"130.jpg", "131.jpg", "132.jpg", "133.jpg", "134.jpg", "135.jpg", "136.jpg", "137.jpg", "138.jpg", "139.jpg",
					"140.jpg", "141.jpg", "142.jpg", "143.jpg", "144.jpg", "145.jpg", "146.jpg", "147.jpg", "148.jpg", "149.jpg",
					"150.jpg", "151.jpg", "152.jpg", "153.jpg", "154.jpg", "155.jpg", "156.jpg", "157.jpg", "158.jpg", "159.jpg",
					"160.jpg", "161.jpg", "162.jpg", "163.jpg", "164.jpg", "165.jpg", "166.jpg", "167.jpg", "168.jpg", "169.jpg",
					"170.jpg", "171.jpg", "172.jpg", "173.jpg", "174.jpg", "175.jpg", "176.jpg", "177.jpg", "178.jpg", "179.jpg",
					"180.jpg", "181.jpg", "182.jpg", "183.jpg", "184.jpg", "185.jpg", "186.jpg", "187.jpg", "188.jpg", "189.jpg",
					"190.jpg", "191.jpg", "192.jpg", "193.jpg", "194.jpg", "195.jpg", "196.jpg", "197.jpg", "198.jpg", "199.jpg",
					"200.jpg", "201.jpg", "202.jpg", "203.jpg", "204.jpg", "205.jpg", "206.jpg", "207.jpg", "208.jpg", "209.jpg",
					"210.jpg", "211.jpg", "212.jpg", "213.jpg", "214.jpg", "215.jpg", "216.jpg", "217.jpg", "218.jpg", "219.jpg",
					"220.jpg", "221.jpg", "222.jpg", "223.jpg", "224.jpg", "225.jpg", "226.jpg", "227.jpg", "228.jpg", "229.jpg",
					"230.jpg", "231.jpg", "232.jpg", "233.jpg", "234.jpg", "235.jpg", "236.jpg", "237.jpg", "238.jpg", "239.jpg",
					"240.jpg", "241.jpg", "242.jpg", "243.jpg", "244.jpg", "245.jpg", "246.jpg", "247.jpg", "248.jpg", "249.jpg",
					"250.jpg", "251.jpg", "252.jpg", "253.jpg", "254.jpg", "255.jpg", "256.jpg", "257.jpg", "258.jpg", "259.jpg",
					"260.jpg", "261.jpg", "262.jpg", "263.jpg", "264.jpg", "265.jpg", "266.jpg", "267.jpg", "268.jpg", "269.jpg",
					"270.jpg", "271.jpg", "272.jpg", "273.jpg", "274.jpg", "275.jpg", "276.jpg", "277.jpg", "278.jpg", "279.jpg",
					"280.jpg", "281.jpg", "282.jpg", "283.jpg", "284.jpg", "285.jpg", "286.jpg", "287.jpg", "288.jpg", "289.jpg",
					"290.jpg", "291.jpg", "292.jpg", "293.jpg", "294.jpg", "295.jpg", "296.jpg", "297.jpg", "298.jpg", "299.jpg"};
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

void bindProgram()
{
	// upload the projection matrix to GPU
	float p[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::Projection);
	matrixHandler->GetMatrix(p);
	pipelineHandler->SetProjectionMatrix(p);

	// upload the mdoelview matrix to GPU
	float m[16];
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->GetMatrix(m);
	pipelineHandler->SetModelViewMatrix(m);

	// bind the pipeline
	pipelineHandler->Bind();
}

void renderObject()
{
	GLint first = 0;
	GLsizei count = 3;
	switch (drawState)
	{
	case POINT_MODE:
		// darw point
		count = numPointVertices;
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_POINTS, first, count);
		break;
	case WIREFRAME_MODE:
		// draw wireframe
		count = numLineVertices;
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_LINE_STRIP, first, count);
		break;
	case SOLID_MODE:
		// draw solid
		count = numTriangleVertices;
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLE_STRIP, first, count);
		break;
	}

	glBindVertexArray(0);	// unbind the VAO
}

void displayFunc()
{
	// clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set a camera
	matrixHandler->SetMatrixMode(OpenGLMatrix::ModelView);
	matrixHandler->LoadIdentity();
	// camera z-position
	float zStudent = heightmapImage->getHeight() + 3.0f + 1666934179.0f / 10000000000.0f;
	matrixHandler->LookAt(0, 0, zStudent, 0, 0, -1, 0, 1, 0);

	// transformations
	matrixHandler->Translate(landTranslate[0], landTranslate[1], landTranslate[2]);
	matrixHandler->Rotate(landRotate[0], 1.0f, 0.0f, 0.0f);
	matrixHandler->Rotate(landRotate[1], 0.0f, 1.0f, 0.0f);
	matrixHandler->Rotate(landRotate[2], 0.0f, 0.0f, 1.0f);
	matrixHandler->Scale(landScale[0], landScale[1], landScale[2]);

	// binding
	bindProgram();
	// render objects
	renderObject();

	glutSwapBuffers();
}

void idleFunc()
{
	if (!stop) {
		// do some stuff... 
		landRotate[1] += delta;
		if (landRotate[1] > 360.0f)
		{
			landRotate[1] -= 360.0f;
		}

		// screen shot
		if (nameIdx < 300)
			saveScreenshot(name[nameIdx++]);
		else
			nameIdx = 0;
	}

	// make the screen update 
	glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
	glViewport(0, 0, w, h);

	// setup perspective matrix
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
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
			landTranslate[0] += mousePosDelta[0] * 0.1f;
			landTranslate[1] -= mousePosDelta[1] * 0.1f;
		}
		if (middleMouseButton)
		{
			// control z translation via the middle mouse button
			landTranslate[2] += mousePosDelta[1] * 0.1f;
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
	// a mouse button has been pressed or depressed

	// keep track of the mouse button state, in leftMouseButton, middleMouseButton, rightMouseButton riables
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
		delete[] pointPositions;
		delete[] pointColors;
		delete[] linePositions;
		delete[] lineColors;
		delete[] trianglePositions;
		delete[] triangleColors;
		exit(0); // exit the program
		break;
	case ' ':
		cout << "You pressed the spacebar." << endl;
		stop = !stop;
		break;
	case 'x':
		// take a screenshot
		saveScreenshot("300.jpg");
		break;

	case '1':
		// point
		drawState = POINT_MODE;
		break;
	case '2':
		// wireframe
		drawState = WIREFRAME_MODE;
		break;
	case '3':
		// solid
		drawState = SOLID_MODE;
		break;
	}
}

void loadJPEG(const char* filename)
{
	heightmapImage = new ImageIO();
	if (heightmapImage->loadJPEG(filename) != ImageIO::OK)
	{
		cout << "Error reading image " << filename << "." << endl;
		exit(EXIT_FAILURE);
	}
}

void initPoints(int width, int height, int channel)
{
	numPointVertices = width * height;
	// allocate pointPositions(x, y, z) and colors(r, g, b, a)
	if (pointPositions != NULL)
		delete[] pointPositions;
	pointPositions = new GLfloat[sizeof(GLfloat) * 3 * numPointVertices];
	if (pointColors != NULL)
		delete[] pointColors;
	pointColors = new GLfloat[sizeof(GLfloat) * 4 * numPointVertices];
	// grayscale or color image
	int r = 0, g = 1, b = 2;	// color
	if (channel == 1)
	{
		r = g = b = 0;	// gray
	}

	float scale = 0.2f; // height scale
	int offset_p = 0;
	int offset_c = 0;
	// initialize pointPositions(x, y, z) and pointColors(r, g, b, a)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// x, y, z
			pointPositions[offset_p++] = (GLfloat)x - width / 2.0f;
			pointPositions[offset_p++] = (GLfloat)scale * (0.299 * heightmapImage->getPixel(x, y, r) + 0.587 * heightmapImage->getPixel(x, y, g) + 0.114 * heightmapImage->getPixel(x, y, b));
			pointPositions[offset_p++] = (GLfloat)-y + height / 2.0f;
			// r = g = b, a = 1
			pointColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, r) / 255.0f;
			pointColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, g) / 255.0f;
			pointColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, b) / 255.0f;
			pointColors[offset_c++] = 1.0f;
		}
	}
}

void initLines(int width, int height, int channel)
{
	numLineVertices = 3 * width * height - 2 * (width + height - 1);
	// allocate linePositions(x, y, z) and lineColors(r, g, b, a)
	if (linePositions != NULL)
		delete[] linePositions;
	linePositions = new GLfloat[sizeof(GLfloat) * 3 * numLineVertices];
	if (lineColors != NULL)
		delete[] lineColors;
	lineColors = new GLfloat[sizeof(GLfloat) * 4 * numLineVertices];
	// grayscale or color image
	int r = 0, g = 1, b = 2;	// color
	if (channel == 1)
	{
		r = g = b = 0;	// gray
	}

	float scale = 0.2f; // height scale
	int offset_p = 0;
	int offset_c = 0;
	// initialize linePositions(x, y, z) and lineColors(r, g, b, a)
	for (int y = 0; y < height; y++)
	{
		if (y < 1) {
			// first line
			for (int x = 0; x < width; x++)
			{
				// position
				linePositions[offset_p++] = (GLfloat)(width - 1 - x) - width / 2.0f;
				linePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(width - 1 - x, 0, r) + 0.587*heightmapImage->getPixel(width - 1 - x, 0, g) + 0.114*heightmapImage->getPixel(width - 1 - x, 0, b));
				linePositions[offset_p++] = (GLfloat)height / 2.0f;

				// color
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, 0, r) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, 0, g) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, 0, b) / 255.0f;
				lineColors[offset_c++] = 1.0f;
			}
		}
		else {
			// one part of the strip
			for (int x = 0; x < width - 1; x++)
			{
				// position
				linePositions[offset_p++] = (GLfloat)x - width / 2.0f;
				linePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(x, y, r) + 0.587*heightmapImage->getPixel(x, y, r) + 0.114*heightmapImage->getPixel(x, y, r));
				linePositions[offset_p++] = (GLfloat)-y + height / 2.0f;

				linePositions[offset_p++] = (GLfloat)(x + 1) - width / 2.0f;
				linePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(x + 1, y - 1, r) + 0.587*heightmapImage->getPixel(x + 1, y - 1, g) + 0.114*heightmapImage->getPixel(x + 1, y - 1, b));
				linePositions[offset_p++] = (GLfloat)(-y + 1) + height / 2.0f;

				// color
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, r) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, g) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, b) / 255.0f;
				lineColors[offset_c++] = 1.0f;

				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x + 1, y - 1, r) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x + 1, y - 1, g) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x + 1, y - 1, b) / 255.0f;
				lineColors[offset_c++] = 1.0f;
			}
			// last line
			for (int x = 0; x < width; x++)
			{
				// position
				linePositions[offset_p++] = (GLfloat)(width - 1 - x) - width / 2.0f;
				linePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(width - 1 - x, y, r) + 0.587*heightmapImage->getPixel(width - 1 - x, y, g) + 0.114*heightmapImage->getPixel(width - 1 - x, y, b));
				linePositions[offset_p++] = (GLfloat)-y + height / 2.0f;

				// color
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, y, r) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, y, g) / 255.0f;
				lineColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1 - x, y, b) / 255.0f;
				lineColors[offset_c++] = 1.0f;
			}
		}
	}
}

void initTriangles(int width, int height, int channel)
{
	numTriangleVertices = (2 * width) * (height - 1) + 2 * (height - 2);
	// allocate trianglePositions(x, y, z) and triangleColors(r, g, b, a)
	if (trianglePositions != NULL)
		delete[] trianglePositions;
	trianglePositions = new GLfloat[sizeof(GLfloat) * 3 * numTriangleVertices];
	if (triangleColors != NULL)
		delete[] triangleColors;
	triangleColors = new GLfloat[sizeof(GLfloat) * 4 * numTriangleVertices];
	// grayscale or color image
	int r = 0, g = 1, b = 2;	// color
	if (channel == 1)
	{
		r = g = b = 0;	// gray
	}

	float scale = 0.2f; // height scale
	int offset_p = 0;
	int offset_c = 0;
	// initialize trianglePositions(x, y, z) and triangleColors(r, g, b, a)
	for (int y = 0; y < height - 1; y++)
	{
		if (y > 0)
		{
			// repeat first vertex
			trianglePositions[offset_p++] = (GLfloat)-width / 2.0f;
			trianglePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(0, y, r) + 0.587*heightmapImage->getPixel(0, y, g) + 0.114*heightmapImage->getPixel(0, y, b));
			trianglePositions[offset_p++] = (GLfloat)-y + height / 2.0f;

			// color
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(0, y, r) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(0, y, g) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(0, y, b) / 255.0f;
			triangleColors[offset_c++] = 1.0f;
		}
		for (int x = 0; x < width; x++)
		{
			// one part of the strip
			trianglePositions[offset_p++] = (GLfloat)x - width / 2.0f;
			trianglePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(x, y, r) + 0.587*heightmapImage->getPixel(x, y, g) + 0.114*heightmapImage->getPixel(x, y, b));
			trianglePositions[offset_p++] = (GLfloat)-y + height / 2.0f;

			trianglePositions[offset_p++] = (GLfloat)x - width / 2.0f;
			trianglePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(x, y + 1, r) + 0.587*heightmapImage->getPixel(x, y + 1, g) + 0.114*heightmapImage->getPixel(x, y + 1, b));
			trianglePositions[offset_p++] = (GLfloat)(-y - 1) + height / 2.0f;

			// color
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, r) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, g) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y, b) / 255.0f;
			triangleColors[offset_c++] = 1.0f;

			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y + 1, r) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y + 1, g) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(x, y + 1, b) / 255.0f;
			triangleColors[offset_c++] = 1.0f;
		}
		if (y < height - 2)
		{
			// repeat last index
			trianglePositions[offset_p++] = (GLfloat)(width - 1) - width / 2.0f;
			trianglePositions[offset_p++] = (GLfloat)scale * (0.299*heightmapImage->getPixel(width - 1, y + 1, r) + 0.587*heightmapImage->getPixel(width - 1, y + 1, g) + 0.114*heightmapImage->getPixel(width - 1, y + 1, b));
			trianglePositions[offset_p++] = (GLfloat)(-y - 1) + height / 2.0f;

			// color
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1, y + 1, r) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1, y + 1, g) / 255.0f;
			triangleColors[offset_c++] = (GLfloat)heightmapImage->getPixel(width - 1, y + 1, b) / 255.0f;
			triangleColors[offset_c++] = 1.0f;
		}
	}
}

void initHandler()
{
	// initialize openGLMatrix
	matrixHandler = new OpenGLMatrix();
	// initialize shader pipeline program
	pipelineHandler = new BasicPipelineProgram();
	pipelineHandler->Init("../openGLHelper-starterCode");
}

void initVBO(int idx, int numVerts, GLfloat* positions, GLfloat* colors)
{
	// reserve a name for the buffer object
	glGenBuffers(1, &vbo[idx]);
	// bind it to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, vbo[idx]);
	// Allocate space for it (sizeof(pointPositions) + sizeof(pointColors))
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * numVerts + sizeof(GLfloat) * 4 * numVerts, NULL, GL_STATIC_DRAW);

	// upload position data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * numVerts, positions);
	// upload color data
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * numVerts, sizeof(GLfloat) * 4 * numVerts, colors);
}

void initVAO(int idx, int numVerts)
{
	// generate and bind the vertex arrays
	glGenVertexArrays(1, &vao[idx]);
	glBindVertexArray(vao[idx]);

	// get a handle to the program
	GLuint program = pipelineHandler->GetProgramHandle();
	// position
	GLuint loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(loc);
	const void* offset = (const void*)0;
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, offset);

	// color
	GLuint loc2 = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(loc2);
	const void* offset2 = (const void*)(sizeof(GLfloat) * 3 * numVerts);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, offset2);

	glBindVertexArray(0);	// unbind the VAO
}

void initScene(int argc, char *argv[])
{
	// background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set properties
	glEnable(GL_DEPTH_TEST);

	// load the image from a jpeg disk file to main memory
	loadJPEG("heightmap/island.jpg");

	// initialize the system(handler, vertex&color, vbo, vao)
	initHandler();
	// point
	initPoints(heightmapImage->getWidth(), heightmapImage->getHeight(), heightmapImage->getBytesPerPixel());
	initVBO(0, numPointVertices, pointPositions, pointColors);
	initVAO(0, numPointVertices);
	// line
	initLines(heightmapImage->getWidth(), heightmapImage->getHeight(), heightmapImage->getBytesPerPixel());
	initVBO(1, numLineVertices, linePositions, lineColors);
	initVAO(1, numLineVertices);
	// triangle
	initTriangles(heightmapImage->getWidth(), heightmapImage->getHeight(), heightmapImage->getBytesPerPixel());
	initVBO(2, numTriangleVertices, trianglePositions, triangleColors);
	initVAO(2, numTriangleVertices);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "The arguments are incorrect." << endl;
		cout << "usage: ./hw1 <heightmap file>" << endl;
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



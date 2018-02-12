/* **************************
 * CSCI 420
 * Assignment 3 Raytracer
 * Name: Changki Lee
 * *************************
*/

#ifdef WIN32
  #include <windows.h>
#endif

#if defined(WIN32) || defined(linux)
  #include <GL/gl.h>
  #include <GL/glut.h>
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <GLUT/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gRayTracer.h"
#ifdef WIN32
  #define strcasecmp _stricmp
#endif

#include <imageIO.h>

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2

int mode = MODE_DISPLAY;
char * filename = NULL;

GRayTracer rayTracer;

#define WIDTH 640
#define HEIGHT 480

unsigned char buffer[HEIGHT][WIDTH][3];

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

void draw_scene()
{
  //a simple test output
  for(unsigned int x = 0; x < WIDTH; ++x)
  {
    glPointSize(2.0);  
    glBegin(GL_POINTS);
    for(unsigned int y = 0; y < HEIGHT; ++y)
    {
	  GColor pixel(0, 0, 0);
	  for (float i = -0.5; i <= 0.5; i+=0.5)
		  for (float j = -0.5; j <= 0.5; j+=0.5)
			  pixel += rayTracer.singlePixelTrace(x + i, y + j);
	  pixel = pixel * (1 / 9.0);
      plot_pixel(x, y, pixel.getRed(), pixel.getGreen(), pixel.getBlue());
    }
    glEnd();
    glFlush();
  }
  printf("Done!\n"); fflush(stdout);
}

void plot_pixel_display(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  glColor3f(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  buffer[y][x][0] = r;
  buffer[y][x][1] = g;
  buffer[y][x][2] = b;
}

void plot_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
    plot_pixel_jpeg(x,y,r,g,b);
}

void save_jpg()
{
  printf("Saving JPEG file: %s\n", filename);

  ImageIO img(WIDTH, HEIGHT, 3, &buffer[0][0][0]);
  if (img.save(filename, ImageIO::FORMAT_JPEG) != ImageIO::OK)
    printf("Error in Saving\n");
  else 
    printf("File saved Successfully\n");
}

void display()
{
}

void setCamera()
{
	// camera setting
	GPoint3 cameraPos(0, 0, 10);
	GPoint3 lookAt(0, 0, -1);
	GVector3 upVec(0, 1, 0);
	double fov = 60.0;
	rayTracer.setCamera(cameraPos, lookAt, upVec, fov);
	rayTracer.setResolution(WIDTH, HEIGHT);
	rayTracer.setMaxLevel(5);
}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  setCamera();
}

void idle()
{
  //hack to make it only draw once
  static bool once = false;
  if(!once)
  {
    draw_scene();
    if(mode == MODE_JPEG)
      save_jpg();
  }
  once = true;
}

int main(int argc, char ** argv)
{
  if ((argc < 2) || (argc > 3))
  {  
    printf ("Usage: %s <input scenefile> [output jpegname]\n", argv[0]);
    exit(0);
  }
  if(argc == 3)
  {
    mode = MODE_JPEG;
    filename = argv[2];
  }
  else if(argc == 2)
    mode = MODE_DISPLAY;

  glutInit(&argc,argv);
  rayTracer.loadScene(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}


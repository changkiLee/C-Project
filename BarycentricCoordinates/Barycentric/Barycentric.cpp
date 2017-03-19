#include <stdlib.h>
#include <gl/glut.h>
#include <string>
#include <iostream>
using namespace std;

int Width = 800, Height = 600;

void Render();
void Reshape(int w, int h);

void SetupviewTransform();
void SetupViewVolume();

void Mouse(int button, int state, int x, int y);
void BitmapString(float x, float y, char *str, float r, float g, float b);

struct vertex
{
	double x, y;
	float color[3];
};

// vector2d p
vertex p;

double cotangent(vertex a, vertex b, vertex c)
{
	vertex ba;
	ba.x = a.x - b.x;
	ba.y = a.y - b.y;
	vertex bc;
	bc.x = c.x - b.x;
	bc.y = c.y - b.y;

	double dot = (bc.x * ba.x) + (bc.y * ba.y);
	double cross = ba.x*bc.y - ba.y*bc.x;

	if(cross < 0)
		cross = -cross;

	return dot/cross;
}

void computeBarycentric(vertex *q, double *w, int n)
{
	double weightSum = 0;
	for(int i = 0; i < n; i++)
	{
		int j = (i + n - 1) % n;
		int k = (i + 1) % n;
		w[i] = (cotangent(p, q[i], q[j]) + cotangent(p, q[i], q[k])) / ((p.x - q[i].x)*(p.x - q[i].x) + (p.y - q[i].y)*(p.y - q[i].y));
		weightSum += w[i];
	}
	for(int i = 0; i < n; i++){
		w[i] /= weightSum;
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Generalized Barycentric Coordinates on Irregular Polygons");

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);

	glutMouseFunc(Mouse);
	glutMainLoop();
	
	return 0;
}

void Render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 뷰&관측 변환
	SetupviewTransform();
	SetupViewVolume();
	glMatrixMode(GL_MODELVIEW);

	int n = 6;
	// polygon Q
	vertex *q = new vertex [n];
	double *w = new double [n];

	for(int i = 0; i < n; i++)
	{
		q[i].x = 0;
		q[i].y = 0;
		w[i] = 0;
	}

	// q 위치
	q[0].x = 0.0; q[0].y = 150.0;
	q[1].x = -150.0; q[1].y = 120.0;
	q[2].x = -200.0; q[2].y = -50.0;
	q[3].x = 20.0; q[3].y = -200.0;
	q[4].x = 180.0; q[4].y = -80.0;
	q[5].x = 120.0; q[5].y = 130.0;

	// q 색
	q[0].color[0] = 1.0; q[0].color[1] = 0.0; q[0].color[2] = 0.0;
	q[1].color[0] = 0.0; q[1].color[1] = 1.0; q[1].color[2] = 0.0;
	q[2].color[0] = 0.0; q[2].color[1] = 0.0; q[2].color[2] = 1.0;
	q[3].color[0] = 1.0; q[3].color[1] = 1.0; q[3].color[2] = 0.0;
	q[4].color[0] = 0.0; q[4].color[1] = 1.0; q[4].color[2] = 1.0;
	q[5].color[0] = 1.0; q[5].color[1] = 0.0; q[5].color[2] = 1.0;
	
	// 계산
	computeBarycentric(q, w, n);

	// 높이값
	float h[6];
	h[0] = 1;
	h[1] = 1;
	h[2] = 1;
	h[3] = 1;
	h[4] = 1;
	h[5] = 1;

	p.color[0] = 0.0;
	p.color[1] = 0.0;
	p.color[2] = 0.0;
	// p 색
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < n; j++)
		{
			p.color[i] += (h[i] * w[j] * q[j].color[i]);
		}
	}

	// 다면체
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3fv(p.color);
		glVertex2d(p.x, p.y);
		for(int i = 0; i < n; i++)
		{
			glColor3fv(q[i].color);
			glVertex2d(q[i].x, q[i].y);
		}
		glColor3fv(q[0].color);
		glVertex2d(q[0].x, q[0].y);
	}
	glEnd();

	// p색
	glBegin(GL_QUADS);
	{
		glColor3fv(p.color);
		glVertex2d(350, 100);
		glVertex2d(350, 200);
		glVertex2d(250, 200);
		glVertex2d(250, 100);
	}
	glEnd();

	// 문자
	BitmapString(p.x, p.y, "p", 1.0, 0.0, 0.0);
	BitmapString(q[0].x, q[0].y, "q0", 0.0, 0.0, 0.0);
	BitmapString(q[1].x, q[1].y, "q1", 0.0, 0.0, 0.0);
	BitmapString(q[2].x, q[2].y, "q2", 0.0, 0.0, 0.0);
	BitmapString(q[3].x, q[3].y, "q3", 0.0, 0.0, 0.0);
	BitmapString(q[4].x, q[4].y, "q4", 0.0, 0.0, 0.0);
	BitmapString(q[5].x, q[5].y, "q5", 0.0, 0.0, 0.0);

	// p값 비교
	cout << "p의 좌표값(x, y)               : " << p.x << ", " << p.y << endl;
	double x = 0;
	double y = 0;
	for(int i = 0; i < n; i++)
	{
		x += w[i]*q[i].x;
		y += w[i]*q[i].y;
	}
	cout << "계산으로 구한 p의 좌표값(x, y) : " << x << ", " << y << endl << endl;

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// Reset the viewport to the new dimensions
	glViewport(0, 0, w, h);
	// Set up the projection
	SetupViewVolume();
}

// 뷰잉 변환
void SetupviewTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0);
}

// 관측 변환
void SetupViewVolume()
{
	// Select the projection matrix and clear it out
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set up orthographic projection
	// “aspect” is a global variable
	glOrtho(-Width/2, Width/2, -Height/2, Height/2, -100, 100);
}

void Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		p.x = x - Width/2;
		p.y = Height/2 - y;
	}
	glutPostRedisplay();
}

// 글자 출력
void BitmapString(float x, float y, char *str, float r, float g, float b)
{
	int len;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	len = (int) strlen(str);

	for(int i = 0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}
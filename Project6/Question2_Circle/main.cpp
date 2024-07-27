//mid point circle
#include <GL\freeglut.h>
#include <Windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <ctime>
#include <vector>
using namespace std;

struct screen_point_struct
{
	GLint x, y;
};

void setpixel(GLint x, GLint y);
void init();
void display();
void MidPointAlgo(screen_point_struct center, GLint radius);
//void plotin8octants(screen_point_struct center, Point_on_screen );


class Point_on_screen
{
private:
	GLint x, y;
public:
	Point_on_screen() {
		x = 0;
		y = 0;
	}
	void setcord(GLint x_val, GLint y_val)
	{
		x = x_val;
		y = y_val;
	}
	GLint X_val() const {
		return x;
	}
	GLint Y_val() const {
		return y;
	}

	void incx()
	{
		++x;
	}

	void decy()
	{
		--y;
	}
};

void setpixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void plotin8octants(screen_point_struct center, Point_on_screen P)
{
	setpixel(center.x + P.X_val(), center.y + P.Y_val());
	setpixel(center.x - P.X_val(), center.y + P.Y_val());
	setpixel(center.x + P.X_val(), center.y - P.Y_val());
	setpixel(center.x - P.X_val(), center.y - P.Y_val());

	setpixel(center.x + P.Y_val(), center.y + P.X_val());
	setpixel(center.x - P.Y_val(), center.y + P.X_val());
	setpixel(center.x + P.Y_val(), center.y - P.X_val());
	setpixel(center.x - P.Y_val(), center.y - P.X_val());

}

//scree_point_struct is the argument
void MidPointAlgo(screen_point_struct center, GLint radius)
{
	Point_on_screen current_point;
	current_point.setcord(0, radius);
	plotin8octants(center, current_point);

	GLint p = 1 - radius;
	while (current_point.X_val() < current_point.Y_val())
	{
		current_point.incx();
		if (p < 0)
		{
			p += 2 * current_point.X_val() + 1;

		}
		else
		{
			current_point.decy();
			p += 2 * current_point.X_val() + 1 - 2 * current_point.Y_val() + 1;
		}
		plotin8octants(center, current_point);
	}
}


void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 600, 0, 600);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	screen_point_struct center;
	center.x = 300;
	center.y = 300;

	glColor3f(0.0f, 0.0f, 1.0f);
	MidPointAlgo(center,200);
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Midpoint Circle Drawing");
	init(); //initialisation
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
#include <GL/glut.h>
#include<GL/glu.h>
#include <stdlib.h>
#include<stdio.h>

int first = 0;
int xs, ys, xf, yf;

char initialPoint[10], finalPoint[10];
void *font = GLUT_BITMAP_TIMES_ROMAN_24;

void showstring(int x, int y, char* string)
{
	glRasterPos2i(x, y);
	int str_length = (int)strlen(string);
	for (int i = 0; i <= str_length; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

void drawpixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void bresenhamDrawLine(int x1, int y1, int x2, int y2)
{
	int dx, dy, P0;
	dx = x2 - x1;
	dy = y2 - y1;

	int Xinc = (dx < 0) ? -1 : 1;
	int Yinc = (dy < 0) ? -1 : 1;

	dx = abs(dx);
	dy = abs(dy);

	int xk = x1;
	int yk = y1;

	if (dy < dx)
	{
		P0 = 2 * dy - dx;
		drawpixel(x1, y1);
		for (int k = x1; k <= x2; k++)
		{
			if (P0 < 0)
			{
				drawpixel(++xk, yk);
				P0 += 2 * dy;
			}
			else
			{
				drawpixel(++xk, yk = yk + Yinc);
				P0 += 2 * dy-2*dx;
			}
		}

	}
	else
	{
		P0 = 2 * dx - dy;
		drawpixel(x1, y1);
		for (int k = y1; k <= y2; k++)
		{
			if (P0 < 0)
			{
				drawpixel(xk, ++yk);
				P0 += 2 * dy;
			}
			else
			{
				drawpixel(xk = xk + Xinc, ++yk);					;
				P0 += 2 * dx - 2 * dy;
			}
		}
	}

}

void display(void)
{
	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	showstring(xs + 5, ys, initialPoint);
	showstring(xf + 5, yf, finalPoint);

	if (first == 0)
	{
		int dx = xf - xs;
		int dy = yf - ys;

		if (abs(dy) < abs(dx))
		{
			if (xs < xf)
				bresenhamDrawLine(xs, ys, xf, yf);
			else
				bresenhamDrawLine(xf, yf, xs, ys);
		}
		else
		{
			if (ys < yf)
				bresenhamDrawLine(xs, ys, xf, yf);
			else
				bresenhamDrawLine(xf, yf, xs, ys);
		}
	}

	glFlush();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		switch (first)
		{
		case 0: 
			//sprintf_s(initialPoint, " ");
			//sprintf_s(finalPoint, " ");			
			xs = x;
			ys = 600 - y;
			sprintf_s(initialPoint, "(%d,%d)", xs, ys);
			sprintf_s(finalPoint, " ");
			first = 1;
			break;

		case 1:
			xf = x;
			yf = 600 - y;
			sprintf_s(finalPoint, "(%d,%d)", xf, yf);
			first = 0;
			break;
		}
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);

	glutCreateWindow("Bresenham's Line Drawing Algo");
	gluOrtho2D(0, 800, 0, 600);

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
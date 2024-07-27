#pragma once
#ifndef ShapeDrawer_h
#define ShapeDrawer_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <cstdlib>
#include <ctime>

#define _USE_MATH_DEFINES
#include <cmath>

#define M_PI 3.14159265358979323846 /**< A macros for Pi value */


#define direction(x1, x2) ((x2 > x1) ? 1 : -1) /**< Macros to check if x is increasing or decresing */
#define toRadians(degrees) ((double)degrees * M_PI)/180 /**< Macros to convert angles from degrees to radians */

//! A Class to draw circle and lines.
class ShapeDrawer {

private:

    GLfloat red, green, blue, pointSize;

    void swap(int* a, int* b) {
        ///Swap two numbers.
        int d = *a;
        *a = *b;
        *b = d;
    }

    void plotCircle(GLint xi, GLint yi, GLint xc, GLint yc) {

        /// Plotting symmetrical pixels in all 8 octants to draw the circle.
        drawPixel(xi + xc, yi + yc);
        drawPixel(xi + xc, yc - yi);
        drawPixel(xc - xi, yc + yi);
        drawPixel(xc - xi, yc - yi);
        drawPixel(xc + yi, yc + xi);
        drawPixel(xc + yi, yc - xi);
        drawPixel(xc - yi, yc + xi);
        drawPixel(xc - yi, yc - xi);

    }

public:

    ShapeDrawer() {
        /// Constructor method for ShapeDrawer.
        ///+ Set the default color to red in case setColour method is not called.
        ///+ seed the srand with timestamp.
        this->red = 1.0; 
        this->green = 0.0;
        this->blue = 0.0;
        this->pointSize = 1.0;
        srand((unsigned int)time(NULL));
    }

    void randomizeColour() {

        ///+ A method to update the values of red, green, blue with some random doubles.
        this->red = ((double)(rand() % 100)) / 100;
        this->green = ((double)(rand() % 100)) / 100;
        this->blue = ((double)(rand() % 100)) / 100;
    }

    void setColour(GLfloat red = 1.0, /**< Red value */
        GLfloat green = 0.0, /**< Green value */
        GLfloat blue = 0.0 /**< Blue value */
    ) {
        /// Setting the colour of the drawing object.
        /** By default, red color is chosen. */
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void setPointSize(GLfloat pointSize /**< Point size in pixels */
    ) {
        /// Sets the size of the pixel for the drawing object
        this->pointSize = pointSize;
    }

    void drawPixel(GLint x, /**< X coordinate of the pixel to be illuminated */
        GLint y /**< Y coordinate of the pixel to be illuminated */
    ) {
        /// Illuminating a pixel with given coordinates on screen.
        ///+ Pixel is illuminated with the drawing color of the ShapeDrawer object. /
        glColor3f(red, green, blue);
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    void drawLine(int x1, /**< X coordinate of the starting point */int y1, /**< Y coordinate of the starting point */int x2, /**< X coordinate of the ending point */int y2 /**< Y coordinate of the ending point */)
    {

        /// Drawning a line using mid point algorithm or Bressenhm algorithm
        /** Drawing the line using the current color of ShapeDrawer object */

        int dx = abs(x2 - x1); // the magnitude of change in X-Direction
        int dy = abs(y2 - y1); // the magnitude of change in Y-Direction
        int xDirection = 0, yDirection = 0, swapStatus = 0;
        int i, x = x1, y = y1;

        xDirection = direction(x1, x2); //Find whether X is increasing or decreasing
        yDirection = direction(y1, y2); //Find whether Y is increasing or decreasing

        if (dy > dx) {
            swap(&dy, &dx); // If slope is greater than 1, swap dx and dy
            swapStatus = 1; // change flag 
        }

        int d = 2 * dy - dx; 
        int dNE = 2 * (dy - dx); // Change in value of decision variable of North East pixel is illuminated
        int dE = 2 * dy; // Change in value of decision variable of East pixel is illuminated

        for (i = 1; i <= dx; i++) { // Start in X direction.
            drawPixel(x, y); // Illuminate the first pixel.

            if (d >= 0) // Select North East pixel if d >= 0
            {
                if (swapStatus)
                    x = x + xDirection; // Increment x if we are moving along Y direction
                else
                    y = y + yDirection; // Increment y if we are moving along X direction.

                d = d + dNE; // Change the value of decision variable.
            }
            else
                d = d + dE;

            if (swapStatus)
                y = y + yDirection; // Move along Y axis if the slope of line is greater than 1 i.e. dy and dx were swapped.
            else
                x = x + xDirection; // Move along X in the other case.


        }
    }

    void drawLineAtAngle(GLint x1, /**< X coordinate of the starting point */GLint y1, /**< Y coordinate of the starting point */double angle, /**< Angle made */double length /**< Length of the line segment */) 
    {

        /// Drawing a line segment given it's starting point, length and the angle made with positive X direction
        ///+ The line is drawn using the current color of ShapeDrawer object. Using mid point algorithm /
        // Select the ending point based on the length and angle with positive X direction.
        int x2 = round(x1 + (length * cos(toRadians(angle))));
        int y2 = round(y1 + (length * sin(toRadians(angle))));

        // Call the drawLine method
        drawLine(x1, y1, x2, y2);

    }

    void drawCircle(int x0, /**< X coordinate of the centre of the circle */
        int y0, /**< Y coordinate of the centre of the center of the circle */
        int r0 /**< Radius of the circle */
    ) {
        /// A method to draw a circle using midpoint algorithm with constant increments given it's centre and radius.
        /** The circle is drawn using the current color of ShapeDrawer object */

        // We draw only 1/8 of a circle and plot remaining points using symmetry.

        int x = 0; // X coordinate of first point.
        int y = r0; // Y coordinate of first point.
        int d = 1 - r0; // Initial value of decision variable

        // Method of second order difference
        int deltaE = 3;
        int deltaSE = -2 * r0 + 5;

        while (x <= y) {
            plotCircle(x, y, x0, y0); // Plot symmetrical points

            // Choosing pixel 
            if (d < 0) {
                d = d + deltaE; // Increment d.
                deltaE = deltaE + 2;
                deltaSE = deltaSE + 2; 
            }
            else {
                d = d + deltaSE;
                deltaE = deltaE + 2;
                deltaSE = deltaSE + 4;
                y--;
            }
            x++;
        }
    }
};


#endif
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Lindenmayer.h"

/**
\mainpage 
* # Title: Plants, fractals, and formal languages
* ## IS F311, Computer Graphics Project 1
* 
* Adapala Sanjana 2020B4A72243H

* Akshitha Bajjuri 2020B2A71973H

* Aman Agarwal 2020B4AA2328H

* ### Symbols
* 
*+ F : indicates to draws a line at a given coordinate at the specified angle and length.
*+ - : rotate right
*+ + : rotate left
*+ [ : save current state
*+ ] : restore previous state
*+ / : draw circle at the end of the line
*+ C0: change drawing colour to the first colour in the matrix
*+ C1: change drawing colour to the second colour in the matrix
*+ C2: change drawing colour to the second colour in the matrix 
* 
* ### Axioms, Rules and Iterations
* 
*#### Tree: 
* 
*   Axiom : F
* 
*   Rule : F = C0FC1/F+[+F-F-C2/F]-[-C0F+F+F]
* 
*   Iterations: 5
* 
*#### Sun: 
* 
*   Axiom : [X]++[X]++[X]++[X]++[X]
* 
*   Rules 
*+      W=YF++ZF----XF[-YF----WF]++
*+      X=C0+YF--ZF[C1---WF--XF]+
*+      Y=-WF++XF[+++YF++ZF]-
*+      Z=--YF++++WF[+ZF++++XF]--XF
*+      F=
* 
*Iterations: 4
* 
* ###Output Images
* 
* #### Iteration 1
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\it1.png" width = "40%">
* 
* #### Iteration 2
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\it2.png" width = "40%">
* 
* #### Iteration 3
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\it3.png" width = "40%">
* 
* #### Iteration 4
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\it4.png" width = "40%">
* 
* #### Iteration 5
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\final_image.png" width = "40%">
* 
* # Algortihm Description
* Lindenmayer systems or L-systems use parallel rewriting systems to model various organisms and fractals. It consists of an alphabet to make strings, an axiom and set of rules for production. 
* L-Systems are recursive in nature, which leads to self-similarity. Our project consists of two main shapes - line and circle. The line drawing is an implementation of Bressenham's Line Algorithms, and the cicle utilizes the Midpoint Circle Algorithm. To draw the final output, we use the concept of L-Systems. The generated string is parsed and the set of production rules are applied.
* These rules are described above. The outputs produced with increasing iterations is shown above as well. 
* 
* # Timing Analysis
* We can see clearly that with each iteration, the time for execution(in milliseconds) increases drastically.
* 
* <img src = "D:\Academics\7_sem\Comp Graphics\Assignment 1\Project6\TimeAnalysis.png" width = "40%">
* # Issues in Coding
* Understading the color allocation for the different braches of the tree and understanding the placement for the fruits was a challenge. Drawing the sun and adding a gradient to the sunrays was a learning curve.
* # References
* [1] https://paulbourke.net/fractals/lsys/ 
* 
* [2] https://www.youtube.com/watch?v=f6ra024-ASY&t=1134s
* 
* [3] http://alvyray.com/Papers/CG/PlantsFractalsandFormalLanguages.pdf
* 
* [4] https://docs.racket-lang.org/lindenmayer/Interpreting_L-systems.html?source=post_page-----98e07de6bd63----------------------
*/

void init();
void draw();
void processKeys(unsigned char, int, int);
int screenWidth, screenHeight;

int main(int argc, char** argv)
{

    /// Initialising the glut main window and main loop
    glutInit(&argc, argv);
    init();
    glutMainLoop();
    return 0;

}

void init() {

    ///Initialization of screen dimensions, diplay mode, position. The window  
    screenWidth = glutGet(GLUT_SCREEN_WIDTH); // Get screen width
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT); // Get screen height

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0); // Set initial window position
    glutInitWindowSize(screenWidth, screenHeight); // Set window size
    glutCreateWindow("L-Systems"); // Create and set a window title

    glutKeyboardFunc(processKeys); // Set keyboard event listener
    glutDisplayFunc(draw); // diplaying the L-system drawings on the window

    glutFullScreen(); // Enable full screen mode

}

void draw() {

    /// Setup background gradient and drawing the L-systems

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);

    //Top Gradient
    glColor3f(0.82, 0.95, 1.00);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);

    //Bottom Gradient
    glColor3f(0.36, 0.63, 0.73);
    glVertex2f(1.0, -1.0);
    glVertex2f(-1.0, -1.0);
    glEnd();

    /**************************/

    // Setup ortho 2-D projection matrix.

    gluOrtho2D(0, screenWidth, 0, screenHeight);

    ShapeDrawer sd;
    sd.setColour(0.545, 0.270, 0.074);

    /**************************/

    // Draw the land
    int i;
    double gradient = 1.0;
    for (i = 0; i < screenHeight / 8; i++) {
        sd.setColour(0.36, 0.23, 0.12);
        //gradient -= 4.0 / screenHeight;
        sd.drawLine(0, i, screenWidth, i);
    }

    /**************************/

    // Draw trees
    double leftTreeColors[][3] = { {0.05, 0.16, 0.06}, {0.19, 0.55, 0.08}, {0.576, 0.458, 0.858} };
    Lindenmayer tree("F", 5);
    tree.setX(0.20 * screenWidth);
    tree.setY(screenHeight / 8);
    tree.setAngle(22.5);
    tree.setBranchLength(0.010 * screenHeight);
    tree.setBranchScaleFactor(0.8);
    tree.setPointSize(1);
    tree.setPointScaleFactor(1);
    tree.addRule("F=C0FC1/F+[+F-F-C2/F]-[-C0F+F+F]");
    tree.draw(leftTreeColors);

    double midTree1Colors[][3] = { {0.20, 0.13, 0.09}, {0.41, 0.32, 0.26}, {1.00, 0.52, 0.25} };
    tree.setX(0.38 * screenWidth);
    tree.setY(screenHeight / 8);
    tree.setBranchLength(0.006 * screenHeight);
    tree.draw(midTree1Colors);

    double midTree2Colors[][3] = { {0.05, 0.16, 0.06}, {0.19, 0.55, 0.08}, {0.576, 0.458, 0.858} };
    tree.setX(0.60 * screenWidth);
    tree.setY(screenHeight / 8);
    tree.setBranchLength(0.010 * screenHeight);
    tree.draw(midTree2Colors);

    double rightTreeColors[][3] = { {0.20, 0.13, 0.09}, {0.41, 0.32, 0.26}, {1.00, 0.52, 0.25} };
    tree.setX(0.80 * screenWidth);
    tree.setY(screenHeight / 8);
    tree.setBranchLength(0.006 * screenHeight);
    tree.draw(rightTreeColors);


    /**************************/
    // Draw sun

    Lindenmayer sun("[X]++[X]++[X]++[X]++[X]", 4);
    double sunColors[][3] = { {1.00, 0.78, 0.25 }, {1.00, 0.69, 0.25 } };
    sun.addRule("W=YF++ZF----XF[-YF----WF]++");
    sun.addRule("X=C0+YF--ZF[C1---WF--XF]+");
    sun.addRule("Y=-WF++XF[+++YF++ZF]-");
    sun.addRule("Z=--YF++++WF[+ZF++++XF]--XF");
    sun.addRule("F=");
    sun.setY(0.82 * screenHeight);
    sun.setX(0.85 * screenWidth);
    sun.setPointSize(2);
    sun.setBranchLength(0.01 * screenHeight);
    sun.setBranchScaleFactor(1);
    sun.setAngle(36);
    sun.draw(sunColors);

    glColor3f(1.00, 0.78, 0.25); // Sunray color (yellow)
    int numSunrays = 60; // Number of sunrays
    double sunRadius = 80.0; // Adjust the radius of the sunrays
    double angleIncrement = 360.0 / numSunrays;

    // Draw sunrays using lines
    for (int i = 0; i < numSunrays; i++) {
        double angle = i * angleIncrement;
        double startX = sunRadius * cos(angle * M_PI / 180.0) + 0.85 * screenWidth;
        double startY = sunRadius * sin(angle * M_PI / 180.0) + 0.82 * screenHeight;
        double endX = (sunRadius + 60.0) * cos(angle * M_PI / 180.0) + 0.85 * screenWidth; // Adjust the length of sunrays
        double endY = (sunRadius + 60.0) * sin(angle * M_PI / 180.0) + 0.82 * screenHeight; // Adjust the length of sunrays

        // Draw the sunray
        glBegin(GL_LINES);
        glVertex2d(startX, startY);
        glVertex2d(endX, endY);
        glEnd();

        glBegin(GL_LINES);

        glColor3f(1.00, 0.78, 0.25);
        glVertex2f(startX, startY);
        glColor3f(1.00, 0.88, 0.26);
        glVertex2f(endX, endY);

        glEnd();
    }
    /*******************************/
    glFlush();

}

void processKeys(unsigned char key, int x, int y) {
    ///Function to handle key presses. Exit whenesc is clicked.
    switch (key) {
    case 27:
        exit(0); // Exit on Esc press
        break;
    }

    glutPostRedisplay();
}
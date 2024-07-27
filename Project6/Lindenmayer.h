#pragma once
#ifndef Lindenmayer_h
#define Lindenmayer_h

#include <string>
#include <map>
#include <stack>
#include "ShapeDrawer.h"
#define _USE_MATH_DEFINES
#include <cmath>

#define M_PI 3.14159265358979323846


using namespace std;

//! Lindenmayer class to draw L-system trees and sun

class Lindenmayer {

private:

 /**
 * Turtle struct is meant to store the state variables
 * It specifies branch length, angle, pointsize and colour
 */
    struct turtleState {
        int x, y;
        double branchLength, angle, red, green, blue, pointSize;
    };

    
    int currentX, currentY, iterations;
    double branchLength, pointSize, rotationAngle, branchScaleFactor, pointScaleFactor;
    string axiom, generation;

    // Used to store the rules
    map<string, string> rules;

    //Used to store the states
    stack<turtleState> stateStack;

    // ShapeDrawer object
    ShapeDrawer sd;


public:
    /**
    * A Lindenmayer constructor that takes axiom and number of iterations as arguments.
    * Other fields are set to some default values
    */

    Lindenmayer(string axiom /**<The inital state of the system*/, int iterations /**<Number of times the rules are applied to the axiom */) {

        currentX = 0;
        currentY = 0;
        this->iterations = iterations;
        branchLength = 100;
        pointSize = 1.0;
        rotationAngle = 20.0;
        branchScaleFactor = 0.75;
        pointScaleFactor = 1.0;
        this->axiom = axiom;
        generation = "";
    }

   
    void setX(int x /**< The new X-Coordinate */) {
        /// Method to set the current value of x.
        /** Initial X-Coordinate = 0 */ 
        currentX = x;
    }

    void setY(int y /**< The new Y-Coordinate */) {
        /// Method to set the current value of y.
        /** Initial Y-Coordinate = 0 */
        currentY = y;
    }

    void setAngle(double angle /**< Setting rotation angle for L-System in degrees*/) {
        /// Setting the rotation angle of + and -.
        ///+ '+' corresponds to left rotation
        ///+ '-' corresponds to right rotation

        /** Initial rotation angle = 20 degrees */ 
        rotationAngle = angle;
    }

    void setBranchLength(double branchLength /**< Branch length in the tree */) {
        /// Method for setting number of pixels moved in one step.

        /** Initial branch length = 100px */

        this->branchLength = branchLength;
    }

    void setBranchScaleFactor(double branchScaleFactor /**< The branch scale factor for L-System */) {
        /// Sets the factor that is multiplied to the branch length at every branching off point.

        /** Initial branch scale factor = 0.75 */

        this->branchScaleFactor = branchScaleFactor;
    }

    void setPointScaleFactor(double pointScaleFactor /**< The point scale factor for L-System */) {
        /// Sets the factor that is multiplied to the point size at every branching off point.

        /** Initial point scale factor = 1.0 */

        this->pointScaleFactor = pointScaleFactor;
    }

    void setPointSize(double pointSize /**< The point size for L-System */) {
        /// Method for setting the initial point size.

        /** Initial point size = 1.0 */

        this->pointSize = pointSize;
        sd.setPointSize(pointSize);
    }

    void addRule(string rule /**< The string that has to be applied to the axiom. A rule should have the symbols F, =, C(int), +, -, [, ]*/) {
        /// A method to add rules to the L-System.
        ///Rules are added to the map named rules
        size_t pos = rule.find('=');
        rules.insert(pair<string, string>(rule.substr(0, pos), rule.substr(pos + 1)));
    }

    
    int getX() {
        /// A method to get the current X-Coordinate of turtle.
        return currentX;
    }

    int getY() {
        /// gets the current Y-Coordinate of turtle.
        return currentY;
    }

    double getAngle() {
        /// gets the current angle of the turtle.
        return rotationAngle;
    }

    string getAxiom() {
        /// gets the axiom of current L-System. 
        return axiom;
    }

    double getBranchLength() {
        /// gets the current branch length of L-System.
        return branchLength;
    }

    double getBranchScaleFactor() {
        ///  gets the current value of branchScaleFactor.
        return branchScaleFactor;
    }

    double getPointSize() {
        /// gets the current value of point size.
        return pointSize;
    }

    double getPointScaleFactor() {
        /// gets the current value of pointScaleFactor.
        return pointScaleFactor;
    }


    void draw(double colors[][3] = NULL 
    /**< 2D array that stores colors. Every row has red, green and blue float values that represents one shade.*/) {

        /// This method draws the L- system using the axion and the given set of rules.
        /// 'F' : Draws a line at the current angle using the ShapeDrawer object when 'F' is encountered.
        /// '/' : Draws a circle at current position using ShapeDrawer object when '//' is encountered.
        /// 'C' : To change the colour
        /// '[' : Save the state of turtle.
        /// ']' : Reapply the last saved state of the turtle.
        /// '+' : left rotation of the turtle.
        /// '-' : right rotation of the turtle.

        int i, j;

        if (generation.length() == 0) // To ensure the string has not already been generated
        {
            map<string, string>::iterator it;
            generation = axiom;
            string tGeneration = ""; // A string to store the final L-system string

            // Generating the final generation string.
            for (i = 0; i < iterations; i++) {

                size_t length = generation.length();

                for (j = 0; j < length; j++) {

                    // Find if there is a rule corresponding to the current character being read,
                    it = rules.find(generation.substr(j, 1));

                    if (it != rules.end()) {
                        // Replace the character according to the rule
                        tGeneration = tGeneration + rules.at(generation.substr(j, 1));
                    }
                    else {
                        tGeneration = tGeneration + generation.substr(j, 1);
                    }
                }
                generation = tGeneration;
                tGeneration = "";
            }
        }

        size_t length = generation.length();


        
        double currentAngle = 0.0;
        double currentBranchLength = branchLength;
        double currentPointSize = pointSize;
        double red = 1, green = 0, blue = 0;

        for (i = 0; i < length; i++) {

            // Parsing through final generation string

            char next = generation.at(i);

            if (next == 'F') {
                // Draw a line at the current angle 
                sd.drawLineAtAngle(currentX, currentY, 90 - currentAngle, currentBranchLength);

                //Update the turtle's position to the end of newly drawn line.
                currentX += round(currentBranchLength * sin(toRadians(currentAngle)));
                currentY += round(currentBranchLength * cos(toRadians(currentAngle)));
            }

            if (next == '/') {
                //Draw a circle at current position
                sd.drawCircle(currentX, currentY, 2);
            }

            if (next == '-') // Right rotation
                currentAngle += rotationAngle;

            if (next == '+') // Left rotation
                currentAngle -= rotationAngle;

            if (next == 'C') {
                // Color changing
                int colorNum = generation.at(i + 1) - 48;
                red = colors[colorNum][0];
                green = colors[colorNum][1];
                blue = colors[colorNum][2];
                sd.setColour(red, green, blue);
                i = i + 1;
            }

            if (next == '[') {

                // Save the state 

                struct turtleState currentState;
                currentState.x = currentX;
                currentState.y = currentY;

                currentState.branchLength = currentBranchLength;
                currentState.pointSize = currentPointSize;
                currentState.angle = currentAngle;

                currentState.red = red;
                currentState.green = green;
                currentState.blue = blue;
                stateStack.push(currentState);

                // Scale the point size and branch size according to the corresponding scale factors.
                currentBranchLength *= branchScaleFactor;
                currentPointSize *= pointScaleFactor;

                sd.setPointSize(currentPointSize);
            }

            if (next == ']') {

                // Restore the last saved state.

                struct turtleState newState = stateStack.top();

                currentX = newState.x;
                currentY = newState.y;

                currentAngle = newState.angle;
                currentBranchLength = newState.branchLength;
                currentPointSize = newState.pointSize;

                red = newState.red;
                green = newState.green;
                blue = newState.blue;

                // Set the new color and point size.
                sd.setColour(red, green, blue);
                sd.setPointSize(pointSize);

                stateStack.pop();
            }


        }

    }

};
#endif 
#ifndef CAR_H
#define CAR_H

#include <Angel.h>
#include <cmath>
#include <iostream>

class Car {
public:

// Constructor function for the car. Takes in the shaders variables and position and scale data and the thetas.
    Car(GLuint loc, GLuint faceColourLoc, GLuint modelLoc,
	vec4 pos, vec3 scale, GLfloat theta_x = 0, GLfloat theta_y = 0 , GLfloat theta_z = 0);  // Constructor

// Destructor function of the car
    ~Car();

// This function generates the vertices for each component of the car using switch statements 
    void init();

// This function draws the car and deals with the differnt scopes of transformations in the code
    void draw(vec4 pos, vec3 scale, GLfloat theta_x, GLfloat theta_y,
     GLfloat theta_z, mat4 globalTranslation ,mat4 wheelRotationModel); 
    
    GLuint vaoRectangle[5], bufferRectangle [5]; // Vector array and buffer for rectangle
    GLuint vaoSquare[2], bufferSquare[2];        // Vector array and buffer for square
    GLuint vaoBacklight[2], bufferBacklight[2];  // Vector array and buffer for Backlights(squares)
    GLuint vaoCylinder[3], bufferCylinder[3];    // Vector array and buffer for Cylinders

    vec4 pos; // holds data for the postion vec4 variable in main 
    vec3 scale; // holds data for the scale vec3 variable in main 
    GLuint loc_view, face_Colour_Loc, model_loc; // holds data for the shader values in the main 
    GLfloat theta_x, theta_y, theta_z; // holds data for the theta values in the main 
    const int numSegments = 36; // For the cylinders faces 
    const float PI = 3.14159265359f;
    vec4 top[36]; // vector for top circle in cylinder face
    vec4 bottom[36]; // vector for bottom circle in cylinder face
    vec4 side[72]; // For each pair of consecutive vertices on the top and bottom circle

private:
// Vertex data for the rectangle of the base and chasis and the windows 
   const vec4 rectangleVertices[8] = {
    vec4(0.0, 0.0, 0.5, 1.0),  // vertex 0
    vec4(2.0, 0.0, 0.5, 1.0),  // vertex 1
    vec4(2.0, 1.0, 0.5, 1.0),  // vertex 2
    vec4(0.0, 1.0, 0.5, 1.0),  // vertex 3
    vec4(0.0, 0.0, 0.0, 1.0),  // vertex 4
    vec4(2.0, 0.0, 0.0, 1.0),  // vertex 5
    vec4(2.0, 1.0, 0.0, 1.0),  // vertex 6
    vec4(0.0, 1.0, 0.0, 1.0)   // vertex 7
};

// Vertex data for the square used to make the front and back windows  
const vec4 squareVertices[4] = {
    vec4(0.0, 0.0, 0.0, 1.0),  // vertex 4
    vec4(2.0, 0.0, 0.0, 1.0),  // vertex 2
    vec4(2.0, 0.0, 0.5, 1.0),  // vertex 1
    vec4(0.0, 0.0, 0.5, 1.0),  // vertex 0
};

// Vertex data for the square used to make the back and front lights of the car
const vec4 backlightVertices[4] = {
   vec4(2.0, 0.0, 0.0, 1.0),  // vertex 5
   vec4(2.0, 1.0, 0.0, 1.0),  // vertex 6
   vec4(2.0, 1.0, 0.5, 1.0),  // vertex 2
   vec4(2.0, 0.0, 0.5, 1.0),  // vertex 1
};

// Color data used to make the base of the car grey
const vec4 colour[6] = {
    vec4(1, 0.5, 0.5, 1.0),  // gray (opaque)
    vec4(1, 0.5, 0.5, 1.0),  // gray (opaque)
    vec4(1, 0.5, 0.5, 1.0),  // gray (opaque)
    vec4(1, 0.5, 0.5, 1.0),  // gray (opaque)
    vec4(1, 0.5, 0.5, 1.0),  // gray (opaque)
    vec4(1, 0.5, 0.5, 1.0)   // gray (opaque)
};
// Color data used to make the Top rectangle of the car white
const vec4 colour2[6] = {
    vec4(1, 1, 1, 1.0),  // White
    vec4(1, 1, 1, 1.0),  // White
    vec4(1, 1, 1, 1.0),  // White
    vec4(1, 1, 1, 1.0),  // White
    vec4(1, 1, 1, 1.0),  // White
    vec4(1, 1, 1, 1.0)   // White
    };

// Color data used to make the windows of the car black
const vec4 colourSquare[2] = {
    vec4(0, 0, 0, 1.0),  // Black
    vec4(0, 0, 0, 1.0)   // Black
};
// Color data used to make the front lights yellow
const vec4 yellowLight[2] = {
    vec4(1, 1, 0, 1.0), // Yellow
    vec4(1, 1, 0, 1.0)  // Yellow
};

// Color data used to make the back lights red
const vec4 redLight[2] = {
    vec4(1, 0, 0, 1.0),  // Red
    vec4(1, 0, 0, 1.0)   // Red
};

// Color data used to make the wheels 
const vec4 colourCylinder[3] = {
  vec4(0,1,1,1), // Cyan
  vec4(1,0,1,1), // Magenta
  vec4(1,0,0,1)  // Red
};

// face data used to make the rectangle
const int face[6][4] = {
    {0, 1, 2, 3},  // front face
    {4, 0, 3, 7},  // bottom face
    {4, 5, 6, 7},  // back face
    {1, 5, 6, 2},  // top face
    {0, 1, 5, 4},  // left face
    {3, 2, 6, 7}   // right face
};

};
#endif  // CAR_H

#include "car.h"

// Constructor initialization
Car::Car(GLuint loc, GLuint faceColourLoc, GLuint modelLoc,
        vec4 pos, vec3 scale, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z) : loc_view{loc}, face_Colour_Loc{faceColourLoc}, model_loc{modelLoc}
{}

  Car::~Car() {
    glDeleteVertexArrays(6, vaoRectangle);
    glDeleteBuffers(6, bufferRectangle);
    glDeleteVertexArrays(3, vaoSquare);
    glDeleteBuffers(3, bufferSquare);
    glDeleteVertexArrays(3, vaoBacklight);
    glDeleteBuffers(3, bufferBacklight);
}

// Initialize OpenGL setup and shaders
void Car::init() {
// -------------------------------------------------------------------------------------------------------------------------------------------------
// Rectangle setup
// Create a vertex array object for each face
    glGenVertexArrays(6, vaoRectangle);
    glGenBuffers(6, bufferRectangle);

  for (int i = 0; i < 6; i++) {
    glBindVertexArray(vaoRectangle[i]);

    // initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, bufferRectangle[i]);
    vec4 prismVertex[6];
    int numVertices = 0;  // Track number of vertices per face

    switch(i) {
        case 0:
            // First rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[0];
            prismVertex[1] = rectangleVertices[1];
            prismVertex[2] = rectangleVertices[2];
            
            prismVertex[3] = rectangleVertices[2];
            prismVertex[4] = rectangleVertices[3];
            prismVertex[5] = rectangleVertices[0];
            break;
        
        case 1:
         // second rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[4];
            prismVertex[1] = rectangleVertices[0];
            prismVertex[2] = rectangleVertices[3];
            
            prismVertex[3] = rectangleVertices[3];
            prismVertex[4] = rectangleVertices[7];
            prismVertex[5] = rectangleVertices[4];
            break;
        
        case 2: // Third  rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[4];
            prismVertex[1] = rectangleVertices[5];
            prismVertex[2] = rectangleVertices[6];
            
            prismVertex[3] = rectangleVertices[6];
            prismVertex[4] = rectangleVertices[7];
            prismVertex[5] = rectangleVertices[4];
            break;
        
        case 3: // Fourth rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[1];
            prismVertex[1] = rectangleVertices[5];
            prismVertex[2] = rectangleVertices[6];
            
            prismVertex[3] = rectangleVertices[6];
            prismVertex[4] = rectangleVertices[2];
            prismVertex[5] = rectangleVertices[1];
            break;
        
        case 4:// Fifth rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[4];
            prismVertex[1] = rectangleVertices[5];
            prismVertex[2] = rectangleVertices[1];
            
            prismVertex[3] = rectangleVertices[1];
            prismVertex[4] = rectangleVertices[0];
            prismVertex[5] = rectangleVertices[4];
            break;
            
            
        case 5:// Sixth rectangle face (two triangle faces into a rectangle)
            prismVertex[0] = rectangleVertices[3];
            prismVertex[1] = rectangleVertices[2];
            prismVertex[2] = rectangleVertices[6];
            
            prismVertex[3] = rectangleVertices[6];
            prismVertex[4] = rectangleVertices[7];
            prismVertex[5] = rectangleVertices[3];
       	    break;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(prismVertex), prismVertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray( loc_view );
    glVertexAttribPointer( loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  }
    glBindVertexArray(0);  // Unbind after setting up


// -------------------------------------------------------------------------------------------------------------------------------------------------
// Square setup
// Create a vertex array object for each face
    glGenVertexArrays(3, vaoSquare);
    glGenBuffers(3, bufferSquare);

  for (int i = 0; i < 2; i++) {
    glBindVertexArray(vaoSquare[i]);

    // initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, bufferSquare[i]);
    vec4 squareVertex[3];
    int numVertices = 0;  // Track number of vertices per face

    switch(i) {
        case 0:
            // First rectangle face (two triangle faces into a rectangle)
            squareVertex[0] = squareVertices[0];
            squareVertex[1] = squareVertices[1];
            squareVertex[2] = squareVertices[2];
            break;

            case 1:
            // First rectangle face (two triangle faces into a rectangle)
            squareVertex[0] = squareVertices[2];
            squareVertex[1] = squareVertices[3];
            squareVertex[2] = squareVertices[0];
            break;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertex), squareVertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray( loc_view );
    glVertexAttribPointer( loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  }
// -------------------------------------------------------------------------------------------------------------------------------------------------
// Backlight setup
    glBindVertexArray(0);  // Unbind after setting up

// Create a vertex array object for each face
    glGenVertexArrays(3, vaoBacklight);
    glGenBuffers(3, bufferBacklight);

  for (int i = 0; i < 2; i++) {
    glBindVertexArray(vaoBacklight[i]);
// initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, bufferBacklight[i]);
    vec4 backlightVertex[3];
    int numVertices = 0;  // Track number of vertices per face

    switch(i) {
        case 0:
            // First rectangle face (two triangle faces into a rectangle)
            backlightVertex[0] = backlightVertices[0];
            backlightVertex[1] = backlightVertices[1];
            backlightVertex[2] = backlightVertices[2];
            break;

            case 1:
            // First rectangle face (two triangle faces into a rectangle)
            backlightVertex[0] = backlightVertices[2];
            backlightVertex[1] = backlightVertices[3];
            backlightVertex[2] = backlightVertices[0];
            break;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(backlightVertex), backlightVertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray( loc_view );
    glVertexAttribPointer( loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  }
// -------------------------------------------------------------------------------------------------------------------------------------------------
// Cylinder(Wheels) setup
// Create the top and bottom circles
    for (int i = 0; i < numSegments; i++) {
        float angleCylinder = 2 * PI * i / numSegments;
        top[i] = vec4(cos(angleCylinder), sin(angleCylinder), 0.5, 1.0);  // Top circle at z = 0.5
        bottom[i] = vec4(cos(angleCylinder), sin(angleCylinder), -0.5, 1.0); // Bottom circle at z = -0.5
    }

    // Create the side (connecting the top and bottom circles)
    for (int i = 0; i < numSegments; i++) {
        int next = (i + 1) % numSegments;
        // Two vertices for each side strip
        side[2*i] = top[i];
        side[2*i+1] = bottom[i];
    }
    glGenVertexArrays(3, vaoCylinder);
    glGenBuffers(3, bufferCylinder);

 // Top face
    glBindVertexArray(vaoCylinder[0]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[0]);
    glBufferData(GL_ARRAY_BUFFER, numSegments * sizeof(vec4), top, GL_STATIC_DRAW);
    glEnableVertexAttribArray(loc_view);
    glVertexAttribPointer(loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Bottom face
    glBindVertexArray(vaoCylinder[1]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[1]);
    glBufferData(GL_ARRAY_BUFFER, numSegments * sizeof(vec4), bottom, GL_STATIC_DRAW);
    glEnableVertexAttribArray(loc_view);
    glVertexAttribPointer(loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Side face
    glBindVertexArray(vaoCylinder[2]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * numSegments * sizeof(vec4), side, GL_STATIC_DRAW);
    glEnableVertexAttribArray(loc_view);
    glVertexAttribPointer(loc_view, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  
}


// Display the car in the scene
void Car::draw(vec4 pos, vec3 scale, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z, mat4 globalTransform, mat4 wheelRotationModel) {
int color_index = 0;
color_index = (color_index + 1) % 3;
mat4 model = globalTransform * Translate(pos) * Scale(scale);

//------------------------------------------------------------------------------------------------------------------------//
// Rectangle generation
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);

for (int i = 0; i < 6 ; i++) {
    glUniform4fv(face_Colour_Loc, 1, colour[i]);
    glBindVertexArray(vaoRectangle[i]);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Draw rectangular faces 
        }
// Now render the second, scaled-down rectangle
mat4 scaleMatrix = Scale(0.7, 0.5, 0.7);  // Scaling down by 25%
mat4 translateMatrix = Translate(0.5, 0.25, 0.5);  // Translate to top center of the first rectangle
  
// Combine scaling and translation
mat4 secondModel = model* translateMatrix * scaleMatrix;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, secondModel);

// Render the second rectangle (scaled and translated)
for (int i = 0; i < 6; i++) {
    glUniform4fv(face_Colour_Loc, 1, colour2[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoRectangle[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Draw the second rectangle
    }
//------------------------------------------------------------------------------------------------------------------------//
// Window rendering 
for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoSquare[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }
// Combine scaling and translation
mat4 secondModelSquare = model * translateMatrix * scaleMatrix;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, secondModelSquare);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoSquare[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }
// First Window 
// Combine scaling and translation
mat4 translateMatrixSquare = Translate(0.5, 0.23, 0.5);
mat4 translateMatrixWindow = Translate(0.1, 0, 0.1);
mat4 scaleMatrixSquare = Scale(0.6, 0.5, 0.4);  
mat4 TModelSquare = model * translateMatrixWindow * translateMatrixSquare * scaleMatrixSquare;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, TModelSquare);


for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoSquare[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }
// Second Window 
mat4 translateMatrixSquareTwo = Translate(0.5, 0.76, 0.5);
mat4 translateMatrixWindowTwo = Translate(0.1, 0, 0.1);
mat4 scaleMatrixSquareTwo = Scale(0.6, 0.5, 0.4);  
mat4 TModelSquareTwo = model * translateMatrixWindowTwo * translateMatrixSquareTwo * scaleMatrixSquareTwo;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, TModelSquareTwo);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoSquare[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }
//-----------------------------------------------------------------------------------------------------------------------//
// Back right redlight
mat4 translateMatrixBacklight = Translate(1.84, 0.75, 0.3);
mat4 scaleMatrixBacklight = Scale(0.1, 0.1, 0.1);  // Scaling down by 25%
mat4 ModeBacklight = model * translateMatrixBacklight * scaleMatrixBacklight;  // Apply both transformations
// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeBacklight);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, redLight[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }

// Back left redlight
mat4 translateMatrixBacklightTwo = Translate(1.84, 0.18, 0.3);
mat4 scaleMatrixBacklightTwo = Scale(0.1, 0.1, 0.1);  // Scaling down by 25%
mat4 ModeBacklightTwo = model * translateMatrixBacklightTwo * scaleMatrixBacklightTwo;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeBacklightTwo);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, redLight[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }

// Front right YellowLight
      mat4 translateMatrixBacklightThree = Translate(-0.21, 0.18, 0.3);
     mat4 scaleMatrixBacklightThree = Scale(0.1, 0.1, 0.1);  // Scaling down by 25%

  mat4 ModeBacklightThree = model * translateMatrixBacklightThree * scaleMatrixBacklightThree;  // Apply both transformations
// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeBacklightThree);
    
for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, yellowLight[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }

// Front Left YellowLight
mat4 translateMatrixBacklightFour = Translate(-0.21, 0.75, 0.3);
mat4 scaleMatrixBacklightFour = Scale(0.1, 0.1, 0.1);  // Scaling down by 25%
mat4 ModeBacklightFour = model * translateMatrixBacklightFour * scaleMatrixBacklightFour;  // Apply both transformations

// Send the new model matrix for the second rectangle to the shader
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeBacklightFour);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, yellowLight[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }



mat4 scaleMatrixFrontWindow = Scale(0.4, 0.3, 0.4);;  // Scaling down by 25%
mat4 translateMatrixFrontWindow = Translate(1.12, 0.35, 0.6);  // Translate to top center of the first rectangle
mat4 ModeFrontWindow = model * translateMatrixFrontWindow * scaleMatrixFrontWindow;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeFrontWindow);


for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }

mat4 scaleMatrixBackWindow = Scale(0.4, 0.3, 0.4);;  // Scaling down by 25%
mat4 translateMatrixBackWindow = Translate(-0.32, 0.35, 0.6);  // Translate to top center of the first rectangle
mat4 ModeBackWindow = model * translateMatrixBackWindow * scaleMatrixBackWindow;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModeBackWindow);

for (int i = 0; i < 2; i++) {
    glUniform4fv(face_Colour_Loc, 1, colourSquare[i]);  // Same colors for the second rectangle
    glBindVertexArray(vaoBacklight[i]);  // Bind the same VAO, assuming you use the same vertex data for both
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second rectangle
    }
//---------------------------------------------------------------------------------------------------------------------------
//Cylinder drawing 
mat4 scaleMatrixCylinder = Scale(0.16, 0.16, 0.16);;  // Scaling down by 25%
mat4 rotationMatrixCylinderY = RotateY(90);  // Translate to top center of the first rectangle
mat4 rotationMatrixCylinderZ = RotateZ(90);  // Translate to top center of the first rectangle
mat4 rotationMatrixCylinderx = RotateZ(0);  // Translate to top center of the first rectangle
mat4 translateMatrixCylinder = Translate(0.32, -0.05, 0.1);  // Translate to top center of the first rectangle
mat4 ModelCylinder=  model * translateMatrixCylinder *rotationMatrixCylinderx  *rotationMatrixCylinderZ * rotationMatrixCylinderY* wheelRotationModel *scaleMatrixCylinder;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModelCylinder);


 // Draw top face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[0]);
    glBindVertexArray(vaoCylinder[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw bottom face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[1]);
    glBindVertexArray(vaoCylinder[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw side face
for (int i = 0; i < 2 * numSegments - 2; i += 2) {
      color_index = (color_index + 1) % 3; // Cycle through colors

    // Set color for this strip segment
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[color_index]);

    // Bind VAO and draw the strip segment
    glBindVertexArray(vaoCylinder[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, i, 4); // Draw two triangles (quad)
}

mat4 translateMatrixCylindertwo = Translate(1.6, -0.05, 0.1);  // Translate to top center of the first rectangle
mat4 ModelCylindertwo = model * translateMatrixCylindertwo *rotationMatrixCylinderx  *rotationMatrixCylinderZ * rotationMatrixCylinderY* wheelRotationModel * scaleMatrixCylinder;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModelCylindertwo);


 // Draw top face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[0]);
    glBindVertexArray(vaoCylinder[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw bottom face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[1]);
    glBindVertexArray(vaoCylinder[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

   // Draw side face
for (int i = 0; i < 2 * numSegments - 2; i += 2) {
      color_index = (color_index + 1) % 3; // Cycle through colors

    // Set color for this strip segment
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[color_index]);

    // Bind VAO and draw the strip segment
    glBindVertexArray(vaoCylinder[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, i, 4); // Draw two triangles (quad)
}


mat4 translateMatrixCylinderThree = Translate(0.32, 1.05, 0.1);  // Translate to top center of the first rectangle
mat4 ModelCylinderThree = model * translateMatrixCylinderThree *rotationMatrixCylinderx  *rotationMatrixCylinderZ * rotationMatrixCylinderY* wheelRotationModel * scaleMatrixCylinder;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModelCylinderThree);


 // Draw top face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[0]);
    glBindVertexArray(vaoCylinder[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw bottom face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[1]);
    glBindVertexArray(vaoCylinder[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw side face
for (int i = 0; i < 2 * numSegments - 2; i += 2) {
      color_index = (color_index + 1) % 3; // Cycle through colors

    // Set color for this strip segment
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[color_index]);

    // Bind VAO and draw the strip segment
    glBindVertexArray(vaoCylinder[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, i, 4); // Draw two triangles (quad)
}


mat4 translateMatrixCylinderFour = Translate(1.6, 1.05, 0.1);  // Translate to top center of the first rectangle
mat4 ModelCylinderFour = model * translateMatrixCylinderFour *rotationMatrixCylinderx  *rotationMatrixCylinderZ * rotationMatrixCylinderY* wheelRotationModel * scaleMatrixCylinder;  // Apply both transformations

    glUniformMatrix4fv(model_loc, 1, GL_TRUE, ModelCylinderFour);


 // Draw top face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[0]);
    glBindVertexArray(vaoCylinder[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw bottom face
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[1]);
    glBindVertexArray(vaoCylinder[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    // Draw side face
for (int i = 0; i < 2 * numSegments - 2; i += 2) {
      color_index = (color_index + 1) % 3; // Cycle through colors

    // Set color for this strip segment
    glUniform4fv(face_Colour_Loc, 1, colourCylinder[color_index]);

    // Bind VAO and draw the strip segment
    glBindVertexArray(vaoCylinder[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, i, 4); // Draw two triangles (quad)
}
}

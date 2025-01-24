#include "building.h"
#include <random>
#include <cmath>
#include <vector>

Building::Building(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
        GLfloat scale_x, GLfloat scale_y, GLfloat scale_z) 
{}

// Triangular prism on Rectangular prism
triPrismHouse::triPrismHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
        GLfloat scale_x, GLfloat scale_y, GLfloat scale_z) 
        : Building(vertexLoc, faceLoc, modelLoc, pos, theta_x, theta_y, theta_z, scale_x, scale_y, scale_z) {


            vertex_loc = vertexLoc;
            face_loc = faceLoc;
            model_loc = modelLoc;
            std::random_device rd;  // Used to seed the generator
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> doubleDist(0.5, 3.0);  // Random height of bottom shape
            double height = doubleDist(gen);
        // Additional initialization specific to triPrismHouse can go here
        const vec4 rectPoint[8] = {
            vec4(-0.5, -0.5, 0.0, 1.0), vec4(0.5, -0.5, 0.0, 1.0),
            vec4(0.5, 0.5, 0.0, 1.0), vec4(-0.5, 0.5, 0.0, 1.0),
            vec4(-0.5, -0.5, height, 1.0), vec4(0.5, -0.5, height, 1.0),
            vec4(0.5, 0.5, height, 1.0), vec4(-0.5, 0.5, height, 1.0)
        };

        // Define the 6 faces of the rectangular prism
        // Each face is represented by 4 vertices (counter-clockwise winding)
        const int rectFace[6][4] = {
            {0,1,2,3},  // bot
            {4,5,6,7},  // top
            {0,1,5,4},  // back
            {2,3,7,6},  // front
            {2,6,5,1},  // left
            {0,4,7,3}   // right
        };

        const vec4 triPoint[6] = {
            vec4(-0.5, -0.5, -0.5, 1), vec4(0.5, -0.5, -0.5, 1),
            vec4(-0.5, 0.5, -0.5, 1),
            vec4(-0.5, -0.5, 0.5, 1), vec4(0.5, -0.5, 0.5, 1),
            vec4(-0.5, 0.5, 0.5, 1), 
        };

        const vec3 triColour[5] = {
            vec3(1,0,0), // red
            vec3(1,1,0), // yellow
            vec3(0,1,0), // green
            vec3(0,1,1), // cyan
            vec3(0,0,1) // blue
        };

        const int triFace[5][4] = {
            {0,2,1,-1},  // bot      
            {3,4,5,-1},  // top
            {5,4,1,2},   // side 1
            {0,3,5,2},   // side 2
            {0,1,4,3}    // side 3
        };

        // Initialization of rectangular prism
        glGenVertexArrays(1, &rectVao);
        glGenBuffers(1, &rectBuffer);

        // Bind VAO and VBO
        glBindVertexArray(rectVao);
        glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectPoint), rectPoint, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vertex_loc);
        glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        // Bind element buffers
        glGenBuffers(6, rectEbo);
        for (int i = 0; i < 6; i++) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEbo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), rectFace[i], GL_STATIC_DRAW);
        }
        // Model matrix for rect prism
        rectModel = Translate(pos) * RotateX(-90) * RotateY(theta_y) * RotateZ(theta_z) * Scale(scale_x, scale_y, scale_z);


        // Similar for triangular prism but 5 faces
        glGenVertexArrays(5, triVao);
        glGenBuffers(5, triBuffer);
        
        for (int i = 0; i < 5; i++) {
            glBindVertexArray(triVao[i]);
        
            // initialize a buffer object
            glBindBuffer(GL_ARRAY_BUFFER, triBuffer[i]);

            GLfloat A[4*4];
            int size = 0;
            for (int j = 0; j < 4; j++) {
                if (triFace[i][j] >= 0) {
                    A[j*4] = triPoint[triFace[i][j]].x;
                    A[j*4+1] = triPoint[triFace[i][j]].y;
                    A[j*4+2] = triPoint[triFace[i][j]].z;
                    A[j*4+3] = triPoint[triFace[i][j]].w;
                    size++;
                }
            }
            glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLfloat), A, GL_STATIC_DRAW);

            glEnableVertexAttribArray( vertex_loc );
            glVertexAttribPointer( vertex_loc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                    BUFFER_OFFSET(0) );

            triModel = rectModel * Translate(0, 0, height + 0.5) * RotateX(90) * RotateY(90) * RotateZ(0);
        }

    }

    triPrismHouse::~triPrismHouse(){
        glDeleteVertexArrays(1, &rectVao);
        glDeleteBuffers(1, &rectBuffer);
        glDeleteVertexArrays(5, triVao);
        glDeleteBuffers(5, triBuffer);
    }

    // Draw function. Globel model is passed in for world translation
    void triPrismHouse::draw(mat4 globalModel) {

        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel * rectModel)); // Specify shape model matrix (rect prism)
        
        // Draw rectangular prism faces
        glBindVertexArray(rectVao);
        for (int i = 0; i < 6; i++) {
            GLfloat v = 0.75 * (i+1)/6.0;
            glUniform4fv(face_loc, 1, vec4(v * 0.25,v * 0.5,v * 0.25,1));  // Set color for the face
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEbo[i]);
            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
        }
        // Draw triangular prism faces
        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel * triModel));

        for (int i = 0; i < 5; i++) {
            GLfloat v = 0.75 * (i+1)/6.0;
            glUniform4fv(face_loc, 1, vec4(v,v *0.5,v * 0.25,1));
            glBindVertexArray(triVao[i]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (i < 2) ? 3 : 4);  // Different # of vertices on top/bottom vs side faces
        }

}

pyrHouse::pyrHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
        GLfloat scale_x, GLfloat scale_y, GLfloat scale_z) 
        : Building(vertexLoc, faceLoc, modelLoc, pos, theta_x, theta_y, theta_z, scale_x, scale_y, scale_z) {

            vertex_loc = vertexLoc;
            face_loc = faceLoc;
            model_loc = modelLoc;
            std::random_device rd;  // Used to seed the generator
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> doubleDist(0.5, 3.0);  // Random height for bottom object
            double height = doubleDist(gen);

        // Bottom shape (rectangular prism)
        const vec4 rectPoint[8] = {
            vec4(-0.5, -0.5, 0.0, 1.0), vec4(0.5, -0.5, 0.0, 1.0),
            vec4(0.5, 0.5, 0.0, 1.0), vec4(-0.5, 0.5, 0.0, 1.0),
            vec4(-0.5, -0.5, height, 1.0), vec4(0.5, -0.5, height, 1.0),
            vec4(0.5, 0.5, height, 1.0), vec4(-0.5, 0.5, height, 1.0)
        };

        // Define the 6 faces of the rectangular prism
        // Each face is represented by 4 vertices (counter-clockwise winding)
        const int rectFace[6][4] = {
            {0,1,2,3},  // bot
            {4,5,6,7},  // top
            {0,1,5,4},  // back
            {2,3,7,6},  // front
            {2,6,5,1},  // left
            {0,4,7,3}   // right
        };
        std::uniform_real_distribution<> doubleDis(0.5, 3.0);  // Random height for pyramid apex
        double topHeight = doubleDis(gen);

        const vec4 pyrPoint[5] = {
            vec4(0.0, 0.0, topHeight, 1.0), // apex
            vec4(-0.5, -0.5, 0, 1.0), vec4(0.5, -0.5, 0, 1.0),
            vec4(0.5, 0.5, 0, 1.0), vec4(-0.5, 0.5, 0, 1.0),
        };
        
        const GLuint pyrFace[4][3] = {
            {0, 1, 2},
            {0, 2, 3},
            {0, 3, 4},
            {0, 4, 1}
        };

        // Generate and bind rect prism VAO and VBO
        glGenVertexArrays(1, &rectVao);
        glGenBuffers(1, &rectBuffer);

        glBindVertexArray(rectVao);
        glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectPoint), rectPoint, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vertex_loc);
        glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        // Generate and bind element buffers
        glGenBuffers(6, rectEbo);
        for (int i = 0; i < 6; i++) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEbo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), rectFace[i], GL_STATIC_DRAW);
        }

        // Assign rectangular prism model matrix
        rectModel = Translate(pos) * RotateX(-90) * RotateY(theta_y) * RotateZ(theta_z) * Scale(scale_x, scale_y, scale_z);

        // Generate and bind pyramid prism VAO and VBO
        glGenVertexArrays(1, &pyrVao);
        glGenBuffers(1, &pyrBuffer);

        glBindVertexArray(pyrVao);
        glBindBuffer(GL_ARRAY_BUFFER, pyrBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyrPoint), pyrPoint, GL_STATIC_DRAW);
        glEnableVertexAttribArray(vertex_loc);
        glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        // Generate and bind element buffers
        glGenBuffers(4, pyrEbo);
        for (int i = 0; i < 4; i++) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyrEbo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyrFace[i]), pyrFace[i], GL_STATIC_DRAW);
        }
        // Pyramid model matrix
        pyrModel = rectModel * Translate(0,0, height);
    } 
    pyrHouse::~pyrHouse(){
        glDeleteVertexArrays(1, &rectVao);
        glDeleteBuffers(1, &rectBuffer);
        glDeleteVertexArrays(1, &pyrVao);
        glDeleteBuffers(1, &pyrBuffer);
    }
    void pyrHouse::draw(mat4 globalModel) {
        // Global model translates all buildings according to user driving
        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel * rectModel));
        
        // Draw rectangular prism faces
        glBindVertexArray(rectVao);
        for (int i = 0; i < 6; i++) {
            GLfloat v = 0.75 * (i+1)/6.0;
            glUniform4fv(face_loc, 1, vec4(v,v *0.3,v * 0.3,1));  // Set color for the face
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEbo[i]);
            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
        }

        // Draw pyramid  faces
        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel * pyrModel));
        glBindVertexArray(pyrVao);
        for (int i = 0; i < 4; ++i) {
            GLfloat v = 0.75 * (i+1)/6.0;
            glUniform4fv(face_loc, 1, vec4(v * 0.2,v * 0.2, v *0.5, 1.0));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyrEbo[i]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        }

}


const int CIRCUMFERENCE_POINTS = 20;  // Number of points to approximate circle
coneCylinderHouse::coneCylinderHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
                                     vec4 pos, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
                                     GLfloat scale_x, GLfloat scale_y, GLfloat scale_z)
    : Building(vertexLoc, faceLoc, modelLoc, pos, theta_x, theta_y, theta_z, scale_x, scale_y, scale_z),
      vertex_loc(vertexLoc), face_loc(faceLoc), model_loc(modelLoc) {
    // Random height and radius generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> heightDist(0.5, 3.0);
    std::uniform_real_distribution<> radiusDist(0.3, 0.7);

    double cylinderHeight = heightDist(gen);
    double cylinderRadius = radiusDist(gen);
    double coneHeight = heightDist(gen) * 0.5;
    double coneRadius = cylinderRadius;

    // Generate cylinder points
    for (int i = 0; i < numSegments; i++) {
        float angle = 2 * PI * i / numSegments;
        top[i] = vec4(cos(angle) * cylinderRadius, sin(angle) * cylinderRadius, cylinderHeight, 1.0);
        bottom[i] = vec4(cos(angle) * cylinderRadius, sin(angle) * cylinderRadius, 0.0, 1.0);
    }

    // Create sides (two vertices per segment)
    for (int i = 0; i < numSegments; i++) {
        side[2 * i] = top[i];
        side[2 * i + 1] = bottom[i];
    }

    // Create cone points
    conePoints.push_back(vec4(0.0, 0.0, cylinderHeight + coneHeight, 1.0));  // Apex
    for (int i = 0; i < numSegments; i++) {
        conePoints.push_back(top[i]);
    }

    // Generate VAOs and Buffers
    glGenVertexArrays(3, vaoCylinder);
    glGenBuffers(3, bufferCylinder);

    // Top circle
    glBindVertexArray(vaoCylinder[0]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Bottom circle
    glBindVertexArray(vaoCylinder[1]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Cylinder sides
    glBindVertexArray(vaoCylinder[2]);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(side), side, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Cone
    glGenVertexArrays(1, &coneVao);
    glGenBuffers(1, &coneBuffer);

    glBindVertexArray(coneVao);
    glBindBuffer(GL_ARRAY_BUFFER, coneBuffer);
    glBufferData(GL_ARRAY_BUFFER, conePoints.size() * sizeof(vec4), conePoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Initialize models
    cylinderModel = Translate(pos) * RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z) * Scale(scale_x, scale_y, scale_z);
    coneModel = cylinderModel;
}

    coneCylinderHouse::~coneCylinderHouse() {
        glDeleteVertexArrays(3, vaoCylinder);
        glDeleteBuffers(3, bufferCylinder);
        glDeleteVertexArrays(1, &coneVao);
        glDeleteBuffers(1, &coneBuffer);
    }

    void coneCylinderHouse::draw(mat4 globalModel) {
        // Draw Cylinder Bottom
        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel* cylinderModel * RotateX(270)));
        // Bottom face
        glBindVertexArray(vaoCylinder[0]);
        glUniform4fv(face_loc, 1, vec4(0.3,0.1,0.6,1));  // Random colours    
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

        // Top face
        glBindVertexArray(vaoCylinder[1]);
        glUniform4fv(face_loc, 1, vec4(0.3,0.1,0.1,1));  // No rhyme nor reason
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

        // Sides
        glBindVertexArray(vaoCylinder[2]);
        glUniform4fv(face_loc, 1, vec4(0.3,0.2,0.4,1)); // Purple is cool
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * numSegments);

        // Draw Cone
        glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel* coneModel * RotateX(270)));
        glBindVertexArray(coneVao);

        // Cone base
        glUniform4fv(face_loc, 1, vec4(0.3,0.1,0.1,1));  
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);

        // Cone sides
        glUniform4fv(face_loc, 1, vec4(0.3,0.1,0.1,1)); // Maroon
        glDrawArrays(GL_TRIANGLE_STRIP, 1, numSegments);
    }
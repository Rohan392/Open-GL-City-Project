#include <Angel.h>
#include <iostream>
#include <random>
#include "camera.h"
#include "building.h"
#include "car.h"
#include "light.h"

using namespace std;
//----------------------------------------------------------------------------
GLint normalMatLoc, normalVecLoc;

Camera *camera;
Car* car; 
//---------------------------------------------------------------------------
// Object model globals
GLfloat theta_x = 0.0, theta_y = 0.0, theta_z = 0.0;
GLfloat plane_theta_x = 0.0, plane_theta_y = 0.0, plane_theta_z = 0.0;
GLfloat plane_scale_x = 1, plane_scale_y = 1, plane_scale_z = 1;
GLfloat car_theta_x = 90, car_theta_y = 180, car_theta_z = 0.0;


vec4 pos(0.0, 0.0, 0.0, 0.0); // global reference
vec4 carPos(0.5, 0.0, 0.5, 0.0); // Car starting point / location

// Used to track car location for turn limiting
float x_offset = 0.5; 
float z_offset = 0.5;

vec4 leftView(0.5, 0.5, 1, 1); // Default view eyes
vec4 rightView(0.5, 0.5, -0.5, 1);
vec4 upView(-0.5, 0.5, 0.5, 1);
vec4 downView(1, 0.5, 0.5, 1);

vec4 relativeUp(0, 1, 0, 0); // Default view up
vec4 thirdPersonAt(carPos.x, 0.09, carPos.z, 1); // Default view at
vec4 forwardDir(0, 0, 1, 1); // Starting forward vector

// Set defaults
vec4* currentView = &upView;
vec4* currentUp = &relativeUp;
vec4* currentAt = &thirdPersonAt; 
vec4* currentForward = &forwardDir;

int currentViewMode = 1;

// Global movement step
GLfloat movementIncrement = 0.1;

// More object globals
vec3 buildingScaleFactor(0.1, 0.1, 0.1);
vec3 planeScaleFactor(1.0, 1.0, 1.0);
vec3 scale(1.0, 1.0, 1.0);
vec3 scaleCar(1.0, 1.0, 1.0); // The inital scale of the triangle prism

GLint faceColourLoc, modelLoc, viewLoc, projLoc;
GLuint planeVao[2], planeBuffer[2]; // City plane

const int numStreets = 10;
const float tileWidth = 0.2f;
const int buildingSpacing = 3; // Every 3 tiles place a road
const int gridSize = (numStreets * buildingSpacing);

int grid[gridSize][gridSize]; 

Building*  buildGrid[gridSize][gridSize]; // Container for buildings
vector<Light*> lights; // Container for lights

mat4 globalModel;
mat4 carGlobalModel;
mat4 wheelRotationModel;
mat4 planeModel;

// Plane characteristics
const vec4 planePoint[6] = {
    vec4(0.0, -0.01, 0.0, 1.0), vec4(6.0, -0.01, 0.0, 1.0),
    vec4(6.0, -0.01, 6.0, 1.0),   vec4(0.0, -0.01, 6.0, 1.0),
};

const int planeFace[2][4] = {
    {0, 1, 2, 3}, // Top face - using first triangle vertices
    {3, 2, 5, 0}  // Bottom face - using second triangle vertices
};

const vec4 planeColor = vec4(0.2, 0.2, 0.2, 1); // Dark gray


void init()
{
  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);
  faceColourLoc = glGetUniformLocation(program, "uFaceColour");
  modelLoc = glGetUniformLocation(program, "uModel");
  viewLoc = glGetUniformLocation(program, "uView");
  projLoc = glGetUniformLocation(program, "uProjection");

  // Initialize the vertex position attribute from the vertex shader
  GLuint vloc = glGetAttribLocation( program, "vPosition" );
 
  // Create car and initialize attributes
  car = new Car(vloc, faceColourLoc, modelLoc, carPos, scaleCar);
  car->init();


  std::random_device rd;  // Used to seed building choice
  std::mt19937 gen(rd());  

  std::uniform_int_distribution<> intDist(1, 3); // 3 types of buildings

  // Create grid
  for (int i = 0; i < gridSize; i++){
    for (int j = 0; j < gridSize; j++){
      if (((i + 1) % 3 == 0) || ((j + 1) % 3 == 0)){ // Street

        grid[i][j] = 1;
        buildGrid[i][j] = nullptr;

        // Place lights
        if (((i + 1) % 3 == 0) && ((j + 1) % 3 == 0) && ((i / 3 + j / 3) % 2 == 0)) {// selects every other intersection
          float x = i * tileWidth;
          float z = j * tileWidth;
          float lightScale = 0.02; // scale for light

          // Place traffic lights at the four corners of the intersection, and rotate them
          lights.push_back(new Light(vloc, faceColourLoc, modelLoc,
              vec4(x, 0, z, 1.0), -90, 0, 0, lightScale, lightScale, lightScale)); // Bottom-left corner
          lights.push_back(new Light(vloc, faceColourLoc, modelLoc,
              vec4(x + tileWidth, 0.0, z, 1.0), -90, 0, -90, lightScale, lightScale, lightScale)); // Bottom-right corner
          lights.push_back(new Light(vloc, faceColourLoc, modelLoc,
              vec4(x, 0.0, z + tileWidth, 1.0), -90, 0, 90, lightScale, lightScale, lightScale)); // Top-left corner
          lights.push_back(new Light(vloc, faceColourLoc, modelLoc,
              vec4(x + tileWidth, 0.0, z + tileWidth, 1.0), -90, 0, 180, lightScale, lightScale, lightScale)); // Top-right corner
        }

      } else{ // Buildings
        grid[i][j] = 0;

        int randomInt = intDist(gen);

        float x = (i * tileWidth);
        float z = (j * tileWidth);
        // Randomly select building for this tile
        switch(randomInt){
          case 1: 
            buildGrid[i][j] = new pyrHouse(vloc, faceColourLoc, modelLoc, vec4(x + (tileWidth / 2), 0.0, z + (tileWidth / 2), 0.0), 
            0, 0, 0, buildingScaleFactor.x, buildingScaleFactor.y, buildingScaleFactor.z);
            break;
          case 2: 
            buildGrid[i][j] = new triPrismHouse(vloc, faceColourLoc, modelLoc, vec4(x + (tileWidth / 2), 0.0, z + (tileWidth / 2), 0.0), 
            0, 0, 0, buildingScaleFactor.x, buildingScaleFactor.y, buildingScaleFactor.z);
            break;
          case 3: 
            buildGrid[i][j] = new coneCylinderHouse(vloc, faceColourLoc, modelLoc, vec4(x + (tileWidth / 2), 0.0, z + (tileWidth / 2), 0.0), 
            0, 0, 0, buildingScaleFactor.x, buildingScaleFactor.y, buildingScaleFactor.z);
            break;

        }
      }
    }
  }

  // Create camera
  vec4 eye(-0.5, 0.5, 0.5, 1);
  vec4 at(carPos.x, 0.09, carPos.z, 1);
  camera = new Camera(viewLoc, projLoc, eye, at, relativeUp, -1.0, 1.0, -1.0, 1.0, -0.5, 2);


  // Initialize plane characteristics

  glGenVertexArrays(2, planeVao);
  glGenBuffers(2, planeBuffer);

  for (int i = 0; i < 1; i++) {
    // Bind the VAO for the current face
    glBindVertexArray(planeVao[i]);
    glBindBuffer(GL_ARRAY_BUFFER, planeBuffer[i]);
  
    GLfloat A[4 * 4]; // Temporary array for attributes
    int size = 0;

    for (int j = 0; j < 4; j++) {
        if (planeFace[i][j] >= 0) { // Check if the face index is valid
          A[j * 4] = planePoint[planeFace[i][j]].x;     // x
          A[j * 4 + 1] = planePoint[planeFace[i][j]].y; // y
          A[j * 4 + 2] = planePoint[planeFace[i][j]].z; // z
          A[j * 4 + 3] = planePoint[planeFace[i][j]].w; // w
          size++;
      }
    }

    glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(GLfloat), A, GL_STATIC_DRAW);

    // Define vertex attributes 
    glEnableVertexAttribArray(vloc);
    glVertexAttribPointer(vloc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
  }

  // Initialize plane model matrix
  planeModel = Translate(vec4(0.0, 0.0, 0.0, 0.0)) * RotateX(plane_theta_x) * RotateY(plane_theta_y) * RotateZ(plane_theta_z) *
        Scale(plane_scale_x, plane_scale_y, plane_scale_z);
  
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background

  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
}


//----------------------------------------------------------------------------


void drawPlane(mat4 model) {
    // Set uniforms
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (model * planeModel));

    for(int i = 0; i < 2; i < i++){
      glUniform4fv(faceColourLoc, 1, planeColor); // Set face colour
      glBindVertexArray(planeVao[i]); 
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Render 2 triangles
    }
    
}

// Function for cycling traffic light colour
void timer(int val)
{
  for (Light* light : lights) {
    light->next_colour();
  }
  glutPostRedisplay();
  glutTimerFunc(3000, timer, 0);
}


// Main drawing function

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

    camera->sendToShader(); // Camera call to handle viewing

    drawPlane(globalModel); // Draw city plane

    for (int i = 0; i < gridSize; i++) {
      for (int j = 0; j < gridSize; j++) {
        if (grid[i][j] == 0){
          buildGrid[i][j]->draw(globalModel); // Polymorphic building draw
        }
      }
    }

    for (Light* light : lights) {
        light->draw(globalModel); // Intersection lights
    }

    mat4 carModel = carGlobalModel * Translate(carPos)* RotateX(car_theta_x)*RotateY(car_theta_y) *RotateZ(car_theta_z) *Scale(0.05, 0.05, 0.05);
    car->draw(pos, scale, car_theta_x, car_theta_y, car_theta_z, carModel,wheelRotationModel); // Create car

    glutSwapBuffers();
}

// Ensure turning is only done at intersections
bool validPos(float x_coord, float z_coord) {

    int x = static_cast<int>(x_coord * 100); // Have to cast and multiply because fmod destroys everything
    int z = static_cast<int>(z_coord * 100); // floats suck

    int upper = static_cast<int>(0.6 * 100);
    int lower = static_cast<int>(0.4 * 100);

    if ((x % upper) > lower){ // X street check
      if ((z % upper) > lower){ // Z treet check
        return 1;
      }
    }
    return 0;
}

GLfloat radians(GLfloat degrees) {
    return (degrees * M_PI) / 180.0; // Convert degrees to radians
}

int getForwardDir(){
  GLfloat angle_rad = radians(-car_theta_z); // Convert current z-axis rotation to radians

  // Calculate forward vector - direction of global model travel/translation
  vec4 forward(std::round(-cos(angle_rad)), 0, std::round(-sin(angle_rad)) , 0); // Calculate forward vector

  currentForward = &forward;

  if(forward.x == -1){
    return 1; // Up
  } else if(forward.x == 1){
    return 2; // Down
  } if(forward.z == 1){
    return 3; // Left
  } else if(forward.z == -1){
    return 4; // Right
  }
  return 0;
}

void updateForwardVec(){
  GLfloat angle_rad = radians(-car_theta_z); // Convert current y-axis rotation to radians
  vec4 forward(std::round(-cos(angle_rad)), 0, std::round(-sin(angle_rad)) , 0);

  currentForward = &forward;
}
//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
  switch ( key ) {
  case 033:
    exit( EXIT_SUCCESS );
    break;
  }
  glutPostRedisplay();
}


void arrow(int key, int x, int y)
{
  switch (key) {
    // Left 90* turn
    case GLUT_KEY_LEFT:{
        bool isValid = validPos(x_offset, z_offset);
        if (isValid){
          car_theta_z += 90;
          int forwardDir = getForwardDir();
          
          switch(forwardDir){ // Determine eye for new view
            case 1:
              currentView = &upView;
              break;
            case 2: 
              currentView = &downView;
              break;
            case 3: 
              currentView = &leftView;
              break;
            case 4: 
              currentView = &rightView;
              break;
          }
          camera->moveEye(*currentView);

          if (currentViewMode == 4){ // Determine at for first person perspective
            if((*currentForward).x != 0){
              vec4 newAt(-((*currentForward).x), 0.1, 0.5, 0); // Reverse of global (building, plane & light) travel
              currentAt = &newAt;
            } else if((*currentForward).z != 0){
              vec4 newAt(0.5, 0.1, -((*currentForward).z), 0);
              currentAt = &newAt;
            }
            camera->look(*currentAt);
          }
        }
        break;
    }
    // Same thing but right
    case GLUT_KEY_RIGHT:{
        bool isValid = validPos(x_offset, z_offset);
        if (isValid){
            car_theta_z -= 90;
          int forwardDir = getForwardDir();
          switch(forwardDir){
            case 1:
              currentView = &upView;
              break;
            case 2: 
              currentView = &downView;
              break;
            case 3: 
              currentView = &leftView;
              break;
            case 4: 
              currentView = &rightView;
              break;
          }
          camera->moveEye(*currentView);
          if (currentViewMode == 4){
            if((*currentForward).x != 0){
              vec4 newAt(-((*currentForward).x), 0.1, 0.5, 0);
              currentAt = &newAt;
            } else if((*currentForward).z != 0){
              vec4 newAt(0.5, 0.1, -((*currentForward).z), 0);
              currentAt = &newAt;
            }
            camera->look(*currentAt);
          }
        
        }
        break;
    }
    // Driving forward
    case GLUT_KEY_UP:{
      int forwardDir = getForwardDir();
      if ((x_offset < 0.1 || x_offset > 6.0 || z_offset < 0.1 || z_offset > 6.0)){
        if (x_offset < 0.1){
          x_offset += movementIncrement;
        } else if (x_offset > 6.0){
          x_offset -= movementIncrement;
        } else if (z_offset < 0.1){
          z_offset += movementIncrement;
        } else if (z_offset > 6.0){
          z_offset -= movementIncrement;
        }
        globalModel = Translate((-movementIncrement * (*currentForward).x), 0, (-movementIncrement * (*currentForward).z)) * globalModel;
        break;
      } 

      updateForwardVec();
      // Update internal tracking
      switch(forwardDir){
          case 1:
            x_offset = x_offset + movementIncrement;
            break;
          case 2:
            x_offset = x_offset - movementIncrement;
            break;
          case 3:
            z_offset = z_offset - movementIncrement;
            break;
          case 4:
            z_offset = z_offset + movementIncrement;
            break;
        }
        // Forward vector holds direction, define step/increment
        globalModel = Translate((movementIncrement * (*currentForward).x), 0, (movementIncrement * (*currentForward).z)) * globalModel;
        wheelRotationModel = RotateZ(10) * wheelRotationModel; // Rotate wheel models
        break;
      
    }
    // Driving backward
    case GLUT_KEY_DOWN:{
      int forwardDir = getForwardDir();
      if ((x_offset < 0.1 || x_offset > 6.0 || z_offset < 0.1 || z_offset > 6.0)){
        if (x_offset < 0.1){
          x_offset += movementIncrement;
        } else if (x_offset > 6.0){
          x_offset -= movementIncrement;
        } else if (z_offset < 0.1){
          z_offset += movementIncrement;
        } else if (z_offset > 6.0){
          z_offset -= movementIncrement;
        }
        globalModel = Translate((movementIncrement * (*currentForward).x), 0, (movementIncrement * (*currentForward).z)) * globalModel;
        break;
      } 
      updateForwardVec();

      switch(forwardDir){ // Opposite of forward
          case 1:
            x_offset = x_offset - movementIncrement;
            break;
          case 2:
            x_offset = x_offset + movementIncrement;
            break;
          case 3:
            z_offset = z_offset + movementIncrement;
            break;
          case 4:
            z_offset = z_offset - movementIncrement;
            break;
      }
      globalModel = Translate((-movementIncrement * (*currentForward).x), 0, (-movementIncrement * (*currentForward).z)) * globalModel;
      break;

    }
    case GLUT_KEY_F1:{ // Default view
        // Restore default Up/At if coming from F2 or F4
        currentViewMode = 1;
        currentUp = &relativeUp;
        camera->changeUp(*currentUp);
        currentAt = &thirdPersonAt;
        camera->look(*currentAt);

        int forwardDir = getForwardDir();
        
        // Eye positions depening on forward direction
        upView = vec4(-0.5, 0.5, 0.5, 1);
        downView = vec4(1.5, 0.5, 0.5, 1);
        leftView = vec4(0.5, 0.5, 1.5, 1);
        rightView = vec4(0.5, 0.5, -0.5, 1);

        switch(forwardDir){
          case 1:
            currentView = &upView;
            break;
          case 2:
            currentView = &downView;
            break;
          case 3:
            currentView = &leftView;
            break;
          case 4:
            currentView = &rightView;
            break;
        }
        
        camera->moveEye(*currentView); // Self explanatory
        break;
    }
    // Overhead view
    case GLUT_KEY_F2:{
        currentViewMode = 2;

        currentAt = &thirdPersonAt;
        camera->look(*currentAt);
        // Eye will remain in place, but 
        upView = downView = rightView = leftView = vec4(0.5, 1, 0.5, 1);
        // the up vector has to be in line with the forward direction
        updateForwardVec();
        vec4 newUp = (*currentForward * -1);
        currentUp = &newUp;


        camera->changeUp(*currentUp);
        camera->moveEye(vec4(0.5, 1, 0.5, 1));
        break;
      }
    // Side view
    case GLUT_KEY_F3:{
        
        currentViewMode = 3;

        currentUp = &relativeUp;
        camera->changeUp(*currentUp);
        currentAt = &thirdPersonAt;
        camera->look(*currentAt);

        upView = vec4(0.5, 0.2, 1, 1); // Only different thing from F1 are these
        downView = vec4(0.5, 0.2, 0, 1);
        rightView = vec4(0, 0.2, 0.5, 1);
        leftView = vec4(1, 0.2, 0.5, 1);

        int forwardDir = getForwardDir();

        switch(forwardDir){
          
          case 1:
            currentView = &upView;
            break;
          case 2:
            currentView = &downView;
            break;
          case 3:
            currentView = &leftView;
            break;
          case 4:
            currentView = &rightView;
            break;
        }
        
        camera->moveEye(*currentView);
        
        break;
    }
    // First person view
    case GLUT_KEY_F4:{ 
        
        currentViewMode = 4;
        currentUp = &relativeUp;
        camera->changeUp(*currentUp);

        // Similar to F1 the eye will remain the same
        upView = downView = rightView = leftView = vec3(0.5, 0.1, 0.5);
        
        // But this time the at vector is forward
        updateForwardVec();

        // Forward vector decides at direction
        if((*currentForward).x != 0){
          vec4 newAt(-((*currentForward).x), 0.1, 0.5, 0); // Reverse of world move direction
          currentAt = &newAt;
        } else if((*currentForward).z != 0){
          vec4 newAt(0.5, 0.1, -((*currentForward).z), 0);
          currentAt = &newAt;
        }
        
        camera->look(*currentAt);
        camera->moveEye(*currentView);
        break;
    }
  
  }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);

  // If you are using freeglut, the next two lines will check if 
  // the code is truly 3.2. Otherwise, comment them out
    
  glutInitContextVersion(3, 2);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutCreateWindow("Triangular Prism");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow);
  glutTimerFunc(3000, timer, 0);
  
  glutMainLoop();

  for (Light* light : lights) {
      delete light;
  }
  lights.clear();


  return 0;
}

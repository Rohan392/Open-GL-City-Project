#include <Angel.h>

#ifndef CAMERA_H
#define CAMERA_H

using namespace std;

class Camera
{
public:
// Camera object constructor 
  Camera(GLuint viewLoc, GLuint projLoc,
	 vec4 eye, vec4 at, vec4 up,
	 int oleft, int oright, int obot, int otop, int oznear,
	 int ozfar);
  
// Camera object destructor 
  ~Camera();

// Changes the at variable to a  position
  void look(vec4 newAt);

// Changes the eye variable to a postion
  void moveEye(vec4 newEye);

// changes the up variable to a new postion
  void changeUp(vec4 newUp);

// sends the lookat matrix to the shaders 
  void sendToShader() const;
  

private: 
  GLuint view_loc, proj_loc; // shader variables
  vec4 eye_pos, at_pos, up_dir; // Lookat variables 
  mat4 proj_mat; // projection matrix 
  
};

#endif

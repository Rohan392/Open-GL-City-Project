#include "camera.h"
// Constructor for an Camera variable 
Camera::Camera(GLuint viewLoc, GLuint projLoc,
	       vec4 eye, vec4 at, vec4 up,
	       int oleft, int oright, int obot, int otop, int oznear,
	       int ozfar)
  : view_loc(viewLoc), proj_loc(projLoc), eye_pos(eye), at_pos(at), up_dir(up) // intialization of the variables 
{
  proj_mat = Perspective(45.0, 1.0, 0.1, 10.0); // intialization of the projection matrix 
}
// camera destructor 
Camera::~Camera()
{

}
// Function reinitalizes the eye variables in the xyz plane
void Camera::moveEye(vec4 newEye)
{
  eye_pos.x = newEye.x;
  eye_pos.y = newEye.y;
  eye_pos.z = newEye.z;

}

// Function reinitalizes the vup variable
void Camera::changeUp(vec4 newUp){
  up_dir = newUp;
}

// Function reinitalizes the at variable in the xyz plane 
void Camera::look(vec4 newAt)
{
  at_pos.x = newAt.x;
  at_pos.y = newAt.y;
  at_pos.z = newAt.z;
}

// Function sends view and projection matrix to the shader and the lookat matrix to the shader
void Camera::sendToShader() const
{
  mat4 view_mat = LookAt(eye_pos, at_pos, up_dir);
  glUniformMatrix4fv(view_loc, 1, GL_TRUE, view_mat);
  glUniformMatrix4fv(proj_loc, 1, GL_TRUE, proj_mat);
}

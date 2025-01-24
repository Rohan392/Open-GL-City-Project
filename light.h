#include <Angel.h>

#ifndef LIGHT_H
#define LIGHT_H

using namespace std;

class Light
{
public:
  Light(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
	vec4 pos, GLfloat theta_x = 0, GLfloat theta_y = 0, GLfloat theta_z = 0,
	GLfloat scale_x = 1, GLfloat scale_y = 1, GLfloat scale_z = 1);
  ~Light();

  void next_colour();
  void draw(mat4 globalModel);
  

private:
  GLuint vao[2], buffer[2], ebo[12];
  mat4 model;
  int color_index;
  GLuint vertex_loc, face_loc, model_loc;
};

#endif

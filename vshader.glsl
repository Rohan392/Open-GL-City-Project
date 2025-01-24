#version 150
in vec4 vPosition;
// uniform means this is the same value for all vertices
uniform vec4 uFaceColour;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
out vec4 vColour;
void main()
{
    gl_Position = uProjection * uView * uModel * vPosition;
  vColour = uFaceColour;
}

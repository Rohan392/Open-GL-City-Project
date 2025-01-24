#include "light.h"

Light::Light(GLuint vertexLoc, GLuint faceLoc,
             GLuint modelLoc, vec4 pos,
             GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
             GLfloat scale_x, GLfloat scale_y, GLfloat scale_z)
    : color_index{0}, vertex_loc{vertexLoc}, face_loc{faceLoc},
      model_loc{modelLoc} {
    //all points for the light and pole
    const vec4 point[16] = {
        vec4(-0.5, -0.5, 4.0, 1.0), vec4(0.5, -0.5, 4.0, 1.0),
        vec4(0.5, 0.5, 4.0, 1.0),   vec4(-0.5, 0.5, 4.0, 1.0),
        vec4(-0.5, -0.5, 6.5, 1.0), vec4(0.5, -0.5, 6.5, 1.0),
        vec4(0.5, 0.5, 6.5, 1.0),   vec4(-0.5, 0.5, 6.5, 1.0),
        vec4(-0.1, -0.1, 0.0, 1.0), vec4(0.1, -0.1, 0.0, 1.0),
        vec4(0.1, 0.1, 0.0, 1.0),   vec4(-0.1, 0.1, 0.0, 1.0),
        vec4(-0.1, -0.1, 4.0, 1.0), vec4(0.1, -0.1, 4.0, 1.0),
        vec4(0.1, 0.1, 4.0, 1.0),   vec4(-0.1, 0.1, 4.0, 1.0)};

    //all the faces for the light and pole organized
    const GLuint face[12][4] = {
        {0, 1, 2, 3},    {4, 5, 6, 7},    {0, 1, 5, 4},
        {2, 3, 7, 6},    {2, 6, 5, 1},    {0, 4, 7, 3},
        {8, 9, 10, 11},  {12, 13, 14, 15}, {8, 9, 13, 12},
        {10, 11, 15, 14}, {10, 14, 13, 9}, {8, 12, 15, 11}};

    //generate all the vertex arrays and buffers needed to generate the shapes
    glGenVertexArrays(2, vao);
    glGenBuffers(2, buffer);
    glGenBuffers(12, ebo);

    // Setup VAO 0 (Light and Pole)
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //iterate through all faces
    for (int i = 0; i < 12; i++) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), face[i], GL_STATIC_DRAW);
    }

    // Setup VAO 1 (Rectangular Fan Geometry)
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    vec4 A[4] = {
        vec4(-0.25, -0.51, 6, 1), vec4(0.25, -0.51, 6, 1),
        vec4(0.25, -0.51, 5.0, 1), vec4(-0.25, -0.51, 5.0, 1)};
    glBufferData(GL_ARRAY_BUFFER, sizeof(A), A, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    model = Translate(pos) * RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z) *
            Scale(scale_x, scale_y, scale_z);
}


Light::~Light()
{
  glDeleteVertexArrays(2, vao);
  glDeleteBuffers(2, buffer);
  glDeleteBuffers(12, ebo);
}
//allows to flip between light colours
void Light::next_colour()
{
  color_index = (color_index + 1) % 3;
}
//draw function for whole model
void Light::draw(mat4 globalModel) {
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, (globalModel * model));

    // Draw Light and Pole
    glBindVertexArray(vao[0]);
    for (int i = 0; i < 12; i++) {
        if(i < 6) {
          GLfloat v = 0.75 * ((i % 6) + 1) / 6.0; // Adjust color for light and pole
          glUniform4fv(face_loc, 1, vec4(v, v, v, 1));
        } else {
          glUniform4fv(face_loc, 1, vec4(0.0, 0.0, 0.0, 1));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
    }

    // Draw Rectangular Fan Geometry
    vec4 lightColor[3] = {vec4(1, 0, 0, 1), vec4(1, 1, 0, 1), vec4(0, 1, 0, 1)};
    glUniform4fv(face_loc, 1, lightColor[color_index]);
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}




#ifndef BUILDING_H
#define BUILDING_H

#include <Angel.h>
#include <vector>


class Building{
    public:
        Building(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x = 0, GLfloat theta_y = 0, GLfloat theta_z = 0,
        GLfloat scale_x = 1, GLfloat scale_y = 1, GLfloat scale_z = 1);
        virtual ~Building() = default;
        virtual void draw(mat4 globalModel) = 0;
};
class triPrismHouse : public Building
{
    public:
        triPrismHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x = 0, GLfloat theta_y = 0, GLfloat theta_z = 0,
        GLfloat scale_x = 1, GLfloat scale_y = 1, GLfloat scale_z = 1);
        ~triPrismHouse() override;
        void draw(mat4 globalModel) override;
        
    private:
        GLuint triVao[5], triBuffer[5], triEbo[5];
        GLuint rectVao, rectBuffer, rectEbo[6];
        const vec3 rectColour[6];
        const vec3 triColour[5];
        mat4 rectModel, triModel;
        GLuint vertex_loc, model_loc, face_loc;
};
class pyrHouse : public Building
{
    public:
        pyrHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x = 0, GLfloat theta_y = 0, GLfloat theta_z = 0,
        GLfloat scale_x = 1, GLfloat scale_y = 1, GLfloat scale_z = 1);
        ~pyrHouse() override;
        void draw(mat4 globalModel) override;
        
    private:
        GLuint pyrVao, pyrBuffer, pyrEbo[4];
        GLuint rectVao, rectBuffer, rectEbo[6];
        const vec3 rectColour[6];
        mat4 rectModel, pyrModel;
        GLuint vertex_loc, model_loc, face_loc;
};

class coneCylinderHouse : public Building {
private:
    GLuint vaoCylinder[3], bufferCylinder[3];
    GLuint coneVao, coneBuffer;
    mat4 cylinderModel, coneModel;
    GLuint vertex_loc, face_loc, model_loc;
    const int numSegments = 36;
    const float PI = 3.14159265359f;
    vec4 top[36];
    vec4 bottom[36];
    vec4 side[72];
    
    // Add these to store points
    std::vector<vec4> cylinderPoints;
    std::vector<vec4> conePoints;

public:
    coneCylinderHouse(GLuint vertexLoc, GLuint faceLoc, GLuint modelLoc,
        vec4 pos, GLfloat theta_x, GLfloat theta_y, GLfloat theta_z,
        GLfloat scale_x, GLfloat scale_y, GLfloat scale_z);
    ~coneCylinderHouse();
    void draw(mat4 globalModel) override;
};

#endif 
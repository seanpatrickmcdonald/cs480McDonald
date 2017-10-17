#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "bmploader.h"

const int numBodies = 11;

enum ObjectName
{
    sun = 0,
    mercury = 1,
    venus = 2,
    earth = 3,
    moon = 4,
    mars = 5,
    jupiter = 6,
    saturn = 7,
    uranus = 8,
    neptune = 9,
    pluto = 10,
};

struct ObjInit{
    std::string obj_filename[numBodies];
    std::string tex_filename[numBodies];
    std::string obj_name[numBodies];  //This is for creating the parent child relationships
    std::string parent_name[numBodies];
    float rotationSpeed[numBodies];
    float orbitSpeed[numBodies];
    float orbitRadius[numBodies];
    float scale[numBodies];
};

struct modelObject{
  glm::mat4 model;
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  GLuint VB;
  GLuint IB;
};

class Object
{
  public:
    Object();
    Object(ObjInit objectStruct, int index);
    Object(int asdf); //specifying a root object
    ~Object();

    void Update(unsigned int dt);
    void Update(unsigned int dt, glm::vec3 parentCenter);
    void Render(GLint, GLint);
  
    bool drawOrbitPath = false;
    void buildOrbitPath(float radius);

    glm::mat4 GetModel();
    static int readFromConfig(std::string configPath, ObjInit &obj);

    Object *parentPtr;
    std::vector <Object *> childPtr;

    glm::vec3 trans_vector;

    GLuint texture_int;
    std::string obj_name;
    void toggleFixedScale();

    float scale;


  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    glm::mat4 orbitmodel;
    std::vector<Vertex> orbitVertices;
    std::vector<unsigned int> orbitIndices;
    GLuint orbitVB;
    GLuint orbitIB;

    modelObject *ringModel;

    Assimp::Importer *modelImporter;

    float fixedScale = 3;
    bool fixedScaleTog = false;

    float rotationAngle;
    float orbitAngle;

    float rotationSpeed;
    float orbitSpeed;
    float orbitRadius;
};

#endif /* OBJECT_H */

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "bmploader.h"

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
    std::string obj_filename[11];
    std::string tex_filename[11];
    std::string obj_name[11];  //This is for creating the parent child relationships
    std::string parent_name[11];
    float rotationSpeed[11];
    float orbitSpeed[11];
    float orbitRadius[11];
    float scale[11];
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

    glm::mat4 GetModel();
    static int readFromConfig(std::string configPath, ObjInit &obj);

    Object *parentPtr;
    std::vector <Object *> childPtr;

    glm::vec3 trans_vector;

    GLuint texture_int;
    std::string obj_name;
    float scale;

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Assimp::Importer *modelImporter;

    float rotationAngle;
    float orbitAngle;

    float rotationSpeed;
    float orbitSpeed;
    float orbitRadius;
};

#endif /* OBJECT_H */

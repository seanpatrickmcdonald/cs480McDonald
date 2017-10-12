#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "bmploader.h"
#include <fstream> // used for file input
#include <iostream> // used for I/O
#include <string> // used for string operations

class Object
{
  public:
    Object();
    Object(std::string objFilename);
    ~Object();
    void Update(unsigned int dt);
    void Render(GLint);
	void 
    glm::mat4 GetModel();
	void Object::readFromConfig(char* configPath, ObjInit obj);

struct ObjInit{
	std::string obj_filename[11];
  	std::string tex_filename[11];
    std::string obj_name[11];  //This is for creating the parent child relationships
    std::string parent_name[11];
    float rotationAngle[11];
    float orbitAngle[11];
    float orbitRadius[11];

}'

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint texture_int;

    Assimp::Importer *modelImporter;
	/*std::string obj_filename;
  	std::string tex_filename;
    std::string obj_name;  //This is for creating the parent child relationships
    std::string parent_name;*/
    float rotationAngle;
    float orbitAngle;
    float orbitRadius;
};

#endif /* OBJECT_H */

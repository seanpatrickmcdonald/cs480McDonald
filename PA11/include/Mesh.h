#ifndef MESH_H_
#define MESH_H_

#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "shader.h"
#include "stb_image.h"

class Mesh
{
public:

	Mesh();
	~Mesh();

    void InitializeVertices();    
    GLuint LoadTexture(std::string textureName);   
    void LoadVertexData(std::string objFilename);

    void Update(unsigned int dt);
    void Render(Shader*);
    void ShadowRender();

    glm::mat4 model;
    glm::mat4 GetModel();
    std::string obj_filename;

    const aiScene *m_scene;
    static std::vector<GLuint> texture_ints;
    static std::vector<std::string> texture_names;

protected:

    std::vector<Vertex> Vertices;
    std::vector<uint> Indices;
    std::vector<uint> IndicesIndices;
    GLuint VB;
    GLuint IB;
    GLuint texture_int;
    std::vector<uint> texture_indices;
    uint num_textures;

    float scale;
};







#endif
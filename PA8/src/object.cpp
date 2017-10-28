#include "object.h"

Object::Object()
{  

}

GLuint Object::loadBMP(std::string textureName)
{    
    BMPLoader loader;
    loader.loadFromFile(textureName);

    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
        
    glTextureStorage2D(tex, 1, GL_RGBA32F, loader.width, loader.height);
    glBindTexture(GL_TEXTURE_2D, tex);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureSubImage2D(tex,
		        0,
		        0, 0,
		        loader.width, loader.height,
		        GL_RGB,
		        GL_UNSIGNED_BYTE,
		        loader.data);
 
    return tex;
}

const aiMesh* Object::LoadAssimp(std::string objFilename)
{
    //Use assimp namespace for this function only
    using namespace Assimp;

    Importer *modelImporter;

    //Create Assimp Importer
    modelImporter = new Importer();
    const aiScene* modelScene = modelImporter->ReadFile(objFilename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
    const aiMesh *modelMesh = modelScene->mMeshes[0];

    return modelMesh;
}

Object::Object(std::string objFilename)
{
    //bring in textures and load to opengl
    texture_int = loadBMP("../assets/earth.bmp");

    //build our aimesh object
    const aiMesh *modelMesh = LoadAssimp(objFilename);



    //Load Vertex Positions and TextureCoords
    for (unsigned int i = 0; i < modelMesh->mNumVertices; i++)
    {
        glm::vec3 vertexVec;

        vertexVec.x = modelMesh->mVertices[i].x;
        vertexVec.y = modelMesh->mVertices[i].y;
        vertexVec.z = modelMesh->mVertices[i].z;

        glm::vec2 uv;

        uv.x = modelMesh->mTextureCoords[0][i].x;
        uv.y = modelMesh->mTextureCoords[0][i].y;
	        
        Vertex dummyVertex(vertexVec, uv);
        Vertices.push_back(dummyVertex);        
    }
    //Load Indices
    for (unsigned int i = 0; i < modelMesh->mNumFaces; i++)
    {
        if (modelMesh->mFaces[i].mNumIndices == 3)  //only if it's a triangle
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                Indices.push_back(modelMesh->mFaces[i].mIndices[j]);
            }
        }
    }

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{


}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(GLint location)
{

  glBindTexture(GL_TEXTURE_2D, texture_int);
  

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}


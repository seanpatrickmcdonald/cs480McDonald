#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  rotationAngle = 0.0f;
  orbitAngle = 0.0f;
  orbitRadius = 6.5f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::Object(std::string objFilename)
{
    //Use assimp namespace for this function only
    using namespace Assimp;

    //Create Assimp Importer
    modelImporter = new Importer();
    const aiScene* modelScene = modelImporter->ReadFile(objFilename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
    
    for (unsigned int i = 0; i < modelScene->mMeshes[0]->mNumVertices; i++)
    {
        glm::vec3 vertexVec;

        vertexVec.x = modelScene->mMeshes[0]->mVertices[i].x;
        vertexVec.y = modelScene->mMeshes[0]->mVertices[i].y;
        vertexVec.z = modelScene->mMeshes[0]->mVertices[i].z;

        glm::vec3 color(1.0f, 1.0f, 1.0f);
	        
        Vertex dummyVertex(vertexVec, color);
        Vertices.push_back(dummyVertex);        
    }

    for (unsigned int i = 0; i < modelScene->mMeshes[0]->mNumFaces; i++)
    {
        if (modelScene->mMeshes[0]->mFaces[i].mNumIndices == 3)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                Indices.push_back(modelScene->mMeshes[0]->mFaces[i].mIndices[j]);
            }
        }
    }

   for (unsigned int i = 0; i < Indices.size(); i++)


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

  rotationAngle += dt * M_PI/2000;
  //orbitAngle += dt * M_PI/2000;

  glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(orbitRadius * cos(orbitAngle), 0.0f, orbitRadius * sin(orbitAngle) ));
  glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), (rotationAngle), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 ScaleMatrix = glm::scale(glm::vec3(1.01f, 1.0f, 1.0f));

  model = TranslationMatrix * RotationMatrix * ScaleMatrix;

}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}


#include "physics_object.h"

const std::string texture_location = "../assets/";

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{

}


PhysicsObject::PhysicsObject(PhysicsObjectStruct objStruct, PhysicsManager *physics_manager)
: Object( objStruct.texFilename)
{  
  btCollisionShape *collisionShape;   
  btVector3 Yup(0.0f, 1.0f, 0.0f);

  /*

    Data is loaded into vertices, so we can do with it as we need. Structure names on next lines.


    std::vector<Vertex> Vertices
    std::vector<unsigned int> Indices  


    (Vertex Struct from graphics_headers.h)

    struct Vertex
    {
      glm::vec3 vertex;
      glm::vec3 normal;
      glm::vec2 uv;

      Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 u): vertex(v), normal(n), uv(u) {}
    }

    
    Example: 
        Vertices[index].vertex will give a glm::vec3, so access this with .x, .y, .z, or use the 
            glm::value_ptr casting.

    Indices is also initalized, and that will also have to be used to get all of the triangles.

  */
 
	
  collisionMesh = new btTriangleMesh(); 

  loadVertexData(objStruct.objFilename);


  if (objStruct.primitiveType == "sphere")
      collisionShape = new btSphereShape(btScalar(0.016f));
  else if (objStruct.primitiveType == "cylinder")
      collisionShape = new btCylinderShape(btVector3(0.25f, 0.5f, 0.5f));
  else if (objStruct.primitiveType == "box")
      collisionShape = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));
  else if (objStruct.primitiveType == "plane")
      collisionShape = new btStaticPlaneShape(Yup, 0);
  else if (objStruct.mass == 0)
	  collisionShape = new btBvhTriangleMeshShape(collisionMesh, true);
  else
    collisionShape = new btConvexTriangleMeshShape(collisionMesh, true);
  

  //btConvexTriangleMeshShape
  //btBvhTriangleMeshShape

  btVector3 origin     = objStruct.origin;
  btScalar mass        = btScalar(objStruct.mass);
  btScalar restitution = objStruct.restitution;
  btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
  bool kinematic       = objStruct.kinematic;

  physicsIndex = objStruct.physicsIndex;
  physics_manager->AddRigidBody(collisionShape, origin, mass, restitution, inertia, kinematic, objStruct.objName, physicsIndex);


  //std::cout << objStruct.objName << ": " << physicsIndex << std::endl;


  Object::InitializeVertices();
}

void PhysicsObject::loadVertexData(std::string objFilename)
{
    //Use assimp namespace for this function only
    using namespace Assimp;
	  Importer *modelImporter;
    IndicesIndices.push_back(0);

    //Create Assimp Importer
    modelImporter = new Importer();
    const aiScene* modelScene = modelImporter->ReadFile(objFilename, 
                    aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

    if (modelScene == nullptr)
    {
        std::cout.flush() << "Invalid Mesh: " << objFilename << std::endl;
    }

    std::vector<std::string> texFileNames;
    if (modelScene->HasMaterials())
    {
      for (uint i = 0; i < modelScene->mNumMaterials; i++)
      {
      	aiString name;
        const aiMaterial* material = modelScene->mMaterials[i];
        material->Get(AI_MATKEY_NAME,name);

        std::string cur_material = texture_location;

        if (std::string(name.C_Str()) == "DefaultMaterial" || std::string(name.C_Str()) == "None" || name.length == 0)
        {
          continue;
        }

        for (uint j = 0; j < name.length; j++)
        {
          if (name.C_Str()[j] != '.')
          {
            cur_material.push_back(name.C_Str()[j]);
          }

          else
          {
            break;
          }
        }
        
        //building the current material and push to texture vector
        cur_material.append(".png");

        GLuint cur_texture = loadTexture(cur_material);
        if (cur_texture > 0)
        texture_ints.push_back(cur_texture);
      }

    } 
 
    
    int lastNumVertices = 0; //have to add this to the indices of subsequent meshes
    for (uint h = 0; h < modelScene->mNumMeshes; h++)
    {
      const aiMesh *modelMesh = modelScene->mMeshes[h];

      texture_indices.push_back(modelMesh->mMaterialIndex);

      //Load Vertex Positions and TextureCoords
      for (uint i = 0; i < modelMesh->mNumVertices; i++)
      {
          //Position
          glm::vec3 vertexVec;

          vertexVec.x = modelMesh->mVertices[i].x;
          vertexVec.y = modelMesh->mVertices[i].y;
          vertexVec.z = modelMesh->mVertices[i].z;

          //Normal
          glm::vec3 normal;

          if (modelMesh->HasNormals() > 0)
          {
              normal.x = modelMesh->mNormals[i].x;
              normal.y = modelMesh->mNormals[i].y;
              normal.z = modelMesh->mNormals[i].z;
          }

          else
          {
              normal.x = 0;
              normal.y = 0;
              normal.z = 0;
          }

          //Texture uv coords
          glm::vec2 uv;

          if (modelMesh->GetNumUVChannels() > 0)
          {
              uv.x = modelMesh->mTextureCoords[0][i].x;
              uv.y = modelMesh->mTextureCoords[0][i].y;
          }

          else
          {
              uv.x = 1.0f;
              uv.y = 1.0f;
          }        
              
          Vertex dummyVertex(vertexVec, normal, uv);
          Vertices.push_back(dummyVertex); 

      }

      //Load Indices
    	btVector3 triArray[3];
      for (unsigned int i = 0; i < modelMesh->mNumFaces; i++)
      {
  		  const aiFace& face = modelMesh->mFaces[i];

        if (face.mNumIndices == 3)  //only if it's a triangle
        {
          for (unsigned int j = 0; j < 3; j++)
          {	
			      aiVector3D position = modelMesh->mVertices[face.mIndices[j]];
			      triArray[j] = btVector3(position.x, position.y, position.z);


            if (h == 0)
            {
            //std::cout << "1: " << face.mIndices[0] 
            //          << " 2: " << face.mIndices[1]
            //          << " 3: " << face.mIndices[2] << std::endl;
            }

            Indices.push_back(face.mIndices[j] + lastNumVertices);
          }
		
          collisionMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
        }
      }

      IndicesIndices.push_back(modelMesh->mNumFaces * 3 + IndicesIndices[h]);
      lastNumVertices += modelMesh->mNumVertices;
    }

  //Bump textures down to start at zero
  for (uint i = 0; i < texture_indices.size(); i++)
  {
    texture_indices[i] -= 1;
  }

  delete modelImporter;
  modelImporter = nullptr;
  
}


#include "physics_object.h"

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
      collisionShape = new btStaticPlaneShape(Yup, 1.1);
  else
  {
	  collisionShape = new btBvhTriangleMeshShape(collisionMesh, true);
  }

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

    //Create Assimp Importer
    modelImporter = new Importer();
    const aiScene* modelScene = modelImporter->ReadFile(objFilename, 
                    aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

    const aiMesh *modelMesh = modelScene->mMeshes[0];

    
    if (modelScene == nullptr)
    {
        std::cout.flush() << "Invalid Mesh: " << objFilename << std::endl;
    }

    //Load Vertex Positions and TextureCoords
    for (unsigned int i = 0; i < modelMesh->mNumVertices; i++)
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

        if (modelMesh->mFaces[i].mNumIndices == 3)  //only if it's a triangle
        {
            for (unsigned int j = 0; j < 3; j++)
            {	
				aiVector3D position = modelMesh->mVertices[face.mIndices[j]];
				triArray[j] = btVector3(position.x, position.y, position.z);
                Indices.push_back(modelMesh->mFaces[i].mIndices[j]);
            }
			
            collisionMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
        }
    }
    

}


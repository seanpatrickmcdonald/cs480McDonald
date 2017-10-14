#include "object.h"

Object::Object()
{  

}

Object::Object(int asdf)
{
  parentPtr = nullptr;
}

GLuint loadBMP(std::string filename)
{
    
    BMPLoader loader;
    loader.loadFromFile(filename);
     
    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 1, GL_RGBA32F, loader.width, loader.height);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTextureSubImage2D(tex,
                        0,
                        0, 0,
                        loader.width, loader.height,
                        GL_RGB,
                        GL_UNSIGNED_BYTE,
                        loader.data);


    return tex;
}

Object::Object(ObjInit objectStruct, int index)
{
    rotationAngle = 0.0f;
    orbitAngle = 0.0f;
    
    orbitRadius = objectStruct.orbitRadius[index] * 2;
    orbitSpeed = objectStruct.orbitSpeed[index];
    rotationSpeed = objectStruct.rotationSpeed[index];
    scale = objectStruct.scale[index];

    obj_name = objectStruct.obj_name[index];
    texture_int = loadBMP(objectStruct.tex_filename[index]);    

    //Use assimp namespace for this function only
    using namespace Assimp;

    //Create Assimp Importer
    modelImporter = new Importer();
    const aiScene* modelScene = modelImporter->ReadFile(objectStruct.obj_filename[index], aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
    const aiMesh *modelMesh = modelScene->mMeshes[0];
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

    for (unsigned int i = 0; i < modelMesh->mNumFaces; i++)
    {
        if (modelMesh->mFaces[i].mNumIndices == 3)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                Indices.push_back(modelScene->mMeshes[0]->mFaces[i].mIndices[j]);
            }
        }
    }

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


  delete modelImporter;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

//root start update chain
void Object::Update(unsigned int dt)
{
  //root object, so let's start updating children
  if (parentPtr == nullptr && childPtr.size() > 0)
  {
    for (unsigned int i = 0; i < childPtr.size(); i++)
    {
        childPtr[i]->Update(dt, glm::vec3(0, 0, 0));
    }
  }
}

void Object::Update(unsigned int dt, glm::vec3 parentCenter)
{

  rotationAngle += rotationSpeed * dt * M_PI/2000;
  orbitAngle += orbitSpeed * dt * M_PI/20000;

  glm::vec3 translationVector = glm::vec3(orbitRadius * cos(orbitAngle) + parentCenter.x, 
                                                 0.0f, 
                                                 orbitRadius * sin(orbitAngle) + parentCenter.z);

  glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), translationVector);
  glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), (rotationAngle), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 ScaleMatrix = glm::scale(glm::vec3(scale, scale, scale));

  model = TranslationMatrix * RotationMatrix * ScaleMatrix;

  if (childPtr.size() > 0)
  {
    for (unsigned int i = 0; i < childPtr.size(); i++)
    {
        childPtr[i]->Update(dt, translationVector);
    }
  }
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(GLint location, GLint model_matrix_int)
{  
  glUniformMatrix4fv(model_matrix_int, 1, GL_FALSE, glm::value_ptr(model));
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


//This function akes in a config file containing planet data
//the Objinit contains arrays and planet data will be stored in each index of the array
//One objInit can initiate all planet models including the moon satelite. 

int Object::readFromConfig(std::string configPath, ObjInit &obj)
{
	std::ifstream configFile;
	std::string line;

	// open file
      configFile.open(configPath, std::ifstream::in);

         // check for open failure
         if( !configFile.is_open() )
         {
            // return failure
            return false;
         }
	int index;
	for(index = 0; index <11; index++)
	{
		configFile >> obj.obj_name[index];
		//check for invalid
		configFile >> obj.obj_filename[index];
		//check fr invalid
		configFile >> obj.tex_filename[index];
		//check for invalid
		configFile >> obj.parent_name[index];
		//check fr invalid
		configFile >> obj.rotationSpeed[index];
		//check for invalid
		configFile >> obj.orbitSpeed[index];
		//check fr invalid
		configFile >> obj.orbitRadius[index];

                configFile >> obj.scale[index];
        
	}

     return 1;
		
}

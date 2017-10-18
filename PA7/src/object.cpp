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
    glTextureStorage2D(tex, 10, GL_RGBA32F, loader.width, loader.height);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTextureSubImage2D(tex,
                        0,
                        0, 0,
                        loader.width, loader.height,
                        GL_RGB,
                        GL_UNSIGNED_BYTE,
                        loader.data);

    glGenerateMipmap(GL_TEXTURE_2D);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


    return tex;
}






modelObject* buildRing( float radius)
{
  modelObject *objectIn = new modelObject;

  float angle = 0;
  unsigned int index = 0;

  float step = 1/radius;
  if (step > 0.01)
  {
    step = 0.01;
  }


  //while we're below a full circle - 2PI radians
  while(angle < 2 * M_PI)
  {
    glm::vec3 v1 = glm::vec3(sin(angle) * (radius + 0.125), 0.001, cos(angle) * (radius + 0.125));
    glm::vec3 v2 = glm::vec3(sin(angle) * (radius - 0.125), 0.001, cos(angle) * (radius - 0.125));

    glm::vec2 uv = glm::vec2(0.5f, 0.5f);

    Vertex vert1 = Vertex(v1, uv);
    Vertex vert2 = Vertex(v2, uv);

    objectIn->Vertices.push_back(vert1);
    objectIn->Vertices.push_back(vert2);
    
    if (angle + step >= 2 * M_PI)
    {
        objectIn->Indices.push_back(index);
        objectIn->Indices.push_back(0);
        objectIn->Indices.push_back(index + 1);
        objectIn->Indices.push_back(index + 1);
        objectIn->Indices.push_back(0);
        objectIn->Indices.push_back(1);
    } 

    else
    { 
        objectIn->Indices.push_back(index);
        objectIn->Indices.push_back(index + 2);
        objectIn->Indices.push_back(index + 1);
        objectIn->Indices.push_back(index + 1);
        objectIn->Indices.push_back(index + 2);
        objectIn->Indices.push_back(index + 3);
    }

    index += 2;

    //step forward 0.1 radians
    angle += step;
  }

  glGenBuffers(1, &objectIn->VB);
  glBindBuffer(GL_ARRAY_BUFFER, objectIn->VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * objectIn->Vertices.size(), &objectIn->Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &objectIn->IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectIn->IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * objectIn->Indices.size(), &objectIn->Indices[0], GL_STATIC_DRAW);

  glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 0.0));
  objectIn->model = TranslationMatrix;

  return objectIn;
}







void Object::buildOrbitPath(float radius)
{

  float angle = 0;
  unsigned int index = 0;

  float step = 1/radius;
  if (step > 0.01)
  {
    step = 0.01;
  }

  //while we're below a full circle - 2PI radians
  while(angle < 2 * M_PI)
  {
    glm::vec3 v1 = glm::vec3(sin(angle) * (radius + scale * 0.125), 0.0, cos(angle) * (radius + scale * 0.125));
    glm::vec3 v2 = glm::vec3(sin(angle) * (radius - scale * 0.125), 0.0, cos(angle) * (radius - scale * 0.125));

    glm::vec2 uv = glm::vec2(0.0f, 0.0f);

    Vertex vert1 = Vertex(v1, uv);
    Vertex vert2 = Vertex(v2, uv);

    orbitVertices.push_back(vert1);
    orbitVertices.push_back(vert2);
    
    if (angle + step >= 2 * M_PI)
    {
        orbitIndices.push_back(index);
        orbitIndices.push_back(0);
        orbitIndices.push_back(index + 1);
        orbitIndices.push_back(index + 1);
        orbitIndices.push_back(0);
        orbitIndices.push_back(1);
    } 

    else
    { 
        orbitIndices.push_back(index);
        orbitIndices.push_back(index + 2);
        orbitIndices.push_back(index + 1);
        orbitIndices.push_back(index + 1);
        orbitIndices.push_back(index + 2);
        orbitIndices.push_back(index + 3);
    }

    index += 2;

    //step forward 0.1 radians
    angle += step;
  }

  glGenBuffers(1, &orbitVB);
  glBindBuffer(GL_ARRAY_BUFFER, orbitVB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * orbitVertices.size(), &orbitVertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &orbitIB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbitIB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * orbitIndices.size(), &orbitIndices[0], GL_STATIC_DRAW);

  glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, 0.0));
  orbitmodel = TranslationMatrix;
}

Object::Object(ObjInit objectStruct, int index)
{
    rotationAngle = 0.0f;
    orbitAngle = float(rand())/ RAND_MAX * 2 * M_PI;
    
    orbitRadius = objectStruct.orbitRadius[index] * 2;
    orbitSpeed = objectStruct.orbitSpeed[index];
    rotationSpeed = objectStruct.rotationSpeed[index];
    scale = objectStruct.scale[index];

    if(objectStruct.obj_name[index] != "sun")
    {
      buildOrbitPath(orbitRadius);      
      drawOrbitPath = true;
    }

    obj_name = objectStruct.obj_name[index];
    texture_int = loadBMP(objectStruct.tex_filename[index]);

    if (obj_name == "saturn"){
        ringModel = buildRing(1.1f);
    }    

    else
    {
    	ringModel = nullptr;
    }

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

//children update/chain
void Object::Update(unsigned int dt, glm::vec3 parentCenter)
{

  rotationAngle += rotationSpeed * dt * M_PI/2000;
  orbitAngle += orbitSpeed * dt * M_PI/20000;

  trans_vector = glm::vec3(orbitRadius * sin(orbitAngle) + parentCenter.x, 
                                                 0.0f, 
                                                 orbitRadius * cos(orbitAngle) + parentCenter.z);

  glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), trans_vector);
  glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), (rotationAngle), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 ScaleMatrix;
  if (!fixedScaleTog)
  {
    ScaleMatrix = glm::scale(glm::vec3(scale, scale, scale));
  }

  else
  {
    ScaleMatrix = glm::scale(glm::vec3(fixedScale, fixedScale, fixedScale));
  }
  model = TranslationMatrix * RotationMatrix * ScaleMatrix;

  if (childPtr.size() > 0)
  {
    for (unsigned int i = 0; i < childPtr.size(); i++)
    {
        childPtr[i]->Update(dt, trans_vector);
    }
  }

  if (parentCenter.x != 0)
  {
    parentCenter.y = 0.0001;
  }
  glm::mat4 orbTranslationMatrix = glm::translate(glm::mat4(), parentCenter);
  orbitmodel = orbTranslationMatrix;

  if (ringModel != nullptr)
  ringModel->model = TranslationMatrix;
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


  if (drawOrbitPath)
  {
      glUniformMatrix4fv(model_matrix_int, 1, GL_FALSE, glm::value_ptr(orbitmodel));

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);

      glBindBuffer(GL_ARRAY_BUFFER, orbitVB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbitIB);

      glDrawElements(GL_TRIANGLES, orbitIndices.size(), GL_UNSIGNED_INT, 0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
  }

  if (ringModel != nullptr)
  { 
  	glUniformMatrix4fv(model_matrix_int, 1, GL_FALSE, glm::value_ptr(ringModel->model));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, ringModel->VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ringModel->IB);
    glDrawElements(GL_TRIANGLES, ringModel->Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }
  
}

void Object::chooseScale(bool setScale)
{
  fixedScaleTog = setScale;
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
	for(index = 0; index <numBodies; index++)
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

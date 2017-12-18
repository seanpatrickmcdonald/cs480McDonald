#include "Mesh.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

std::vector<GLuint> Mesh::texture_ints;
std::vector<std::string> Mesh::texture_names;

const std::string texture_location = "../assets/";

Mesh::Mesh()
{

  model = glm::mat4(1.0f);
	m_scene = nullptr;
}


Mesh::~Mesh()
{
  Vertices.clear();
  Indices.clear();

  glDeleteBuffers(1, (const GLuint *)&IB);
  glDeleteBuffers(1, (const GLuint *)&VB);
}


GLuint Mesh::LoadTexture(std::string textureName)
{    
    int width, height, n;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data;
    data = stbi_load(textureName.c_str(), &width, &height, &n, 3);

    if (!data)
    {
        std::cout << "Failed to Load Texture: " << textureName << std::endl;
    }

    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);   
    glTextureStorage2D(tex, 10, GL_RGBA32F, width, height);     
    glBindTexture(GL_TEXTURE_2D, tex);
   
    glTextureSubImage2D(tex,
		        0,
		        0, 0,
		        width, height,
		        GL_RGB,
		        GL_UNSIGNED_BYTE,
		        data);
 
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    if (data)
    {
      delete[] data;
      data = nullptr;
    }


    return tex;
}

void Mesh::LoadVertexData(std::string objFilename)
{
    obj_filename = objFilename;

    //Use assimp namespace for this function only
    using namespace Assimp;
	  Importer *modelImporter;
    IndicesIndices.push_back(0);

    //Create Assimp Importer
    modelImporter = new Importer();
    m_scene = modelImporter->ReadFile(objFilename, 
                    aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

    if (m_scene == nullptr)
    {
        std::cout.flush() << "Invalid Mesh: " << objFilename << std::endl;
    }


    if (m_scene->HasAnimations())
    {
    	for (uint i = 0; i < m_scene->mNumAnimations; i++)
    	{
    		//std::cout << m_scene->mAnimations[i]->mName.C_Str() << std::endl;
    	}
    }


    std::vector<uint> cur_mesh_index;

    if (objFilename == "../assets/knight.obj")
    {
      Mesh::texture_ints.push_back(LoadTexture("../assets/D88C852E.jpg"));
      //cur_mesh_index.push_back(te                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               )
    }
    
    else
    {
      std::vector<std::string> texFileNames;
      if (m_scene->HasMaterials())
      {
        for (uint i = 0; i < m_scene->mNumMaterials; i++)
        {
        	aiString name;
          const aiMaterial* material = m_scene->mMaterials[i];
          material->Get(AI_MATKEY_NAME,name);

          std::string cur_material = texture_location;

          if (std::string(name.C_Str()) == "DefaultMaterial" || std::string(name.C_Str()) == "None" || name.length == 0)
          {
            cur_mesh_index.push_back(0);
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
          
          //Don't want to push the same texture
          bool repeatTexture = false;
          for (uint i = 0; i < Mesh::texture_names.size(); i++)
          {
            if (cur_material == Mesh::texture_names[i])
            {
              repeatTexture = true;
              cur_mesh_index.push_back(i);
            }
          }

          if (!repeatTexture)
          {
            GLuint cur_texture = LoadTexture(cur_material);

            cur_mesh_index.push_back(texture_names.size());
            //std::cout << texture_names.size() << ": ";
            Mesh::texture_names.push_back(cur_material);
            //std::cout << texture_names[texture_names.size() - 1] << std::endl;

            if (cur_texture > 0)
            Mesh::texture_ints.push_back(cur_texture);
          }



        }
      } 
    }
    
    int lastNumVertices = 0; //have to add this to the indices of subsequent meshes
    for (uint h = 0; h < m_scene->mNumMeshes; h++)
    {
      const aiMesh *modelMesh = m_scene->mMeshes[h];

      if (objFilename != "../assets/knight.obj")
      texture_indices.push_back(cur_mesh_index[modelMesh->mMaterialIndex]);

      else
        texture_indices.push_back(4);


      //std::cout << cur_mesh_index[modelMesh->mMaterialIndex] << std::endl;

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
      for (unsigned int i = 0; i < modelMesh->mNumFaces; i++)
      {
  		const aiFace& face = modelMesh->mFaces[i];

        if (face.mNumIndices == 3)  //only if it's a triangle
        {
          for (unsigned int j = 0; j < 3; j++)
          {	
            Indices.push_back(face.mIndices[j] + lastNumVertices);
          }
        }
      }

      IndicesIndices.push_back(modelMesh->mNumFaces * 3 + IndicesIndices[h]);
      lastNumVertices += modelMesh->mNumVertices;
    }
 

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  if (modelImporter != nullptr)
  {
    delete modelImporter;
    modelImporter = nullptr;

  }
  if (m_scene != nullptr)
  {
    m_scene = nullptr;
  }
}


void Mesh::Render(Shader *shader)
{
  if (IndicesIndices.size() == 0)
    IndicesIndices.push_back(0), IndicesIndices.push_back(Indices.size());

  //std::cout << Mesh::texture_ints.size() << std::endl;
  //std::cout << texture_indices[texture_indices.size() - 1] << std::endl;
  //std::cout << Indices.size() << std::endl;

  for (uint i = 0; i < texture_indices.size(); i++)
  {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, Mesh::texture_ints[texture_indices[i]]);
      //std::cout << obj_filename << ": " << Mesh::texture_ints[texture_indices[i]] << std::endl;
      //std::cout << texture_indices[i] << "\n\n";
      //glBindTexture(GL_TEXTURE_2D, Mesh::texture_ints[0]);
      glUniform1i(shader->uniforms[TEXSAMPLER], 0); 

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);

      glBindBuffer(GL_ARRAY_BUFFER, VB);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

      glDrawElements(GL_TRIANGLES, IndicesIndices[i + 1] - IndicesIndices[i], 
           GL_UNSIGNED_INT, (void*)(sizeof(int) * IndicesIndices[i]));

      //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
  }
}

void Mesh::ShadowRender()
{
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
}

glm::mat4 Mesh::GetModel()
{
  return model;
}
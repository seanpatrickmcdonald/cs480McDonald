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
  rotDirection = 1;
  rotationSpeed = 1.0f;
  orbitDirection = 1;
  orbitRadius = 2;
  orbitSpeed = 1;

  scale = glm::vec3(1.0f, 1.0f, 1.0f);

  parent = nullptr;
  child = nullptr;

  center = glm::vec3(0.0f, 0.0f, 0.0f);

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

void Object::FlipRotation(Object* objectToFlip)
{
  objectToFlip->rotDirection *= -1;
}

void Object::FlipOrbit(Object* objectToFlip)
{
	objectToFlip->orbitDirection *= -1;
}

void Object::SetRotationSpeed(Object* objectToChange, float speedIn)
{
  if (speedIn >= 0)
  {
    objectToChange->rotationSpeed = speedIn;
  }
}

void Object::SetOrbitRadius(Object* objectToChange, float radiusIn)
{
	if (radiusIn > 0)
	{
		objectToChange->orbitRadius = radiusIn;		
	}
}

void Object::SetOrbitSpeed(Object* objectToChange, float speedIn)
{
	if (speedIn >= 0)
	{
		objectToChange->orbitSpeed = speedIn;
	}
}

void Object::SetScale(Object* objectToChange, float scaleIn)
{
  if (scaleIn > 0)
  {
    objectToChange->scale = glm::vec3(scaleIn, scaleIn, scaleIn);
  }
}

void Object::SetParent(Object* objectToChange, Object* parentIn)
{
  objectToChange->parent = parentIn;
  parentIn->child = objectToChange;
}

void Object::Update(unsigned int dt)
{
  
  rotationAngle += rotationSpeed * rotDirection * dt * M_PI / 2000;

  orbitAngle += orbitDirection * orbitSpeed * dt * M_PI / 2000;

  glm::vec3 translationVector = glm::vec3(cos(orbitAngle) * orbitRadius, 
                                          0.0f, 
                                          sin(orbitAngle) * orbitRadius) + center;

  glm::mat4 TranslationMatrix = glm::translate(
      glm::mat4(), 
      translationVector
      );                
  
  if (child != nullptr)
  {
    child->center = translationVector;
    child->Update(dt);
  }

  glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), (rotationAngle), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 ScaleMatrix = glm::scale(scale);

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


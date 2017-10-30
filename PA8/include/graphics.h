#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics_object.h"
#include "json.hpp"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv);
    void Update(unsigned int dt);
    void Render();
    PhysicsManager* getPhysicsManager();
    glm::vec3 trans_vector = glm::vec3(0.0f, 0.0f, 0.0f);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    PhysicsManager *m_physics;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_mySampler;

    //Object **m_objects;
    PhysicsObject **m_physicsObjects;


    //unsigned int numObjects;
    size_t num_physics_objects;
};

#endif /* GRAPHICS_H */

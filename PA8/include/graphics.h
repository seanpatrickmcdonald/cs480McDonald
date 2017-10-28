#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics_object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv);
    void Update(unsigned int dt);
    void Render();

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
};

#endif /* GRAPHICS_H */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "glu.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv);
    void Update(unsigned int dt);
    void Render();
    Object ** getObjects();
    Camera *m_camera;

    unsigned int simulationSpeed;

  private:
    std::string ErrorString(GLenum error);

    Shader *m_shader;
    Object *root;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_mySampler;

    Object **m_objects;
};

#endif /* GRAPHICS_H */

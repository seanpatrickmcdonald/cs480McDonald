#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics_object.h"
#include "json.hpp"
#include "gui.h"
#include "DebugDraw.h"

using namespace std;

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv, SDL_Window*);
    void Update(unsigned int dt);
    void Render();
    PhysicsManager* getPhysicsManager();

    void ToggleShader();

    //GuiHandle and window handle
    GuiHandle *m_gui;    
    SDL_Window* m_window; 

    std::vector<Object *> m_object;
    PhysicsObject **m_physicsObjects;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_pervertex_shader;
    Shader *m_perfrag_shader;
    Shader *current_shader;
    PhysicsManager *m_physics;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float specularPower;

    bool renderObjects;
    bool renderPhysics;

    size_t numObjects;
    size_t num_physics_objects;

    mDebugDraw *debugDrawer;
};

#endif /* GRAPHICS_H */

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
#include "gui.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv, SDL_Window*);
    void Update(unsigned int dt);
    void Render();
    PhysicsManager* getPhysicsManager();
    glm::vec3 trans_vector = glm::vec3(0.0f, 0.0f, 0.0f);

    void ToggleShader();

    //GuiHandle and window handle
    GuiHandle *m_gui;    
    SDL_Window* m_window;    

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
    glm::vec3 spot_focus = glm::vec3(0.0, -1.0, 0.0);

    Object **m_objects;
    PhysicsObject **m_physicsObjects;

    size_t numObjects;
    size_t num_physics_objects;
};

#endif /* GRAPHICS_H */

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
#include "character.h"
#include "ShadowMap.h"

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
    Camera* getCamera();

    void ToggleShader();
    ShadowMap *shadowMap;

    //GuiHandle and window handle
    GuiHandle *m_gui;    
    SDL_Window* m_window; 

    std::vector<Object *> m_object;
    PhysicsObject **m_physicsObjects;
    Character *characterObject;

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
    glm::vec3 spot_position = glm::vec3(3.0, 10.0, 0.0);
    glm::vec3 spot_focus = glm::vec3(-4.75, -1.0, 0.0);
    float lightStrength;

    bool renderObjects;
    bool renderPhysics;

    float spot_dt = 0.0;

    size_t numObjects;
    size_t num_physics_objects;

    bool renderGUIDebug = true;

    mDebugDraw *debugDrawer;

};

#endif /* GRAPHICS_H */

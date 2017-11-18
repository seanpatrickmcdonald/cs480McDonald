#include "graphics.h"

using json = nlohmann::json;

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
  if(m_physics)
  	delete m_physics;
  m_physics = nullptr;
}

PhysicsObjectStruct* structFromJSON(json j, size_t index)
{
  PhysicsObjectStruct *passStruct = new PhysicsObjectStruct();
  passStruct->objName = j["physicsobjects"][index]["name"];
  passStruct->objFilename = j["physicsobjects"][index]["mesh"];
  passStruct->texFilename = j["physicsobjects"][index]["texture"];
  //passStruct->inertia = j["object"][index]["inertia"];
  passStruct->mass = j["physicsobjects"][index]["mass"];
  passStruct->restitution = j["physicsobjects"][index]["restitution"];
  passStruct->kinematic = j["physicsobjects"][index]["kinematic"];
  passStruct->origin = btVector3(
                                j["physicsobjects"][index]["origin.x"],
                                j["physicsobjects"][index]["origin.y"],
                                j["physicsobjects"][index]["origin.z"]        
                                );
  passStruct->primitiveType = j["physicsobjects"][index]["primitive"];
  passStruct->physicsIndex = index;

  return passStruct;
}

bool Graphics::Initialize(int width, int height, int argc, char **argv, SDL_Window *gWindow)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //Initialize our PhysicsManager
  m_physics = new PhysicsManager();

  
  //read in our json
  std::ifstream i("../assets/pa11.json");
  nlohmann::json j;
  i >> j ;

  //Create non-physics render only objects
  numObjects = j["objects"].size();

  for (unsigned int i = 0; i < numObjects; i++)
  {     
      Object *dummyObject = new Object(j["objects"][i]["mesh"], j["objects"][i]["texture"]);
      m_object.push_back(dummyObject);
  }

  unsigned int num_cubes = 4;

  // Create Physics Objects  
  num_physics_objects = j["physicsobjects"].size() - 1 + num_cubes;
  m_physicsObjects = new PhysicsObject*[num_physics_objects];

  PhysicsObjectStruct *cubeStruct = structFromJSON(j, 0);

  for (unsigned int cube_num = 0; cube_num < num_cubes; cube_num++)
  {
    m_physicsObjects[cube_num] = new PhysicsObject(*cubeStruct, m_physics);
    cubeStruct->origin = cubeStruct->origin + btVector3(0, 0.5, 0);
  }


  for (unsigned int i = 1; i < 2; i++)
  {     
      m_physicsObjects[i - 1 + num_cubes] = new PhysicsObject(*(structFromJSON(j, i)), m_physics);
  }

  // Set up the shaders
  m_pervertex_shader = new Shader();

  bool progInit = m_pervertex_shader->Initialize();
  if(!progInit)
  {
    printf("Shader Program Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_pervertex_shader->AddShader(GL_VERTEX_SHADER, "../shaders/pervertexlighting.vert"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_pervertex_shader->AddShader(GL_FRAGMENT_SHADER, "../shaders/pervertexlighting.frag"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }  

  // Connect the program
  if(!m_pervertex_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }
  
  
  m_perfrag_shader = new Shader();

  bool progInit2 = m_perfrag_shader->Initialize();
  if(!progInit2)
  {
    printf("Shader Program Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_perfrag_shader->AddShader(GL_VERTEX_SHADER, "../shaders/spfrag.vert"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_perfrag_shader->AddShader(GL_FRAGMENT_SHADER, "../shaders/spfrag.frag"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }  

  // Connect the program
  if(!m_perfrag_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  current_shader = m_perfrag_shader;

  //Gui Setup
  m_window = gWindow;
  m_gui = new GuiHandle();  

  if (!m_gui)
  {
    std::cout.flush() << "Failed to create GuiHandle - Not rendering Gui" << std::endl;
  }
  m_gui->GuiInit(m_window);  

  ambient = current_shader->GetUniform3f(current_shader->uniforms[AMBIENT]);
  diffuse = current_shader->GetUniform3f(current_shader->uniforms[DIFFUSE]);
  specular = current_shader->GetUniform3f(current_shader->uniforms[SPECULARALB]);
  specularPower = current_shader->GetUniformf(current_shader->uniforms[SPECULARPOW]);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); 

  debugDrawer = new mDebugDraw();
  debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

  m_physics->dynamicsWorld->setDebugDrawer(debugDrawer);

  renderObjects = true;
  renderPhysics = false;

  return true;
}

void Graphics::Update(unsigned int dt)
{
  m_physics->Update(dt);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (renderObjects)
  {
      // Start the correct program
      current_shader->Enable();

      // Send in the projection and view to the shader
      glUniformMatrix4fv(current_shader->m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
      glUniformMatrix4fv(current_shader->m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

      //Render Non-Physics Objects    
      for (unsigned int i = 0; i < m_object.size(); i++)
      {
        glUniformMatrix4fv(current_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_object[i]->GetModel()));
        m_object[i]->Render();
      }

      // Render Physics objects
      for (unsigned int i = 0; i < num_physics_objects; i++)
      {
        glUniformMatrix4fv(current_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_physics->GetModelMatrixAtIndex(i)));
        m_physicsObjects[i]->Render();
      }

      //Render Gui
      m_gui->NewFrame(m_window);
     
      ImGui::Begin("Stat Window");

      //ambient handling
      if (current_shader->uniforms[SPECULARPOW] != -1)
      {
        float newAmbient = ambient.x;
        ImGui::SliderFloat("Ambient", &newAmbient, 0.0f, 1.0f);
        ambient = glm::vec3(newAmbient, newAmbient, newAmbient);
        glUniform3f(current_shader->uniforms[AMBIENT], ambient.x, ambient.y, ambient.z);
        
        //diffuse handling
        float newDiffuse = diffuse.x;
        ImGui::SliderFloat("Diffuse", &newDiffuse, 0.0f, 1.0f);
        diffuse = glm::vec3(newDiffuse, newDiffuse, newDiffuse);
        glUniform3f(current_shader->uniforms[DIFFUSE], diffuse.x, diffuse.y, diffuse.z);

        //specularpower handling
        float newSpecPower = specularPower;
        ImGui::SliderFloat("Specular Power", &newSpecPower, 0.0f, 128.0f);
        specularPower = newSpecPower;
        glUniform1f(current_shader->uniforms[SPECULARPOW], specularPower);
        
        //specularalbedo handling
        float newSpecular = specular.x;
        ImGui::SliderFloat("Specular Albedo", &newSpecular, 0.0f, 1.0f);
        specular = glm::vec3(newSpecular, newSpecular, newSpecular);
        glUniform3f(current_shader->uniforms[SPECULARALB], specular.x, specular.y, specular.z);
      }

      ImGui::End();

      ImGui::Render();
  }

  if (renderPhysics)
  {
    debugDrawer->setUniforms(m_camera->GetProjection(), m_camera->GetView());
    m_physics->dynamicsWorld->debugDrawWorld();
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }

}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

void Graphics::ToggleShader()
{
  if (current_shader == m_pervertex_shader) 
    current_shader = m_perfrag_shader;

  else
  current_shader = m_pervertex_shader;

  ambient = current_shader->GetUniform3f(current_shader->uniforms[AMBIENT]);
  diffuse = current_shader->GetUniform3f(current_shader->uniforms[DIFFUSE]);
  specular = current_shader->GetUniform3f(current_shader->uniforms[SPECULARALB]);
  specularPower = current_shader->GetUniformf(current_shader->uniforms[SPECULARPOW]);
}

PhysicsManager* Graphics::getPhysicsManager()
{
  return m_physics;
}

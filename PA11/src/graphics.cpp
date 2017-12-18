#include "graphics.h"

using json = nlohmann::json;

static const GLfloat g_quad_vertex_buffer_data[18] = { 
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
};

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
  if(m_physics)
  	delete m_physics;
  m_physics = nullptr;

  for (uint i = 0; i < lightVector.size(); i++)
  {
  	delete lightVector[i];
  	lightVector[i] = nullptr;
  }

  delete characterObject;
  characterObject = nullptr;

  for (uint i = 0; i < num_physics_objects; i++)
  {
    delete m_physicsObjects[i];
    m_physicsObjects[i] = nullptr;
  }

  delete[] m_physicsObjects;
  m_physicsObjects = nullptr;


  for (uint i = 0; i < m_object.size(); i++)
  {
    delete m_object[i];
  }

  m_object.clear();
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
      Mesh *dummyObject = new Mesh;
      dummyObject->LoadVertexData(j["objects"][i]["mesh"]);
      m_object.push_back(dummyObject);
  }

  unsigned int num_cubes = 1;

  // Create Physics Objects  
  num_physics_objects = j["physicsobjects"].size() - 1 + num_cubes;
  m_physicsObjects = new PhysicsObject*[num_physics_objects];

  PhysicsObjectStruct *cubeStruct = structFromJSON(j, 0);

  for (unsigned int cube_num = 0; cube_num < num_cubes; cube_num++)
  {
    m_physicsObjects[cube_num] = new PhysicsObject(*cubeStruct, m_physics);
    cubeStruct->origin = cubeStruct->origin + btVector3(0, 0.5, 0);
  }


  for (unsigned int i = 1; i < num_physics_objects - num_cubes + 1; i++)
  {     
      m_physicsObjects[i - 1 + num_cubes] = new PhysicsObject(*(structFromJSON(j, i)), m_physics);
  }

  //btVector3 characterOrigin = btVector3(j["player"][0]["origin"][0], j["player"][0]["origin"][1], j["player"][0]["origin"][2]);
  characterObject = new Character(j["player"][0]["mesh"], j["player"][0]["texture"], m_physics, true);

  // Set up the shaders
/*
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
  */
  
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


  btScalar mass        = btScalar(0.0f);
  btScalar restitution = btScalar(0.5f);
  btVector3 inertia    = btVector3(0.0f, 0.0f, 0.0f);
  bool kinematic       = false;

  btCollisionShape *collisionShape; 
  btCollisionShape *collisionShape1; 
  btCollisionShape *collisionShape2; 

  collisionShape = new btBoxShape(btVector3(6.0f, 1.0f, 13.6f));
  collisionShape1 = new btBoxShape(btVector3(1.0f, 4.0f, 11.5f));
  collisionShape2 = new btBoxShape(btVector3(1.0f, 4.0f, 11.5f));
  
  btVector3 origin     = btVector3(-0.5, -0.785, -12.8);
  btVector3 origin1     = btVector3(-6.7, 0.5, -11.0);
  btVector3 origin2     = btVector3(5.7, 0.5, -11.0);

  m_physics->AddRigidBody(collisionShape1, origin1, mass, restitution, inertia, kinematic, "wall1", 1);
  m_physics->AddRigidBody(collisionShape2, origin2, mass, restitution, inertia, kinematic, "wall2", 1);

  m_physics->AddRigidBody(collisionShape, origin, mass, restitution, inertia, kinematic, "InitialGround", 1);



  lightVector.push_back(new Light());
  //lightVector.push_back(new Light());

  LightInitStruct spotStruct; 
  LightInitStruct spotStructs[2];

  spotStructs[0].light_position = glm::vec3(2.5, 3.0, -5.0);;
  spotStructs[0].light_direction = glm::vec3(0.0, 0.0, 0.0);
  spotStructs[0].angle_inner = 20.0;
  spotStructs[0].angle_outer = 22.0;

  spotStructs[0].light_strength = 1.0;
  spotStructs[0].light_color = glm::vec3(1.0, 1.0, 1.0);
  spotStructs[0].light_type = SPOT;

  spotStructs[1] = spotStructs[0];

  lightVector[0]->InitSpot(spotStructs[0]);



  current_shader->Enable();


  LightInitStruct pointStruct[4];

  pointStruct[0].light_position = glm::vec3(10.0, -4.5, 10.0);
  pointStruct[0].light_strength = 10.0;
  pointStruct[0].light_color = glm::vec3(1.0, 1.0, 1.0);
  pointStruct[0].light_type = POINT;

  pointStruct[1].light_position = glm::vec3(20, -4.5, 20);
  pointStruct[2].light_position = glm::vec3(57.8, -4.5, -10.8558);
  pointStruct[3].light_position = glm::vec3(20, -4.5, 20);

  pointLight[0] = new Light();
  pointLight[0]->InitPoint(pointStruct[0]);

  pointLight[1] = new Light();
  pointLight[1]->InitPoint(pointStruct[1]);

  pointLight[2] = new Light();
  pointLight[2]->InitPoint(pointStruct[2]);

  pointLight[3] = new Light();
  pointLight[3]->InitPoint(pointStruct[3]);

  glUniform1i(current_shader->num_lights, 3);




  glUniform1i(current_shader->uniforms[NUMSPOTS], 1);

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
  lightStrength = 5.0;

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS); 

  debugDrawer = new mDebugDraw();
  debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

  m_physics->dynamicsWorld->setDebugDrawer(debugDrawer);

  renderObjects = true;
  renderPhysics = false;

  window_width = width, window_height = height;


  passThroughShader = new Shader();

  bool progInit = passThroughShader->Initialize();
  if(!progInit)
  {
    printf("Shader Program Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!passThroughShader->AddShader(GL_VERTEX_SHADER, "../shaders/passthrutex.vert"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!passThroughShader->AddShader(GL_FRAGMENT_SHADER, "../shaders/passthrutex.frag"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }  

  // Connect the program
  if(!passThroughShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }
  

  glGenBuffers(1, &quad_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

  glClearColor(0.0, 0.0, 0.0, 0.0);


  return true;
}

void Graphics::Update(unsigned int dt)
{
  if (m_physics->Update(dt));
    //m_camera->euler_rotation_angle = 141592653/2 + 3.141592653;
  
  current_shader->Enable();


  btTransform trans;
  btScalar m[16];	
  trans = characterObject->controller->getGhostObject()->getWorldTransform();
  trans.getOpenGLMatrix(m);

  glm::vec3 characterLocation = glm::vec3(trans.getOrigin()[0], trans.getOrigin()[1], trans.getOrigin()[2]);

  m_camera->SetPosition(characterLocation + glm::vec3(0.0, 2.0, 0.0));
  m_camera->LookAt(characterLocation);

  //btVector3 character_location(characterLocation.x, characterLocation.y, characterLocation.z);
  //m_physics->MoveKinematic(character_location, characterObject->m_kinematic_body);

  spot_dt += 1.0/100.0;
}

void Graphics::Render()
{

  if (renderObjects)
  {
      btTransform trans;
      btScalar m[16]; 
      trans = characterObject->controller->getGhostObject()->getWorldTransform();
      trans.getOpenGLMatrix(m);
      glm::mat4 modelMatrix = glm::make_mat4(m);
      glm::vec3 characterLocation = glm::vec3(trans.getOrigin()[0], trans.getOrigin()[1], trans.getOrigin()[2]);

      //moving the spotlight     
      //lightVector[0]->LookAt(glm::vec3(3 * cos(spot_dt) * 5, spot_focus.y, spot_focus.z * sin(spot_dt)));






      /*

	    SHADOW RENDER PASS

      */

      //Start our shadowmap program
      glm::mat4 depthProjectionMatrix, depthViewMatrix, depthModelMatrix;
	    glm::mat4 depthMVP; 

	    //Iterate over all lights
      for (unsigned int light_index = 0; light_index < lightVector.size(); light_index++)
      {
	      lightVector[light_index]->m_shadow_map->Enable(0);

        depthProjectionMatrix = lightVector[light_index]->depthProjectionMatrix;
        depthViewMatrix = lightVector[light_index]->depthViewMatrix;

        //Iterate over all our physics objects
        for (unsigned int object_index = 0; object_index < num_physics_objects; object_index++)
        {
        	depthModelMatrix = m_physics->GetModelMatrixAtIndex(object_index);
        	depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	        glUniformMatrix4fv(lightVector[light_index]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));
	        m_physicsObjects[object_index]->ShadowRender();
        }

        //Iterate over non physics objects
        for (unsigned int i = 0; i < m_object.size(); i++)
        {

          depthModelMatrix = m_object[i]->GetModel();
          depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
         
          glUniformMatrix4fv(pointLight[0]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));

          m_object[i]->ShadowRender();
        }


        //The Character is a separate object (not a rigid body)
        depthMVP = depthProjectionMatrix * depthViewMatrix * modelMatrix;
        glUniformMatrix4fv(lightVector[light_index]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));

        characterObject->ShadowRender(lightVector[light_index]->m_shadow_map->shadow_tex);
      }

      //Iterate over Point Lights

      glm::mat4 pointProjectionMatrix = pointLight[0]->depthProjectionMatrix;

      //Iterate over all sides of cube
      for (uint i = 0; i < 6; i++)
      {
        glm::mat4 pointViewMatrix = glm::lookAt(pointLight[0]->light_position, 
                                                pointLight[0]->m_shadow_map->gCameraDirections[i].Target + pointLight[0]->light_position,
                                                pointLight[0]->m_shadow_map->gCameraDirections[i].Up);

        pointLight[0]->m_shadow_map->Enable(pointLight[0]->m_shadow_map->gCameraDirections[i].CubemapFace);

        glm::vec3 light = pointLight[0]->light_position;
        glUniform3f(pointLight[0]->m_shadow_map->gWorldPos, light.x, light.y, light.z);


        //Iterate over Physics Objects
        for (unsigned int object_index = 0; object_index < num_physics_objects; object_index++)
        {
          glm::mat4 depModelMatrix = m_physics->GetModelMatrixAtIndex(object_index);
          depthMVP = pointProjectionMatrix * pointViewMatrix * depModelMatrix;

          glUniformMatrix4fv(pointLight[0]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));
          glUniformMatrix4fv(pointLight[0]->m_shadow_map->model, 1, GL_FALSE, glm::value_ptr(depModelMatrix));

          m_physicsObjects[object_index]->ShadowRender();
        }

        /*
        //Iterate over the static non-physics terrain
        for (unsigned int i = 0; i < m_object.size(); i++)
        {

          glm::mat4 depModelMatrix = m_object[i]->GetModel();
          depthMVP = pointProjectionMatrix * pointViewMatrix * depModelMatrix;
         
          glUniformMatrix4fv(pointLight[0]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));
          glUniformMatrix4fv(pointLight[0]->m_shadow_map->model, 1, GL_FALSE, glm::value_ptr(depModelMatrix)); 

          m_object[i]->ShadowRender();
        }*/

        //The Character is a separate object (not a rigid body)
        depthMVP = pointProjectionMatrix * pointViewMatrix * modelMatrix;

        glUniformMatrix4fv(pointLight[0]->m_shadow_map->depthMVP, 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniformMatrix4fv(pointLight[0]->m_shadow_map->model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        characterObject->ShadowRender(pointLight[0]->m_shadow_map->shadow_tex);
      }


      /*

		  ACTUAL RENDER PASS

      */

      //re-bind output buffer
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, window_width, window_height);

	    //clear the screen
      glClearColor(0.0, 0.0, 0.0, 0.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //glDisable(GL_CULL_FACE);
      // Start the correct program
      current_shader->Enable();

      glm::vec3 spotFocus, spotPosit;

      //Pass in spotlight focus
      for (unsigned int light_index = 0; light_index < lightVector.size(); light_index++)
      {
	      spotFocus = lightVector[light_index]->light_direction;
	      spotPosit = lightVector[light_index]->light_position;

	      std::string indexString = "[";
	      indexString.append(to_string(light_index));
	      indexString.append("]");

	      std::string focusString = "spot_focus";
	      focusString.append(indexString);      
	      GLuint spotFocusUniform = current_shader->GetUniformLocation(focusString.c_str());

	      std::string positionString = "spot_position";
	      focusString.append(indexString);     
	      GLuint spotPositUniform = current_shader->GetUniformLocation(positionString.c_str());

	      glUniform3f(spotFocusUniform, spotFocus.x, spotFocus.y, spotFocus.z);
	      glUniform3f(spotPositUniform, spotPosit.x, spotPosit.y, spotPosit.z);

		    glActiveTexture(GL_TEXTURE1);
		    glBindTexture(GL_TEXTURE_2D, lightVector[light_index]->m_shadow_map->shadow_tex);
		    //glUniform1i(current_shader->uniforms[SDWSAMPLER], 1);
      }

      //Point Texture
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_CUBE_MAP, pointLight[0]->m_shadow_map->shadow_tex);
      //glUniform1i(current_shader->uniforms[CUBESAMPLER], 2);




      // Send in the projection and view to the shader
      glUniformMatrix4fv(current_shader->m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
      glUniformMatrix4fv(current_shader->m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

      for (uint i = 0; i < 3; i++)
      {
        glUniform3fv(current_shader->light_positions + i, 1, glm::value_ptr(pointLight[i]->light_position));
      }

      //Render Non-Physics Objects    
      for (unsigned int i = 0; i < m_object.size(); i++)
      {
        glUniformMatrix4fv(current_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_object[i]->GetModel()));

        depthModelMatrix = m_object[i]->GetModel();
        depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
        
        glUniformMatrix4fv(current_shader->uniforms[DEPTHMVP], 1, GL_FALSE, glm::value_ptr(depthMVP));

        m_object[i]->Render(current_shader);
      }

      // Render Physics objects
      for (unsigned int i = 0; i < num_physics_objects; i++)
      {
        glUniformMatrix4fv(current_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_physics->GetModelMatrixAtIndex(i)));

        depthModelMatrix = m_physics->GetModelMatrixAtIndex(i);
        depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
        
        glUniformMatrix4fv(current_shader->uniforms[DEPTHMVP], 1, GL_FALSE, glm::value_ptr(depthMVP));

        m_physicsObjects[i]->Render(current_shader);
      }


      glUniformMatrix4fv(current_shader->m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

      depthMVP = depthProjectionMatrix * depthViewMatrix * modelMatrix;
      
      glUniformMatrix4fv(current_shader->uniforms[DEPTHMVP], 1, GL_FALSE, glm::value_ptr(depthMVP));

      characterObject->Render(current_shader);

      //printf("characterLocation: %f, %f, %f \n", characterLocation.x, characterLocation.y, characterLocation.z);
      

      if (renderShadowMap);
      {
        glViewport(0, 0, 256, 256);

        passThroughShader->Enable(); 


        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pointLight[0]->m_shadow_map->shadow_buffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
            pointLight[0]->m_shadow_map->shadow_tex, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pointLight[0]->m_shadow_map->depth_buffer); 

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glVertexAttribPointer(
          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
          3,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          (void*)0            // array buffer offset
        );

        // Draw the triangle !
        // You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
        glDisableVertexAttribArray(0);
      }

      if (renderGUIDebug)
      {
        current_shader->Enable();
          //Render Gui
	      m_gui->NewFrame(m_window);
	     
        ImGui::SetNextWindowPos(ImVec2(0,363)); 
        ImGui::SetNextWindowSize(ImVec2(256, 147)); 
	      ImGui::Begin("Stat Window", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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

	        //point light strength handling
	        
	        float newStrength = lightStrength;
	        ImGui::SliderFloat("Light Strength", &newStrength, 0.0f, 10.0f);
	        lightStrength = newStrength;
	        
	      }

	      ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0,0)); 
        ImGui::SetNextWindowSize(ImVec2(215,24)); 
        ImGui::Begin("CharLocation", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        std::string char_location = "X: ";

        stringstream stream;
        stream << fixed << setprecision(2) << characterLocation.x;
        string s = stream.str();

        char_location.append(s);

        char_location.append(" Y: ");

        stream.str("");
        stream << fixed << setprecision(2) << characterLocation.y;
        s = stream.str();

        char_location.append(s);

        char_location.append(" Z: ");

        stream.str("");
        stream << fixed << setprecision(2) << characterLocation.z;
        s = stream.str();

        char_location.append(s);

        ImGui::TextUnformatted(char_location.c_str());

        ImGui::End();

      ImGui::Render();
      }

      current_shader->Enable();

      glUniform1f(current_shader->uniforms[LIGHTSTR], lightStrength);
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


Camera* Graphics::getCamera()
{
  return m_camera;
}

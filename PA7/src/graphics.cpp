#include "graphics.h"


Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, int argc, char **argv)
{
  simulationSpeed = 1;
  
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

  // Create the object

  ObjInit asdf;
  //create the root object
  root = new Object(21);
  m_objects = new Object *[numBodies]; //initialize the array of pointers

  //read the config file and create the new objects
  if ( Object::readFromConfig("../assets/solarsysConfig.config", asdf) >= 0)
  {   
    for (unsigned int i = 0; i < numBodies; i++)
    {
      m_objects[i] = new Object(asdf, i); //construct the object with parameters

      bool parentNotMade = true;      
      int index = 0;
      while(parentNotMade)
      {
        //if the parent is root, eg Sun
        if (asdf.parent_name[i] == "root")
        {
          root->childPtr.push_back(m_objects[i]);
          parentNotMade = false;
        }
        // elseif we find the right parent by name, add the object pointer to its child vector
        else if (m_objects[index]->obj_name == asdf.parent_name[i])
        { 
          m_objects[index]->childPtr.push_back(m_objects[i]);

          parentNotMade = false;         
        }

        index++;
        if (index > numBodies - 1) //failsafe in case we're outside of array dimensions
        {
          std::cout << "uhoh: " << m_objects[i]->obj_name << std::endl;
          break;
        }
      }
      
    }
  }  

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  int vertexFileIndex = -1;
  int fragmentFileIndex = -1;

  if (argc > 1)
  {
    for (int i = 0; i < argc; i++)
    {
      if (argv[i][0] == '-' && i + 1 < argc && argv[i + 1][0] != '-')
      {
        if (argv[i][1] == 'v')
        {
          vertexFileIndex = i + 1;
        }

        if (argv[i][1] == 'f')
        {
          fragmentFileIndex = i + 1;
        }
      }
    }
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, vertexFileIndex, argv))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, fragmentFileIndex, argv))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }
  

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_mySampler = m_shader->GetUniformLocation("mySampler");
  if (m_mySampler == -1) 
  {
    printf("m_mySampler not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  root->Update(dt * simulationSpeed);
  m_camera->Update(m_objects[m_camera->parentEnum]);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the object
  for (unsigned int i = 0; i < numBodies; i++)
  {
    m_objects[i]->Render(m_mySampler, m_modelMatrix);
  }



  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

Object** Graphics::getObjects()
{
    return m_objects;
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

void Graphics::drawOrbits()
{
	GLUquadricObj *disk[10];
	int index; 
	for (index = 1; index<11; index++)
	{
		disk[index] = gluNewQuadric();
		gluDisk(disk[index], m_objects[index]->returnRadius(), m_objects[index]->returnRadius(), 12, 1);
	}

}

#include "ShadowMap.h"

std::string getShaderString(std::string stringIn);

GLuint ShadowMap::m_shaderProg = 0; 
GLuint ShadowMap::m_cubeProg = 0; 

ShadowMap::ShadowMap()
{

}

ShadowMap::~ShadowMap()
{
	
}

void ShadowMap::Enable(GLenum CubeFace)
{
    if (m_shadow_type == SPOT)
    {
      glUseProgram(ShadowMap::m_shaderProg);

      glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
    }

    else if (m_shadow_type == POINT)
    {
      glUseProgram(ShadowMap::m_cubeProg);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_buffer);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, shadow_tex, 0);
      glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
    

    glViewport(0,0,SHADOWMAP_SIZE,SHADOWMAP_SIZE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
}

std::string getShaderString(std::string stringIn)
{
    std::fstream shaderStream;  

    shaderStream.open(stringIn, std::fstream::in);

    if (!shaderStream)
    {
        std::cout << "Failed to open Shader File: " << std::endl 
                  << stringIn << std::endl << std::endl;        
    }

    std::string shaderString;
    std::stringstream buffer;

    buffer << shaderStream.rdbuf();
    shaderString = buffer.str();    

    shaderStream.close();

    return shaderString;
}


void ShadowMap::MakeSpot()
{  
  glGenFramebuffers(1, &shadow_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);

  glGenTextures(1, &shadow_tex);
  glBindTexture(GL_TEXTURE_2D, shadow_tex);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, SHADOWMAP_SIZE, SHADOWMAP_SIZE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_tex, 0);

  glDrawBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (ShadowMap::m_shaderProg == 0)
  {
    InitSpotProg();
  }

  m_shadow_type = SPOT;
}

void ShadowMap::MakeCube()
{
    // Create the FBO
    glGenFramebuffers(1, &shadow_buffer);

    // Create the depth buffer
    glGenTextures(1, &depth_buffer);
    glBindTexture(GL_TEXTURE_2D, depth_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create the cube map
    glGenTextures(1, &shadow_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_tex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (uint i = 0 ; i < 6 ; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0, GL_RED, GL_FLOAT, NULL);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);

    // Disable reads from the color buffer
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        //return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if ( ShadowMap::m_cubeProg == 0)
    {
      InitCubeProg();
    }

    m_shadow_type = POINT;

    //return GLCheckError();
}





void ShadowMap::InitSpotProg()
{
  std::string vertShaderString = getShaderString("../shaders/ShadowMap.vert");
  std::string fragShaderString = getShaderString("../shaders/ShadowMap.frag");

  GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);  
  GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* vert[1];
  vert[0] = vertShaderString.c_str();
  GLint Lengthsv[1] = { (GLint)vertShaderString.size() };

  const GLchar* frag[1];
  frag[0] = fragShaderString.c_str();
  GLint Lengthsf[1] = { (GLint)fragShaderString.size() };


  glShaderSource(vertShaderObj, 1, vert, Lengthsv);
  glShaderSource(fragShaderObj, 1, frag, Lengthsf);

  glCompileShader(vertShaderObj);
  glCompileShader(fragShaderObj);


  GLint success;
  glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(vertShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
  }

  glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(fragShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
  }


  ShadowMap::m_shaderProg = glCreateProgram();

  glAttachShader(ShadowMap::m_shaderProg, vertShaderObj);
  glAttachShader(ShadowMap::m_shaderProg, fragShaderObj);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(ShadowMap::m_shaderProg);

  glGetProgramiv(ShadowMap::m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(ShadowMap::m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
  }

  glValidateProgram(ShadowMap::m_shaderProg);
  glGetProgramiv(ShadowMap::m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(ShadowMap::m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
  }

  Enable(0);

  depthMVP = glGetUniformLocation(ShadowMap::m_shaderProg, "depthMVP");
}




void ShadowMap::InitCubeProg()
{
  std::string vertShaderString = getShaderString("../shaders/CubeShadowMap.vert");
  std::string fragShaderString = getShaderString("../shaders/CubeShadowMap.frag");

  GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);  
  GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* vert[1];
  vert[0] = vertShaderString.c_str();
  GLint Lengthsv[1] = { (GLint)vertShaderString.size() };

  const GLchar* frag[1];
  frag[0] = fragShaderString.c_str();
  GLint Lengthsf[1] = { (GLint)fragShaderString.size() };


  glShaderSource(vertShaderObj, 1, vert, Lengthsv);
  glShaderSource(fragShaderObj, 1, frag, Lengthsf);

  glCompileShader(vertShaderObj);
  glCompileShader(fragShaderObj);


  GLint success;
  glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(vertShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
  }

  glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(fragShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
  }


  ShadowMap::m_cubeProg = glCreateProgram();

  glAttachShader(ShadowMap::m_cubeProg, vertShaderObj);
  glAttachShader(ShadowMap::m_cubeProg, fragShaderObj);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(ShadowMap::m_cubeProg);

  glGetProgramiv(ShadowMap::m_cubeProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(ShadowMap::m_cubeProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
  }

  glValidateProgram(ShadowMap::m_cubeProg);
  glGetProgramiv(ShadowMap::m_cubeProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(ShadowMap::m_cubeProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
  }

  Enable(0);

  depthMVP = glGetUniformLocation(ShadowMap::m_cubeProg, "depthMVP");
  model = glGetUniformLocation(ShadowMap::m_cubeProg, "model");
}
#include "ShadowMap.h"

std::string getShaderString(std::string stringIn);

ShadowMap::ShadowMap()
{
  glGenFramebuffers(1, &shadow_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);

  glGenTextures(1, &shadow_tex);
  glBindTexture(GL_TEXTURE_2D, shadow_tex);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, 1024, 1024);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_tex, 0);

  glDrawBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);



  m_shaderProg = glCreateProgram();

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


  glAttachShader(m_shaderProg, vertShaderObj);
  glAttachShader(m_shaderProg, fragShaderObj);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
  }

  Enable();

  depthMVP = glGetUniformLocation(m_shaderProg, "depthMVP");
 // model_matrix = glGetUniformLocation(m_shaderProg, "model_matrix");
}

ShadowMap::~ShadowMap()
{
	
}

void ShadowMap::Enable()
{
    glUseProgram(m_shaderProg);
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

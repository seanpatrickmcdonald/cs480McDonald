#include "shader.h"

std::string const defaultVertexShader = "../shaders/pervertexlighting.vert";
std::string const defaultFragmentShader = "../shaders/pervertexlighting.frag";

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

std::string getShaderString(GLenum ShaderType, int shaderIndex, char** argv)
{
    std::fstream shaderStream;  
    std::string streamOpenString;

    if (shaderIndex != -1)
    { 
        streamOpenString = argv[shaderIndex];
    }

    shaderStream.open(streamOpenString, std::fstream::in);

    if (!shaderStream)
    {
        if (shaderIndex != -1)
        {
            std::cout << "Failed to open Shader File: " << std::endl 
                      << streamOpenString << std::endl << std::endl
                      << "Trying Default Shader instead..." << std::endl;
        }

        if(ShaderType == GL_VERTEX_SHADER)
        {
        streamOpenString = defaultVertexShader;

        }

        if (ShaderType == GL_FRAGMENT_SHADER)
        {
        streamOpenString = defaultFragmentShader;
        }          

        shaderStream.open(streamOpenString, std::fstream::in);

        if (!shaderStream) //if default directory fails
        {
            std::cout << "Failed to open default shader: " << streamOpenString << std::endl;
        }

        else if (shaderIndex != -1)
        {
            std::cout << "Default Shader load successful." << std::endl;
        }
    }

    std::string shaderString;
    std::stringstream buffer;

    buffer << shaderStream.rdbuf();
    shaderString = buffer.str();    

    shaderStream.close();

    return shaderString;
}


// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, int shaderIndex, char** argv)
{
  std::string s = getShaderString(ShaderType, shaderIndex, argv);

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, std::string shaderFile)
{
  std::fstream shaderStream;  
  shaderStream.open(shaderFile, std::fstream::in);

  if (!shaderStream)
  {
    std::cout << "Error opening shader file: " << shaderFile << std::endl;

    return false;
  }

  std::string shaderString;
  std::stringstream buffer;

  buffer << shaderStream.rdbuf();
  shaderString = buffer.str();  

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = shaderString.c_str();
  GLint Lengths[1] = { (GLint)shaderString.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}



// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  //Get the uniforms for this current shader
  m_modelMatrix = GetUniformLocation("model_matrix");
  m_viewMatrix = GetUniformLocation("view_matrix");
  m_projectionMatrix = GetUniformLocation("projection_matrix");
  m_ambient = GetUniformLocation("ambient");
  m_diffuse_albedo = GetUniformLocation("diffuse_albedo");
  m_specular_albedo = GetUniformLocation("specular_albedo");
  m_specular_power = GetUniformLocation("specular_power");
  m_lightPosition = GetUniformLocation("light_position");
  
  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location < 0) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

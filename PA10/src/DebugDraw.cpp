#include "DebugDraw.h"

mDebugDraw::mDebugDraw()
{

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3) * 2, &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2, &Indices[0], GL_STATIC_DRAW);




    m_shaderProg = glCreateProgram();
 

    std::string vertexString = "../shaders/passthru.vert";
    std::string fragmentString = "../shaders/passthru.frag";

    std::fstream shaderStreamV, shaderStreamF; 
    shaderStreamV.open(vertexString, std::fstream::in);
    shaderStreamF.open(fragmentString, std::fstream::in);

    std::stringstream bufferV, bufferF;
    std::string shaderStringV, shaderStringF;

    bufferV << shaderStreamV.rdbuf();
    shaderStringV = bufferV.str();    
    bufferF << shaderStreamF.rdbuf();
    shaderStringF = bufferF.str();    

    GLuint ShaderObjV = glCreateShader(GL_VERTEX_SHADER);
    GLuint ShaderObjF = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* v[1];
    v[0] = shaderStringV.c_str();
    GLint LengthsV[1] = { (GLint)shaderStringV.size() };

    const GLchar* f[1];
    f[0] = shaderStringF.c_str();
    GLint LengthsF[1] = { (GLint)shaderStringF.size() };


    glShaderSource(ShaderObjV, 1, v, LengthsV);
    glShaderSource(ShaderObjF, 1, f, LengthsF);

    glCompileShader(ShaderObjV);
    glCompileShader(ShaderObjF);


    GLint success;
    glGetShaderiv(ShaderObjV, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
      GLchar InfoLog[1024];
      glGetShaderInfoLog(ShaderObjV, 1024, NULL, InfoLog);
      std::cerr << "Error compiling: " << InfoLog << std::endl;
    }

    glGetShaderiv(ShaderObjF, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
      GLchar InfoLog[1024];
      glGetShaderInfoLog(ShaderObjF, 1024, NULL, InfoLog);
      std::cerr << "Error compiling: " << InfoLog << std::endl;
    }

    glAttachShader(m_shaderProg, ShaderObjV);
    glAttachShader(m_shaderProg, ShaderObjF);




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


    //Get the uniforms for this current shader
    m_viewMatrix = glGetUniformLocation(m_shaderProg, "view_matrix");
    m_projectionMatrix = glGetUniformLocation(m_shaderProg, "projection_matrix");

    Indices.push_back(0);
    Indices.push_back(1);
}

mDebugDraw::~mDebugDraw()
{

}

void mDebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
  Vertices.clear();
  Vertices.push_back(from);
  Vertices.push_back(to);

  glUseProgram(m_shaderProg);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VB);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(btVector3), 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_LINES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  
}

void mDebugDraw::setDebugMode(int debugModeIn)
{
    debugMode = debugModeIn;
}

void mDebugDraw::reportErrorWarning(const char* warningString)
{

}

void mDebugDraw::draw3dText(const btVector3& location, const char* textString)
{

}

void mDebugDraw::drawContactPoint(const btVector3& PointOnB, 
                    const btVector3 & normalOnB, 
                    btScalar distance, 
                    int lifeTime, 
                    const btVector3& color)
{

}

void mDebugDraw::render()
{

}

void mDebugDraw::setUniforms(glm::mat4 proj, glm::mat4 view)
{
    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(proj)); 
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(view)); 
}
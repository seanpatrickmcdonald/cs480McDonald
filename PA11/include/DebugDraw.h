#ifndef DEBUG_DRAW_H_
#define DEBUG_DRAW_H_

#include "graphics_headers.h"
#include <fstream>
#include <iostream>

class mDebugDraw : public btIDebugDraw
{
public:
    mDebugDraw();
    ~mDebugDraw();

    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
    int getDebugMode() const { return debugMode;}
    void setDebugMode(int debugModeIn);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3& location, const char* textString);
    void drawContactPoint(const btVector3& PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3& color);

    GLuint m_shaderProg;
    std::vector<btVector3> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;

    void render();
    void setUniforms(glm::mat4 proj, glm::mat4 view);

    int debugMode = 0;
};

#endif

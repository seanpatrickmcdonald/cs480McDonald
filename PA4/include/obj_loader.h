#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP


#include <vector>
#include "graphics_headers.h"
#include <fstream>

class ObjLoader
{
public:
	ObjLoader();
	ObjLoader(std::string objFile);
	~ObjLoader();

	void loadObjFile();

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

private:
	Vertex getVertexFromLine(std::string vertexString);
	void getIndexFromLine(std::string indexLine);
};


#endif
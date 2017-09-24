#include "obj_loader.h"


Vertex ObjLoader::getVertexFromLine(std::string vertexString)
{
	glm::vec3 color;

	if (Vertices.size() == 14 || Vertices.size() == 15 ||Vertices.size() == 18 ||Vertices.size() == 19)
	{		
		color = glm::vec3(1.0f, 0.65f, 0.0f);
	}

	else
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);		
	}


	int a = 0; int b = 0; int c = 0;
	std::string x, y, z;

	for(int i = 2; i < vertexString.length(); i++)
	{
		if (a == 0)
		{
			x.push_back(vertexString[i]);
		}

		else if (b == 0)
		{
			y.push_back(vertexString[i]);
		}

		else if (c == 0)
		{
			z.push_back(vertexString[i]);
		}

		if (vertexString[i] == ' ')
		{
			if (a == 0)
			{
				a = 1;
			}

			else if (b == 0)
			{
				b = 1;
			}

			else if (c == 0)
			{
				c = 1;
			}

			continue;
		}
	}

	return Vertex(glm::vec3(stof(x), stof(y), stof(z)), color);
}


void ObjLoader::getIndexFromLine(std::string indexLine)
{
	std::string intString;
	int count;

	for(int i = 2; i < indexLine.length(); i++)
	{		
		if (indexLine[i] == ' ')
		{
			Indices.push_back(stoi(intString));
			intString.clear();
			continue;
		}

		else if (i == indexLine.length() - 1)
		{
			intString.push_back(indexLine[i]);
			Indices.push_back(stoi(intString));
			continue;
		}

		intString.push_back(indexLine[i]);
	}
}

ObjLoader::ObjLoader()
{

}

ObjLoader::ObjLoader(std::string objFile)
{
	std::fstream objBuffer;
	objBuffer.open(objFile, std::ios::in);

	std::vector<std::string> objFileVector;



	if (!objBuffer)
	{
		std::cout << "oops" << std::endl;
	}

	else
	{
		while(!objBuffer.eof())
		{
			std::string dummyString;
			getline(objBuffer, dummyString);
			
			if (dummyString[0] == 'v' && dummyString[1] == ' ')
			{
				Vertices.push_back(getVertexFromLine(dummyString));
			}

			else if (dummyString[0] == 'f')
			{
				getIndexFromLine(dummyString);
			}
		}
		/*
		for (int i = 0; i < Vertices.size(); i++)
		{
			std::cout << Vertices[i].vertex.x << ' '
			<< Vertices[i].vertex.y << ' '
			<< Vertices[i].vertex.z << std::endl;
		}

		for (int i = 0; i < Indices.size(); i = i + 3)
		{
			std::cout << Indices[i] << ' '
			<< Indices[i + 1] << ' '
			<< Indices[i + 2] << std::endl;
		}
		*/
	}
	
	
}

ObjLoader::~ObjLoader()
{

}
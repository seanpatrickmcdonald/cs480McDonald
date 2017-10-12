#include <fstream> // used for file input
#include <iostream> // used for I/O
#include <string> // used for string operations


void Object::readFromConfig(char* configPath, Object obj)
{
	ifstream configFile;
	float temp;
	std::string line;

	// open file
      configFile.open(configPath, ifstream::in);

         // check for open failure
         if( !configFile.is_open() )
         {
            // return failure
            return false;
         }
	int index;
	for(index = 0; index <7; index++)
	{
		configFile >> obj.obj_name;
		//check for invalid
		configFile >> obj.obj_filename;
		//check fr invalid
		configFile >> obj.tex_filename;
		//check for invalid
		configFile >> obj.parent_name;
		//check fr invalid
		configFile >> obj.rotationSpeed;
		//check for invalid
		configFile >> obj.orbitSpeed;
		//check fr invalid
		configFile >> obj.orbitRadius;
	}
		
}











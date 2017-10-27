#include "bmploader.h"


BMPLoader::BMPLoader()
{
    num_textures = 0;
}

BMPLoader::~BMPLoader()
{
    if(data)
    delete[] data;
}

int BMPLoader::loadFromFile(std::string filename)
{
    std::fstream filestream;
    filestream.open(filename, std::ios::in);
    
    if(!filestream)
    {
        std::cout.flush() << "Invalid BMP: " << filename << std::endl;
        return -1;
    }

    filestream.get(header, 54);

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cout.flush() << "Invalid BMP:" << filename << std::endl;
        return -1;
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize <= 0)
    {
        std::cout.flush() << "Invalid BMP:" << filename << std::endl;
        return -1;
    }

    data = new char[imageSize];

    for (unsigned int i = 0; i < imageSize; i++)
    {
	data[i] = filestream.get();
    }
 
    filestream.close();

    dataVector.push_back(data);
    num_textures++;

    return 1;
}

int BMPLoader::loadFromFile(std::vector<std::string> filenameArray)
{
    for (unsigned int i = 0; i < filenameArray.size(); i++)
    {
        if (loadFromFile(filenameArray[i]) < 0)
        {
            return -1;
        }
    }

    return 1;
}

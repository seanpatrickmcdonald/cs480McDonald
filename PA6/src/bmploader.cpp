#include "bmploader.h"


BMPLoader::BMPLoader()
{

}

BMPLoader::~BMPLoader()
{
    delete[] data;
}

int BMPLoader::loadFromFile(std::string filename)
{
    std::fstream filestream;
    filestream.open(filename, std::ios::in);
    
    if(!filestream)
    {
        std::cout << "Invalid BMP: " << filename << std::endl;
        return -1;
    }

    filestream.get(header, 54);

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cout << "Invalid BMP:" << filename << std::endl;
        return -1;
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    data = new char[imageSize];

    for (unsigned int i = 0; i < imageSize; i++)
    {
	data[i] = filestream.get();
    }
 
    filestream.close();

    return true;
}

#include "bmploader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

BMPLoader::BMPLoader()
{

}

BMPLoader::~BMPLoader()
{
    delete[] data;
}

int BMPLoader::loadFromFile(std::string filename)
{
    /*
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
    
    
    data = new unsigned char[imageSize];

    for (unsigned int i = 0; i < imageSize; i++)
    {
	data[i] = filestream.get();
    }
    */

    int x, y, n;
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename.c_str(), &x, &y, &n, 3);
    
    width = x;
    height = y;

    /*
    if (!data)
    {
      std::cout << "failed to load: " << filename << " - Loading with bmploader..." << std::endl;

      data = new unsigned char[imageSize];

      for (unsigned int i = 0; i < imageSize; i++)
      {
	data[i] = filestream.get();
      }
    }


    filestream.close();
    */

    return true;
}

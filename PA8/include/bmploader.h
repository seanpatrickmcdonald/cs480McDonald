#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class BMPLoader
{
public:

    BMPLoader();
    ~BMPLoader();

    int loadFromFile(std::string filename);
    int loadFromFile(std::vector<std::string> filenameVector);

    char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned int num_textures;

    char *data;
    std::vector<char *> dataVector;
};

#endif

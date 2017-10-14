#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <string>
#include <fstream>
#include <iostream>

class BMPLoader
{
public:

    BMPLoader();
    ~BMPLoader();

    int loadFromFile(std::string filename);

    char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;

    unsigned char *data;
};

#endif

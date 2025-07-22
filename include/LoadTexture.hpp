#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H

#include <string>
#include <vector>


GLuint loadTexture(const char* path);
GLuint loadCubemap(std::vector<std::string> faces);

#endif // LOADTEXTURE_H

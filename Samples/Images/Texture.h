#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>
#include <gdiplus.h>

class Texture
{
public:
    Texture(const wchar_t* file);
   ~Texture();

   const uint32_t* Pixels() const;
   int Width() const;
   int Height() const;

private:
    int width, height;
    std::vector<unsigned char> pixels;
};
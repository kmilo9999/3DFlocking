#pragma once
#include <string>
#include <stdint.h>

using namespace std;

class Image
{
public:
	enum Format {
		Format_GreyScale = 1,
		Format_GreyScaleAlpha,
		Format_RGB,
		Format_RGBA,

	};

	Image();
	Image(unsigned int nWidth,unsigned int nHeight, Format nformat, uint8_t * pixels = nullptr);
	bool LoadFromFile(const char * fileName);
	unsigned int GetWidth();
	unsigned int GetHeight();
	uint8_t* GetPixels();
	uint8_t* GetPixel(int col, int row);
	
	int GetFormat(){ return format; }
	
	~Image();

private:
	Format format;
	unsigned int width;
	unsigned int height;
	uint8_t* pixels;

	void SetPixels(unsigned int width, unsigned int height, Format format, uint8_t* pixel);
};


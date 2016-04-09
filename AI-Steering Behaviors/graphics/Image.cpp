#include "Image.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image::Image():width(0), height(0), format((Format)0), pixels(nullptr)
{

}

Image::Image(unsigned int nWidth, unsigned int nHeight, Format nformat, uint8_t * pixels):width(nWidth),height(nHeight),format((Format) nformat),pixels(nullptr)
{

}


Image::~Image()
{
	if (pixels)
	{
		delete[] pixels;
	}
}

bool Image::LoadFromFile(const char * fileName) 
{
	int m_width, m_height, channels;
	uint8_t * m_pixels = stbi_load(fileName, &m_width, &m_height, &channels,0);
	if (!m_pixels)
	{
		cerr << stbi_failure_reason();
		return false;
	}

	SetPixels(m_width, m_height, (Format)channels, m_pixels);

	return true;
}

void Image::SetPixels(unsigned int nwidth, unsigned int nheight, Format nformat, uint8_t* npixels)
{
	if (nwidth == 0)
	{
		throw runtime_error("zero widht image");
	}
	if (nheight == 0)
	{
		throw runtime_error("zero height image");
	}
	if (nformat <= 0 || nformat > 4)
	{
		throw runtime_error("Invalid texture format");
	}

	int newSize = nwidth * nheight * nformat;

	if (pixels)
	{
		delete pixels;
	}

	width = nwidth;
	height = nheight;
	format = nformat;
	pixels = new uint8_t[newSize];

	if (pixels != nullptr)
	{
		memcpy(pixels, npixels, newSize);
	}

}

uint8_t* Image::GetPixel(int col, int row)
{
	if (col >= (int)width || row >= (int)height)
	{
		return nullptr;
	}
	return pixels + (row * width + col )* format;
}


unsigned int Image::GetWidth()
{
	return width;
}

unsigned int Image::GetHeight()
{
	return height;
}

uint8_t* Image::GetPixels()
{
	return pixels;
}


#pragma once
#include "RawModel.h"
#include "Texture.h"
#include "Loader.h"
#include "Image.h"

#define TERRAIN_SIZE 120
#define MAX_HEIGH 40

class Terrain
{
public:
	Terrain(float x, float z,char* heightMap);
	~Terrain();

	TexturedModel* GetModel();

	float GetX();
	float GetZ();

	float GetHeight(float x, float z, Image& heightMap);

private:

	float X;
	float Z;
	char* HeightMap;
	TexturedModel* Model;
	RawModel* GenerateTerrain();
	


};


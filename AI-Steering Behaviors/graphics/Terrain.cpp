#include <iostream>
#include "Terrain.h"
#include "Image.h"

#define MAX_PIXEL_COLOR 256 * 256 * 256

Terrain::Terrain(float x, float z, char* heightMap) : X(x*TERRAIN_SIZE), Z(z*TERRAIN_SIZE), HeightMap(heightMap)
{
	Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/terrain.jpg");
	texture->type = "diffuse";
	vector<Texture*> textures;
	textures.push_back(texture);
	Model = new TexturedModel(GenerateTerrain(),textures);
	
	
}


Terrain::~Terrain()
{
	if (Model)
	{
		delete Model;
	}
	
}


RawModel* Terrain::GenerateTerrain()
{
	Image heighMap;
	if (!heighMap.LoadFromFile(HeightMap))
	{
		cerr << "Image could not be load " << endl;
		return 0;
	}

	int VERTEX_COUNT = heighMap.GetHeight();
	int count = VERTEX_COUNT * VERTEX_COUNT;
	int size3 = count * 3;
	int size2 = count * 2;

	vector< vec3 > vVertexData;
	vector< vec3> vNormalsData;
	vector< vec2 > vCoordsData;
	vector< int > indices;
	
	int vertexPointer = 0;
	
	float halfW = -((float)TERRAIN_SIZE / (float)2);
	float move = (float)TERRAIN_SIZE / (float)(VERTEX_COUNT -1 );

	Image image;

	if (!image.LoadFromFile(HeightMap))
	{
		cerr << "Image could not be load " << endl;
		return 0;
	}


	for (float i = 0; i< VERTEX_COUNT; i++){
		
		float halfL = -((float)TERRAIN_SIZE / (float)2);
		for (float j = 0; j < VERTEX_COUNT; j++){
			

			float vx = ((float)halfL / ((float)VERTEX_COUNT - 1)) * TERRAIN_SIZE;
			float vy = 0; // GetHeight(i, j, heighMap);
			float vz = ((float)halfW / ((float)VERTEX_COUNT - 1))* TERRAIN_SIZE;
			vec3 vertex(vx, vy, vz);
			vVertexData.push_back(vertex);
			halfL += move;


			float nx = 0.0f;
			float ny = 1.0f;
			float nz = 0.0f;
			vec3 normal(vx, vy, vz);
			vNormalsData.push_back(normal);

			float uvx = (float)j / ((float)VERTEX_COUNT - 1);
			float uvy = (float)i / ((float)VERTEX_COUNT - 1);
			vec2 uv(uvx, uvy);
			vCoordsData.push_back(uv);



		}
		halfW += move;
	}

	int pointer = 0;
	for (int gz = 0; gz<VERTEX_COUNT - 1; gz++){
		for (int gx = 0; gx<VERTEX_COUNT - 1; gx++){
			int topLeft = (gz*VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back( topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}	
	

	return Loader::GetInstance()->LoadToVAO(vVertexData, vCoordsData, vNormalsData, indices);
	
}

TexturedModel* Terrain::GetModel()
{
	return Model;
}

float Terrain::GetX()
{
	return X;
}

float Terrain::GetZ()
{
	return Z;
}

float Terrain::GetHeight(float x, float z, Image& heightMap)
{
	if (x < 0 || x >= heightMap.GetWidth() || z < 0 || z >= heightMap.GetHeight())
	{
		return 0;
	}
	uint8_t* height = heightMap.GetPixel(x, z);
	//cout << (float)*height << endl;
	return (float)*height ;
	
	/*float heighValue =  (float)*height;
	heighValue += (MAX_PIXEL_COLOR / 2);
	heighValue /= (MAX_PIXEL_COLOR / 2);
	heighValue *= MAX_HEIGH;
	return heighValue;*/

}
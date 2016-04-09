#pragma once
#include "RawModel.h"
#include "Common.hpp"
#include "Texture.h"
#include "TexturedModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
using namespace std;
class Loader
{
public:
	
	static Loader* GetInstance();
	~Loader();

	RawModel* LoadToVAO(const vector<vec3>& position, const vector<vec2>& texCoordinates,  const vector<vec3>& normals,	const vector<int>& indexes);
	vector<TexturedModel*> LoadToFromFile(char* fileName);
	

private:
	
	Loader();
	int CreateVAO();

	void StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec3>  data);
	void StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec2>  data);
	void BindIndicesBuffer(vector<int> indexes);

	void ProcessNode(aiNode* node, const aiScene* scene, vector<TexturedModel*>& models);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, vector<TexturedModel*>& models);

	void UnbindVAO();
	static Loader* instance;
};


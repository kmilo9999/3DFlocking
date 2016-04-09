#pragma once
#include "RawModel.h"
#include "Texture.h"
#include "Common.hpp"
#include "ShadingTechnique.h"





class TexturedModel
{
public:
	TexturedModel();
	TexturedModel(RawModel* model, vector<Texture*> texture);
	
	~TexturedModel();

	RawModel* GetRawModel();
	Texture* GetTexture(int pos);

	void Addtexture(Texture* texture);

	void Draw();

	void Bind();

	void UnBind();

	vec4& GetDiffuseColor();
	vec4& GetSpecularColor();
	vec4& GetAmbientColor();

	void SetDiffuseColor(vec4& diffuse);
	void SetSpecularColor(vec4& specluar);
	void SetAmbientColor(vec4& ambient);


	float GetShininess();
	void SetShininess(float shininess);

	bool HasTextures();

	TexturedModel* Clone();

	void SetShader(ShadingTechnique* shader);
	ShadingTechnique* GetShader();

	void SetId(string id);
	string GetId();

private:
	
	RawModel* Model;
	vector<Texture*> textures;

	vec4 DiffuseColor;
	vec4 SpecularColor;
	vec4 AmbientColor;

	float Shininess;

	ShadingTechnique* Shader;

	void LoadVariables();

	string id;
};


struct	TexturedModelCompare
{
	bool operator() (TexturedModel* lhs, TexturedModel* rhs) const
	{
		return lhs->GetRawModel()->GetVao() < rhs->GetRawModel()->GetVao();
	}
};
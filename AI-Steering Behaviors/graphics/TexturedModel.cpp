#include "TexturedModel.h"
#include "Common.hpp"

TexturedModel::TexturedModel()
{

}

TexturedModel::TexturedModel(RawModel* model, vector<Texture*> ntexture) :Model(model), textures(ntexture), Shader(nullptr), Shininess(10)
{
	

	for (size_t i = 0; i < textures.size(); ++i)
	{
		if (!textures[i]->Load())
		{
			printf("Error loading texture '%s'\n", textures[i]->m_fileName);
			
		}
		else
		{
			printf("%s - loaded texture \n", textures[i]->m_fileName);
		}
	}
	
}


TexturedModel::~TexturedModel()
{
	delete Model;
	for (size_t i = 0; i < textures.size(); ++i)
	{
		delete textures[i];
	}

	if (Shader)
	{
		delete Shader;
	}

}

RawModel* TexturedModel::GetRawModel()
{
	return Model;
}

Texture* TexturedModel::GetTexture(int pos)
{
	return textures[pos];
}

void TexturedModel::Addtexture(Texture* texture)
{
	textures.push_back(texture);
}

vec4& TexturedModel::GetDiffuseColor()
{
	return DiffuseColor;
}

vec4& TexturedModel::GetSpecularColor()
{
	return SpecularColor;
}

void TexturedModel::SetDiffuseColor(vec4& diffuse)
{
	DiffuseColor = diffuse;
}

void TexturedModel::SetSpecularColor(vec4& specular)
{
	SpecularColor = specular;
}


float TexturedModel::GetShininess()
{
	return Shininess;
}

void TexturedModel::SetShininess(float shininess)
{
	Shininess = shininess;
}

vec4& TexturedModel::GetAmbientColor()
{
	return AmbientColor;
}

void TexturedModel::SetAmbientColor(vec4& ambient)
{
	AmbientColor = ambient;
}

bool TexturedModel::HasTextures()
{
	return textures.size() > 0 ; 
}

TexturedModel* TexturedModel::Clone()
{
	return  new TexturedModel(Model, textures);
}

void TexturedModel::SetShader(ShadingTechnique* shader)
{
	Shader = shader;
}
ShadingTechnique* TexturedModel::GetShader()
{
	return Shader;
}


void TexturedModel::Bind()
{
	LoadVariables();
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glBindVertexArray(Model->GetVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(0);
	for (size_t i = 0; i < textures.size(); ++i)
	{
			textures[i]->Bind(GL_TEXTURE0 + i);
	}
	
}

void TexturedModel::Draw()
{
	glDrawElements(GL_TRIANGLES, Model->GetVertexCount(), GL_UNSIGNED_INT, 0);
}

void TexturedModel::UnBind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glActiveTexture(0);
	glBindVertexArray(0);
}

void TexturedModel::LoadVariables()
{
	bool hasTextures = false;
	bool hasDiffuseMap = false;
	bool hasSpecularMap = false;

	map<string, mytypes> variables;
	variables["ambient"].vector4 = AmbientColor;
	variables["diffuse"].vector4 = DiffuseColor;
	variables["specular"].vector4 = SpecularColor;
	variables["shininess"].floating = Shininess;

	if (HasTextures())
	{
		hasTextures = true;
		
		
		if (textures[0]->type.compare("diffuse") == 0)
		{
				variables["diffuseMap"].integer = 0;
				hasDiffuseMap = true;

		}
		
		
		if (textures.size() > 1)
		{
			if (textures[1]->type.compare("specular") == 0)
			{
				variables["specularMap"].integer = 1;
				hasSpecularMap = true;

			}
		}
		
		
	}
	variables["isTextured"].boolean = hasTextures;
	variables["hasDiffusMap"].boolean = hasDiffuseMap;
	variables["hasSpecularMap"].boolean = hasSpecularMap;
	Shader->LoadVariables(variables);



}

void TexturedModel::SetId(string id)
{
	this->id = id;
}

string TexturedModel::GetId()
{
	return id;
}
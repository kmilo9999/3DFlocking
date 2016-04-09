#pragma once
class RawModel
{
public:
	RawModel();
	RawModel(int vao, int vertexcount);
	~RawModel();

	int GetVao();
	int GetVertexCount();

private:
	int VAO;
	int VertexCount;
};


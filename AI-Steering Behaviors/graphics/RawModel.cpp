#include "RawModel.h"


RawModel::RawModel()
{
}


RawModel::~RawModel()
{
}


RawModel::RawModel(int vao, int vertexcount) :VAO(vao), VertexCount(vertexcount)
{
	
}


int 
RawModel::GetVao()
{
	return VAO;
}


int 
RawModel::GetVertexCount()
{
	return VertexCount;
}
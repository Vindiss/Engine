#include "Vertex.h"

Core::CoreType::Vertex::Vertex(float _x, float _y, float _z, float _u, float _v)
{
	data[0] = _x;
	data[1] = _y;
	data[2] = _z;
	data[3] = _u;
	data[4] = _v;
}

Core::CoreType::Vertex::Vertex(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a)
{
	data[0] = _x;
	data[1] = _y;
	data[2] = _z;
	data[3] = _u;
	data[4] = _v;
	data[5] = _r;
	data[6] = _g;
	data[7] = _b;
	data[8] = _a;
}

Core::CoreType::Vertex::Vertex(float _x, float _y, float _z, float _u, float _v, DirectX::XMVECTORF32 _color)
{
	data[0] = _x;
	data[1] = _y;
	data[2] = _z;
	data[3] = _u;
	data[4] = _v;
	data[5] = _color[0];
	data[6] = _color[1];
	data[7] = _color[2];
	data[8] = _color[3];
}

void Core::CoreType::Vertex::SetLocation(float _x, float _y, float _z) 
{
	data[0] = _x;
	data[1] = _y;
	data[2] = _z;
}

void Core::CoreType::Vertex::SetTextureCoord(float _u, float _v)
{
	data[3] = _u;
	data[4] = _v;
}

void Core::CoreType::Vertex::SetColor(DirectX::XMVECTORF32 _color) 
{
	data[5] = _color[0];
	data[6] = _color[1];
	data[7] = _color[2];
	data[8] = _color[3];
}

void Core::CoreType::Vertex::SetColor(float _r, float _g, float _b, float _a)
{
	data[5] = _r;
	data[6] = _g;
	data[7] = _b;
	data[8] = _a;
}
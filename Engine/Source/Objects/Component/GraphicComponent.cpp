#include "GraphicComponent.h"
#include "Objects/GameObject.h"
#include "Modules/Render/Renderer.h"

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Vertex _vertexs_array[], unsigned int _vertexs_count, unsigned int _indexs_array[], unsigned int _indexs_count)
{
	mesh = new Core::CoreType::Mesh(_vertexs_array, _vertexs_count, _indexs_array, _indexs_count);
}

Objects::Component::GraphicComponent::~GraphicComponent() 
{
	auto renderer = Engine::GetInstance()->GetModule<Module::Render::Renderer>()->GetRenderer();
	renderer->DeleteGraphicObjects(id.GetInstanceNumber());
	delete mesh;
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Vertex _vertexs_array[], unsigned int _vertexs_count, DirectX::XMVECTORF32 _color, unsigned int _indexs_array[], unsigned int _indexs_count)
{
	mesh = new Core::CoreType::Mesh(_vertexs_array, _vertexs_count, _indexs_array, _indexs_count);
	color[0] = _color[0];
	color[1] = _color[1];
	color[2] = _color[2];
	color[3] = _color[3];
	mesh->SetColor(_color);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Vertex _vertexs_array[], unsigned int _vertexs_count, float _r, float _g, float _b, float _a, unsigned int _indexs_array[], unsigned int _indexs_count) 
{
	mesh = new Core::CoreType::Mesh(_vertexs_array, _vertexs_count, _indexs_array, _indexs_count);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Mesh _mesh)
{
	mesh = new Core::CoreType::Mesh(_mesh);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Mesh _mesh, DirectX::XMVECTORF32 _color)
{
	mesh = new Core::CoreType::Mesh(_mesh);
	color[0] = _color[0];
	color[1] = _color[1];
	color[2] = _color[2];
	color[3] = _color[3];
	mesh->SetColor(_color);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::Mesh _mesh, float _r, float _g, float _b, float _a)
{
	mesh = new Core::CoreType::Mesh(_mesh);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type)
{
	mesh = new Core::CoreType::Mesh(_mesh_type);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, DirectX::XMVECTORF32 _color)
{
	mesh = new Core::CoreType::Mesh(_mesh_type);
	color[0] = _color[0];
	color[1] = _color[1];
	color[2] = _color[2];
	color[3] = _color[3];
	mesh->SetColor(_color);
}

Objects::Component::GraphicComponent::GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, float _r, float _g, float _b, float _a)
{
	mesh = new Core::CoreType::Mesh(_mesh_type);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

bool Objects::Component::GraphicComponent::Init()
{
	return true;
}

bool Objects::Component::GraphicComponent::Start()
{
	return true;
}

bool Objects::Component::GraphicComponent::Update()
{
	return true;
}

bool Objects::Component::GraphicComponent::Destruct()
{
	return true;
}

Core::CoreType::Transform Objects::Component::GraphicComponent::GetTransform() const
{
	return gameObject->GetTransform();
}

Core::CoreType::Transform* Objects::Component::GraphicComponent::GetTransformReference() const
{
	return gameObject->GetTransformReference();
}

Core::CoreType::Mesh* Objects::Component::GraphicComponent::GetMesh() const
{
	return mesh;
}

Core::CoreType::Id Objects::Component::GraphicComponent::GetId() const
{
	return id;
}

const float* Objects::Component::GraphicComponent::GetColor() const
{
	return color;
}

void Objects::Component::GraphicComponent::SetColor(DirectX::XMVECTORF32 _color)
{
	color[0] = _color[0];
	color[1] = _color[1];
	color[2] = _color[2];
	color[3] = _color[3];
	mesh->SetColor(_color);
}

void Objects::Component::GraphicComponent::SetColor(float _r, float _g, float _b, float _a) 
{
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

void Objects::Component::GraphicComponent::SetMesh(Core::CoreType::Vertex _vertexs_array[], unsigned int _vertexs_count, float _r, float _g, float _b, float _a, unsigned int _indexs_array[], unsigned int _indexs_count)
{
	mesh = new Core::CoreType::Mesh(_vertexs_array, _vertexs_count, _indexs_array, _indexs_count);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

void Objects::Component::GraphicComponent::SetMesh(Core::CoreType::Mesh _mesh, float _r, float _g, float _b, float _a)
{
	mesh = new Core::CoreType::Mesh(_mesh);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}

void Objects::Component::GraphicComponent::SetMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, float _r, float _g, float _b, float _a)
{
	mesh = new Core::CoreType::Mesh(_mesh_type);
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
	color[3] = _a;
	mesh->SetColor(_r, _g, _b, _a);
}
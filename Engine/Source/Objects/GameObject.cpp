#include "GameObject.h"
#include "Modules/Render/Renderer.h"
#include <vector>
#include <string>
#include <locale>
#include <iostream>
#include <codecvt>
#include <regex>

Objects::GameObject::GameObject(Core::Interface::IScene* scene)
{
	scene->AddGameObject(this);
	SetName("Unamed GameObject");
}

Objects::GameObject::GameObject(Core::Interface::IScene* scene, Objects::GameObject* other)
{
	scene->AddGameObject(this);
	size_t pos = other->GetName().find('(');
	SetName(other->GetName().substr(0, pos));
	transform = other->GetTransform();
	texture_path = other->GetTexturePath();
	Objects::Component::GraphicComponent* graphics = other->GetComponent<Objects::Component::GraphicComponent>();
	if (graphics != nullptr) {
		const float* color = graphics->GetColor();
		std::string other_mesh_path = graphics->GetMesh()->GetMeshPath();
		other_mesh_path.erase(std::remove(other_mesh_path.begin(), other_mesh_path.end(), '\"'), other_mesh_path.end());
		MakeMesh(other_mesh_path, color[0], color[1], color[2], color[3]);
	}
}

Objects::GameObject::~GameObject() 
{
	for (auto* child : *childs) {
		delete child;
	}
	delete childs;

	for (auto* component : *components) {
		delete component;  
	}
	delete components;

	if (parent != nullptr) {
		auto parent_child = parent->GetChild();
		parent_child->erase(std::find(parent_child->begin(), parent_child->end(), this));
	}
}


std::string Objects::GameObject::GetName()
{
	return name;
}

Core::CoreType::Id Objects::GameObject::GetId() const
{
	return id;
}

Objects::GameObject* Objects::GameObject::GetParent() const 
{
	return parent;
}

Core::CoreType::Transform Objects::GameObject::GetTransform() const
{
	return transform;
}

Core::CoreType::Transform* Objects::GameObject::GetTransformReference()
{
	return &transform;
}

std::vector<Objects::GameObject*>* Objects::GameObject::GetChild() 
{
	return childs;
}

std::string Objects::GameObject::GetTexturePath() const
{
	return texture_path;
}

void Objects::GameObject::SetTransform(const Core::CoreType::Transform _transform)
{
	transform = _transform;
}

void Objects::GameObject::SetColor(DirectX::XMVECTORF32 _color)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	graphics->SetColor(_color);
	auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
	render_interface->MakeNewIndexAndVertexBuffer(graphics);
}

void Objects::GameObject::SetColor(float _r, float _g, float _b, float _a)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	graphics->SetColor(_r, _g, _b, _a);
	auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
	render_interface->MakeNewIndexAndVertexBuffer(graphics);
}

void Objects::GameObject::SetName(std::string _name) 
{
	std::vector<Core::Interface::IScene*>* scenes =  Engine::GetInstance()->GetSceneManager()->GetScenesVector();
	bool found = false;
	std::regex namepattern(R"((\w+)\(\d+\))");
	if(name == _name) {
		return;
	}
	std::smatch match;
	if (std::regex_search(_name, match, namepattern)) {
		_name = match[1].str();
	}

	std::regex pattern(_name + R"(\(\d+\))");
	for (Core::Interface::IScene* scene : *scenes)
	{
		for (Objects::GameObject* gameObject : *scene->GetGameObjects())
		{
			if (gameObject == this) {
				found = true;
				int max_nb_same_name = 0;

				for (Objects::GameObject* gameObject : *scene->GetGameObjects()) 
				{
					if (gameObject != this) {
						if (std::regex_match(gameObject->GetName(), pattern)) {
							size_t pos = gameObject->GetName().find('(');
							size_t pos2 = gameObject->GetName().find(')');
							std::string nb = gameObject->GetName().substr(pos + 1, pos2 - pos -1);
							if (std::stoi(nb) > max_nb_same_name) {
								max_nb_same_name = std::stoi(nb);
							}
						}
					}
				}
				name = _name + "(" + std::to_string(max_nb_same_name + 1) + ")";

				break;
			}
		}
		if (found) {
			break;
		}
	}
	
}

void Objects::GameObject::SetTexturePath(std::string _texture_path)
{
	texture_path = _texture_path;
}

void Objects::GameObject::AddComponent(Core::Interface::IComponent* _component)
{
	_component->Init();
	components->push_back(_component);
}

void Objects::GameObject::RemoveComponent(Core::Interface::IComponent* _component)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<Core::Interface::IComponent*>>> element = find(components->begin(), components->end(), _component);
	if (element != components->end())
	{
		components->erase(element);
	}
}

void Objects::GameObject::SetParent(GameObject* _parent)
{
	if (parent)
	{
		parent->RemoveChild(this);
		parent->SetParent(nullptr);
	}

	parent = _parent;
	parent->AddChild(this);
	transform.SetParent(_parent->GetTransformReference());
}

bool Objects::GameObject::HaveParent()
{ 
	return parent != nullptr; 
}
bool Objects::GameObject::HaveChilds()
{ 
	return childs->size() > 0; 
}

void Objects::GameObject::MakeMesh(const Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh) 
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, graphics->GetColor()[0], graphics->GetColor()[1],  graphics->GetColor()[2], graphics->GetColor()[3]);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh, DirectX::XMVECTORF32 _color) 
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh, _color);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, _color[0], _color[1], _color[2], _color[3]);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh, float _r, float _g, float _b, float _a)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh, _r, _g, _b, _a);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, _r, _g, _b, _a);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeMesh(const std::string _mesh_path)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	Core::CoreType::Mesh _mesh = Core::CoreType::Mesh(_mesh_path);
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, graphics->GetColor()[0], graphics->GetColor()[1], graphics->GetColor()[2], graphics->GetColor()[3]);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeMesh(const std::string _mesh_path, DirectX::XMVECTORF32 _color) 
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	Core::CoreType::Mesh _mesh = Core::CoreType::Mesh(_mesh_path);
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh, _color);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, _color[0], _color[1], _color[2], _color[3]);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeMesh(const std::string _mesh_path, float _r, float _g, float _b, float _a)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	Core::CoreType::Mesh _mesh = Core::CoreType::Mesh(_mesh_path);
	if (graphics == nullptr) {
		graphics = CreateComponent<Component::GraphicComponent>(_mesh, _r, _g, _b, _a);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
	else {
		graphics->SetMesh(_mesh, _r, _g, _b, _a);
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		render_interface->MakeNewIndexAndVertexBuffer(graphics);
	}
}

void Objects::GameObject::MakeTexture(std::string _texture_path)
{
	Component::GraphicComponent* graphics = GetComponent<Component::GraphicComponent>();
	texture_path = _texture_path;
	if (graphics != nullptr) {
		auto render_interface = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
		std::wstring wstr(texture_path.begin(), texture_path.end());
		render_interface->CreateBuffer(graphics, wstr.c_str());
	}
}

void Objects::GameObject::AddChild(GameObject* _child) 
{
	childs->push_back(_child);
}

void Objects::GameObject::RemoveChild(GameObject* _child) 
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<GameObject*>>> element = find(childs->begin(), childs->end(), _child);
	if (element != childs->end())
	{
		childs->erase(element);
	}
}

void Objects::GameObject::Update()
{
	for (Core::Interface::IComponent* component : *components)
	{
		component->Update();
	}
}

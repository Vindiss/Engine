#include "SceneManager.h"

bool Core::Manager::SceneManager::InitScenes()
{
	for (Core::Interface::IScene* scene : scenes)
	{
		if (!scene->Init())
		{
			return false;
		}
	}
	return true;
}

bool Core::Manager::SceneManager::StartScenes()
{
	for (Core::Interface::IScene* scene : scenes)
	{
		if (!scene->Start())
		{
			return false;
		}
	}
	return true;
}

void Core::Manager::SceneManager::UpdateScenes()
{
	for (Core::Interface::IScene* scene : scenes)
	{
		scene->Update();
	}
}


void Core::Manager::SceneManager::StopScenes()
{
	for (Core::Interface::IScene* scene : modifiedScenes) {
		ModifySceneFile(scene);
	}
	for (Core::Interface::IScene* scene : scenes)
	{
		scene->Destruct();
		delete scene;
	}
}

Core::Interface::IScene* Core::Manager::SceneManager::GetScene(const std::string& _scene_name) const
{
	for (Core::Interface::IScene* scene : scenes)
	{
		if (scene->GetName() == _scene_name)
		{
			return scene;
		}
	}
	return nullptr;
}


Core::Interface::IScene* Core::Manager::SceneManager::SetSceneByName(std::string _name, const bool _replace_scenes)
{
	if (_replace_scenes)
	{
		scenes.clear();
		Engine::GetInstance()->GetModule<Module::Render::Renderer>()->GetRenderer()->CleanGraphicsObjects();
	}
	for (Core::Interface::IScene* scene : modifiedScenes) 
	{
		if ("class " + scene->GetName() == _name) {
			scenes.push_back(scene);
			mainScene = scene;
			return scene;
		}
	}

	for (const auto& [name, createFunc] : scenesTypes) {
		if (name == _name) {
			auto scene = createFunc();
			scenes.push_back(scene);
			modifiedScenes.push_back(scene);
			mainScene = scene;
			mainScene->Init();
			if (Engine::GetInstance()->GetState() == Engine::EngineState::Running) {
				mainScene->Start();
			}
			return scene;
		}
	}
}

void Core::Manager::SceneManager::SetNewScene(Core::Interface::IScene* scene, const bool _replace_scenes)
{
	if (_replace_scenes)
	{
		scenes.clear();
		Engine::GetInstance()->GetModule<Module::Render::Renderer>()->GetRenderer()->CleanGraphicsObjects();
	}
	scenes.push_back(scene);
	modifiedScenes.push_back(scene);
	mainScene = scene;
}

bool Core::Manager::SceneManager::ModifySceneFile(Core::Interface::IScene* scene)
{
	std::string filename = scene->GetFilePath();
	std::ifstream sceneread(filename);
	std::vector<std::string> lines;
	std::regex pattern("\\bbool\\s+Start\\s*\\(\\s*\\)\\s*override\\b");
	if (sceneread.is_open()) {
		std::string line;
		while (std::getline(sceneread, line)) {
			if (std::regex_search(line, pattern)) {
				lines.push_back(line);
				std::getline(sceneread, line);
				lines.push_back(line);
				for (Objects::GameObject* gameObject : *scene->GetGameObjects()) {
					std::string name = gameObject->GetName();
					std::regex namepattern(R"((\w+)\(\d+\))");
					std::smatch match;
					if (std::regex_search(name, match, namepattern)) {
						size_t pos = name.find('(');
						size_t pos2 = name.find(')');
						std::string numero = name.substr(pos + 1, pos2 - pos - 1);
						name = name.substr(0, pos ) + numero;
					}
					std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
						return std::tolower(c);
					});
					std::replace(name.begin(), name.end(), ' ', '_');
					std::string new_line = "\t\tObjects::GameObject* " + name + " = new Objects::GameObject(this);";
					lines.push_back(new_line);
					new_line = "\t\t" + name + "->SetName(\"" + gameObject->GetName() + "\");";
					lines.push_back(new_line);
					Core::CoreType::Transform transform = gameObject->GetTransform();
					new_line = "\t\t" + name + "->SetTransform(Core::CoreType::Transform(DirectX::XMFLOAT3(" + std::to_string(transform.position.x) +
						", " + std::to_string(transform.position.y) + ", " + std::to_string(transform.position.z) + "), DirectX::XMFLOAT3(" + std::to_string(transform.rotation.x) +
						", " + std::to_string(transform.rotation.y) + ", " + std::to_string(transform.rotation.z) + "), DirectX::XMFLOAT3(" + std::to_string(transform.scale.x) +
						", " + std::to_string(transform.scale.y) + ", " + std::to_string(transform.scale.z) + ")));";
					lines.push_back(new_line);
					if (gameObject->GetParent() != nullptr) {
						std::string parent_name = gameObject->GetParent()->GetName();
						if (std::regex_search(parent_name, match, namepattern)) {
							size_t pos = parent_name.find('(');
							size_t pos2 = parent_name.find(')');
							std::string numero = parent_name.substr(pos + 1, pos2 - pos - 1);
							parent_name = parent_name.substr(0, pos) + numero;
						}
						std::transform(parent_name.begin(), parent_name.end(), parent_name.begin(), [](unsigned char c) {
							return std::tolower(c);
							});
						std::replace(parent_name.begin(), parent_name.end(), ' ', '_');
						new_line = "\t\t" + name + "->SetParent(" + parent_name + ");";
						lines.push_back(new_line);
					}
					Objects::Component::GraphicComponent* graphics = gameObject->GetComponent<Objects::Component::GraphicComponent>();
					if (graphics != nullptr) {
						if (gameObject->GetTexturePath() != "EngineTest\\Assets\\Texture\\default.png") {
							std::string texture_path = gameObject->GetTexturePath();
							size_t pos = texture_path.find("\\");
							while (pos != std::string::npos) {
								texture_path.replace(pos, 1, "/");
								pos = texture_path.find("\\", pos + 1);
							}
							new_line = "\t\t" + name + "->SetTexturePath(\"" + texture_path + "\");";
							lines.push_back(new_line);
						}
						if (graphics->GetColor()[0] == 0 && graphics->GetColor()[1] == 0 && graphics->GetColor()[2] == 0 && graphics->GetColor()[3] == 0) {
							std::string mesh_path = graphics->GetMesh()->GetMeshPath();
							size_t pos = mesh_path.find("\\");
							while (pos != std::string::npos) {
								mesh_path.replace(pos, 1, "/");
								pos = mesh_path.find("\\", pos + 1);
							}
							new_line = "\t\t" + name + "->MakeMesh(" + mesh_path + ");";
							lines.push_back(new_line);
						}
						else {
							const float* color = graphics->GetColor();
							std::string mesh_path = graphics->GetMesh()->GetMeshPath();
							size_t pos = mesh_path.find("\\");
							while (pos != std::string::npos) {
								mesh_path.replace(pos, 1, "/");
								pos = mesh_path.find("\\", pos + 1);
							}
							new_line = "\t\t" + name + "->MakeMesh(" + mesh_path + ", " + std::to_string(color[0]) + ", " + std::to_string(color[1]) + ", " +
								std::to_string(color[2]) + ", " + std::to_string(color[3]) + ");";
							lines.push_back(new_line);
						}
					}

				}
				while (line != "\t\treturn true;") {
					std::getline(sceneread, line);
				}
				lines.push_back(line);
			}
			else {
				lines.push_back(line);
			}
		}
		sceneread.close();
	}
	else {
		return false;
	}

	std::ofstream scenewrite(filename);
	if (scenewrite.is_open()) {
		for (std::string line : lines) {
			scenewrite << line << std::endl;
		}
		scenewrite.close();
	}
	else {
		return false;
	}

	return true;
}

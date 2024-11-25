#pragma once
#include "Modules/Render/Renderer.h"

namespace Module {

	namespace Render {

		class Renderer;
	}
}

template<typename T>
Core::Interface::IScene* Core::Manager::SceneManager::CreateScene()
{
	Core::Interface::IScene* scene = new T();
	scenes.push_back(scene);
	return scene;
}
 

template<typename T>
void Core::Manager::SceneManager::AddSceneType()
{
	auto createFunc = std::bind(&SceneManager::CreateScene<T>, this);
	std::function<Core::Interface::IScene* ()> func = [createFunc]() {
		return createFunc();
	};
	scenesTypes.push_back(std::make_pair(typeid(T).name(), func));
}

template<typename T>
Core::Interface::IScene* Core::Manager::SceneManager::SetScene(const bool _replace_scenes)
{
	if (_replace_scenes)
	{
		scenes.clear();
		Engine::GetInstance()->GetModule<Module::Render::Renderer>()->GetRenderer()->CleanGraphicsObjects();
	}

	Core::Interface::IScene* scene = static_cast<Core::Interface::IScene*>(new T());
	scenes.push_back(scene);
	modifiedScenes.push_back(scene);

	mainScene = scene;
	mainScene->Init();
	if (Engine::GetInstance()->GetState() == Engine::EngineState::Running) {
		mainScene->Start();
	}

	return scene;
}
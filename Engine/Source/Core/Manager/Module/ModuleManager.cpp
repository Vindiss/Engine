#include "ModuleManager.h"
#include "Modules/Display/Window.h"
#include "Modules/Render/Renderer.h"
#include "Modules/Input/Inputs.h"
#include "Modules/Scene/ModuleScene.h"
#include "Modules/Time/Clock.h"


Core::Manager::ModuleManager::ModuleManager(Engine* _engine) 
{
	engine = _engine;
}


bool Core::Manager::ModuleManager::InitModule()
{
	if (!CreateModule<Module::Display::Window>()) {
		return false;
	}
	if (!CreateModule<Module::Input::Inputs>()) {
		return false;
	}
	if (!CreateModule<Module::Render::Renderer>()) {
		return false;
	}
	if (!CreateModule<Module::Time::Clock>()) {
		return false;
	}

	return true;
}

bool Core::Manager::ModuleManager::StartModules()
{
	for (Core::Interface::IModule* module : modules)
	{
		if (!module->Start())
		{
			return false;
		}
	}
	return true;
}

void Core::Manager::ModuleManager::UpdateModules()
{

	for (Core::Interface::IModule* module : modules)
	{
		module->Update();
	}
}


void Core::Manager::ModuleManager::StopModules()
{
	for (Core::Interface::IModule* module : modules)
	{
		module->Destruct();
		delete module;
	}
}


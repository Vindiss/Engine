#pragma once

template<typename T>
bool Core::Manager::ModuleManager::CreateModule()
{
	Core::Interface::IModule* module = new T();
	if (module) {
		module->SetEngineInstance(engine);
		module->Init();
		modules.push_back(module);
		return true;
	}
	return false;
}

template<typename T>
T* Core::Manager::ModuleManager::GetModule() const
{
	for (Core::Interface::IModule* module : modules)
	{
		if (typeid(*module) == typeid(T))
			return static_cast<T*>(module);
	}
	return nullptr;
}
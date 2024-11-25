#pragma once

template <typename T, class U>
bool Core::Manager::ScriptManager::AddScript()
{
	Core::Interface::IScript* script = static_cast<Core::Interface::IScript*>(new T);

	script->Init();

	scriptsToStart.push(script);
	scripts.push_back(script);

	return true;
}

template<typename T>
T* Core::Manager::ScriptManager::GetScriptByName(std::string _name)
{
	for (Core::Interface::IScript* script : scripts)
	{
		if (script->GetScriptName() == _name)
			return static_cast<T*>(script);
	}

	return nullptr;
}
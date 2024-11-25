#include "ScriptManager.h"
#include "Modules/Time/Clock.h"
#include "Engine.h"

bool Core::Manager::ScriptManager::StartScripts()
{
	while (!scriptsToStart.empty())
	{
		scriptsToStart.front()->Start();
		scriptsToStart.pop();
	}

	return true;
}

Core::Interface::IScript* Core::Manager::ScriptManager::GetIScriptByName(std::string &_name)
{
	for (Core::Interface::IScript* script : scripts)
	{
		if (script->GetScriptName() == _name)
			return script;
	}

	return nullptr;
}

void Core::Manager::ScriptManager::UpdateScripts()
{
	StartScripts();

	for (Core::Interface::IScript* script : scripts)
	{
		script->Update();
	}
}

void Core::Manager::ScriptManager::StopScripts()
{
	std::queue<Core::Interface::IScript*>().swap(scriptsToStart);

	for (Core::Interface::IScript* script : scripts)
	{
		script->Destruct();
		delete script;
	}
}


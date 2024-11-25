#include "Scripts/Script.h"

Scripts::Script::Script()
{
	clockModuleReference = Engine::GetInstance()->GetModule<Module::Time::Clock>();
	windowModuleReference = Engine::GetInstance()->GetModule<Module::Display::Window>();
	rendererModuleReference = Engine::GetInstance()->GetModule<Module::Render::Renderer>();
	inputsModuleReference = Engine::GetInstance()->GetModule<Module::Input::Inputs>();
}

Module::Time::Clock* Scripts::Script::GetClock() const
{
	
	return clockModuleReference;
}

Module::Display::Window* Scripts::Script::GetWindow() const
{
	return windowModuleReference;
}

Module::Render::Renderer* Scripts::Script::GetRenderInterface() const
{
	return rendererModuleReference;
}

Module::Input::Inputs* Scripts::Script::GetInputs() const
{
	return inputsModuleReference;
}

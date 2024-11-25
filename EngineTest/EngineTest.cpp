#include <Windows.h>
#include "Engine.h"
#include <Script.h>
#include "Assets/Script/Test.h"
#include "Assets/Scene/DefaultScene.h"


int WINAPI WinMain(HINSTANCE__* const _h_instance, HINSTANCE__* const _h_prev_instance, char* const _command_line, const int _command_show)
{

	HINSTANCE__* hinst_lib = LoadLibrary(L"Engine.dll");

	Engine* engine = Engine::GetInstance();

	engine->Init(_h_instance);

	engine->AddSceneType<DefaultScene>();

	engine->SetScene<DefaultScene>(true);

	engine->AddScript<Test>();

	engine->Start();

	FreeLibrary(hinst_lib);

	return EXIT_SUCCESS;
}

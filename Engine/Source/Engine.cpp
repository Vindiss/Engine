#include "Engine.h"

void Engine::Init(HINSTANCE _h_instance)
{
	state = EngineState::Initializing;
	
	if (_h_instance)
		h_instance = _h_instance;

	moduleManager = new Core::Manager::ModuleManager(this);
	moduleManager->InitModule();

	sceneManager = new Core::Manager::SceneManager();
	sceneManager->InitScenes();

	scriptManager = new Core::Manager::ScriptManager();
	

	state = EngineState::Ready;
}

void Engine::Start() 
{
	if (state == EngineState::Ready) {
		moduleManager->StartModules();
		sceneManager->StartScenes();
		scriptManager->StartScripts();
		state = EngineState::Starting;
	}
	if (state == EngineState::Starting)
	{
		state = EngineState::Running;
		Update();
	}

}

void Engine::Update()
{
	while (state == EngineState::Running)
	{
		if (reseau == true)
		{
			unsigned short port = 3000;

			if (isServeur)
			{
				serveur = new Reseau::Serveur();
				serveur->Start(port);
			}
			else
			{
				client = new Reseau::Client::Client();
				client->connect("127.0.0.1", port);
			}
			reseau = false;
		}
		std::thread server_thread;
		if (isServeur && serveur != nullptr)
		{
			server_thread = std::thread([this]() { serveur->Uptade(); });
		}
		moduleManager->UpdateModules();
		sceneManager->UpdateScenes();
		scriptManager->UpdateScripts();
		
		if (isServeur && serveur != nullptr)
		{
			server_thread.join();
		}
	}

	if (state == EngineState::AskToStop)
	{
		Destruct();
	}
}

void Engine::Stop()
{
	state = EngineState::AskToStop;
}

void Engine::Destruct()
{
	state = EngineState::Stoping;
	moduleManager->StopModules();
	delete moduleManager;
	moduleManager = nullptr;
	sceneManager->StopScenes();
	delete sceneManager;
	sceneManager = nullptr;
	scriptManager->StopScripts();
	delete scriptManager;
	scriptManager = nullptr;
	if (reseau) {
		if (isServeur)
		{
			serveur->Stop();
			delete serveur;
			serveur = nullptr;
		}
		else
		{
			client->disconnect();
			delete client;
			client = nullptr;
		}
	}
	CopyDirectory();
	state = EngineState::Stopped;
}

void Engine::SetIsServeurTrue()
{
	reseau = true;
	isServeur = true;
	isClient = false;
}

void Engine::SetIsServeFalse()
{
	reseau = true;
	isServeur = false;
	isClient = true;
}

void Engine::CopyDirectory() 
{
	system("g++ -o x64\\Debug\\EngineTest EngineTest\\EngineTest.cpp");

	std::string source_dir = "Engine/Config";
	std::string destination_dir1 = "";
	std::string destination_dir2 = "";
#ifdef _DEBUG
	destination_dir1 = "x64/Debug/Engine";
	destination_dir2 = "x64/Debug/Engine/Config";
#else
	destination_dir1 = "x64/Release/Engine";
	destination_dir2 = "x64/Release/Engine/Config";
#endif
	try {
		if (std::filesystem::exists(source_dir) && std::filesystem::is_directory(source_dir)) {
			if (!std::filesystem::exists(destination_dir1)) {
				std::filesystem::create_directory(destination_dir1);
				if (!std::filesystem::exists(destination_dir2)) {
					std::filesystem::create_directory(destination_dir2);
				}
			}

			for (const auto& entry : std::filesystem::directory_iterator(source_dir)) {
				std::string destination_path = destination_dir2 + "/" + entry.path().filename().string();
				std::filesystem::copy(entry.path(), destination_path, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Une erreur s'est produite : " << e.what() << std::endl;
	}



	source_dir = "EngineTest/Assets";
#ifdef _DEBUG
	destination_dir1 = "x64/Debug/EngineTest"; 
	destination_dir2 = "x64/Debug/EngineTest/Assets";
#else
	destination_dir1 = "x64/Release/EngineTest";
	destination_dir2 = "x64/Release/EngineTest/Assets";
#endif
	try {
		if (std::filesystem::exists(source_dir) && std::filesystem::is_directory(source_dir)) {
			if (!std::filesystem::exists(destination_dir1)) {
				std::filesystem::create_directory(destination_dir1);
				if (!std::filesystem::exists(destination_dir2)) {
					std::filesystem::create_directory(destination_dir2);
				}
			}

			for (const auto& entry : std::filesystem::directory_iterator(source_dir)) {
				std::string destination_path = destination_dir2 + "/" + entry.path().filename().string();
				std::filesystem::copy(entry.path(), destination_path, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Une erreur s'est produite : " << e.what() << std::endl;
	}

}

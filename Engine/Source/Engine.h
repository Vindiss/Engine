#pragma once
#include "Core/Manager/Module/ModuleManager.h"
#include "Core/Manager/Script/ScriptManager.h"
#include "Reseau/Serveur.h"
#include "Reseau/Client/Client.h"
#include <thread>
#include <future>
#include <iostream>
#include <filesystem>

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Core/Manager/Scene/SceneManager.h"

namespace Core {

	namespace Manager {

		class SceneManager;
	}
}
#endif // SCENEMANAGER_H



/**
 * @brief The main engine class responsible for managing the application lifecycle.
 */
class Engine
{
public:

	/**
	 * @brief Gets the singleton instance of the Engine class.
	 * @return The instance of the Engine.
	 */
	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	/**
	 * @brief Initializes the engine.
	 * @param _h_instance The handle to the application instance.
	 */
	void Init(HINSTANCE _h_instance = nullptr);

	/**
	 * @brief Starts the engine.
	 */
	void Start();

	/**
	 * @brief Stops the engine.
	 */
	void Stop();

	/**
	 * @brief Updates the engine.
	 */
	void Update();

	/**
	 * @brief Destructs the engine.
	 */
	void Destruct();

	enum EngineState : int {
		Off = 0,
		Initializing,
		Ready,
		Starting,
		Running,
		AskToStop,
		Stoping,
		Stopped,
	};

	/**
	 * @brief Gets the current state of the engine.
	 * @return The current state of the engine.
	 */
	EngineState GetState() const { return state; }

	/**
	 * @brief Gets the handle to the application instance.
	 * @return The handle to the application instance.
	 */
	HINSTANCE GetHInstance() const { return h_instance; }

	/**
	 * @brief Gets the SceneManager instance.
	 * @return The SceneManager instance.
	 */
	Core::Manager::SceneManager* GetSceneManager() { return sceneManager; }

	/**
	 * @brief Checks if the engine is currently running.
	 * @return True if the engine is running, otherwise false.
	 */
	bool IsRunning() const { return state == EngineState::Running; }

	/**
	 * @brief Gets whether the engine is a server.
	 * @return True if the engine is a server, otherwise false.
	 */
	bool GetIsServe() { return isServeur; }

	/**
	 * @brief Gets whether the engine is a client.
	 * @return True if the engine is a client, otherwise false.
	 */
	bool GetIsClient() { return isClient; }

	/**
	 * @brief Sets the engine to be a server.
	 */
	void SetIsServeurTrue();

	/**
	 * @brief Sets the engine to not be a server.
	 */
	void SetIsServeFalse();

	/**
	 * @brief Gets the server instance.
	 * @return The server instance.
	 */
	Reseau::Serveur* GetServe() { return serveur; }

	/**
	 * @brief Gets the client instance.
	 * @return The client instance.
	 */
	Reseau::Client::Client* GetClient() { return client; }
  
	/**
	 * @brief Copy the necessary folders into the .exe folder .
	 */
	void CopyDirectory();
	
	/**
	 * @brief Gets a pointer to the module of the specified type.
	 * @tparam T The type of the module.
	 * @return A pointer to the module.
	 */
	template<typename T>
	T* GetModule() const;

	/**
	 * @brief Adds a script to the script manager.
	 * @tparam T The type of the script.
	 */
	template <typename T>
	void AddScript() const;

	/**
	 * @brief Adds a scene type to the scene manager.
	 * @tparam T The type of the scene.
	 */
	template <typename T>
	void AddSceneType() const;

	/**
	 * @brief Sets the active scene to the scene of the specified type.
	 * @tparam T The type of the scene.
	 * @param _replace_scenes Whether to replace all existing scenes with the new scene.
	 */
	template <typename T>
	void SetScene(bool _replace_scenes = true) const;

	/**
	 * @brief Create a thread to execute the function if possible.
	 * @tparam T The return type of the function.
	 * @param _function The function to execute.
	 */
	template<typename T>
	std::future<T> ExecuteFunctionOnThread(std::function <T()> _function);

	/**
	 * @brief Wait for all threads in the vector be finished.
	 * @tparam T The return type of the threads.
	 * @param _threads_vector The vector that contains the threads.
	 */
	template<typename T>
	void WaitForThreadsEnd(std::vector<std::future<T>>& _threads_vector);

private:

	/**
	 * @brief Default constructor.
	 */
	Engine() = default;

	/**
	 * @brief Default destructor.
	 */
	~Engine() = default;

	/**
	 * @brief Singleton instance of the Engine class.
	 */
	static inline Engine* instance;

	/**
	 * @brief Handle to the ModuleManager instance.
	 */
	Core::Manager::ModuleManager* moduleManager = nullptr;

	/**
	 * @brief Handle to the SceneManager instance.
	 */
	Core::Manager::SceneManager* sceneManager = nullptr;

	/**
	 * @brief Handle to the ScriptManager instance.
	 */
	Core::Manager::ScriptManager* scriptManager = nullptr;

	/**
	 * @brief Pointer to the server instance.
	 */
	Reseau::Serveur* serveur = nullptr;

	/**
	 * @brief Whether the engine is using network.
	 */
	bool reseau = false;

	/**
	 * @brief Whether the engine is a server.
	 */
	bool isServeur = false;

	/**
	 * @brief Whether the engine is a client.
	 */
	bool isClient = false;

	/**
	 * @brief Pointer to the client instance.
	 */
	Reseau::Client::Client* client = nullptr;

	/**
	 * @brief State of the engine.
	 */
	EngineState state = EngineState::Off;

	/**
	 * @brief Handle to the application instance.
	 */
	HINSTANCE h_instance = nullptr;
  
	/**
	 * @brief Number of processor cores.
	 */
	unsigned int nbCores = std::thread::hardware_concurrency();

	/**
	 * @brief Number of threads currently running.
	 */
	  unsigned int actual_nb_threads = 1;
};

/**
 * @brief Gets a pointer to the module of the specified type.
 * @tparam T The type of the module.
 * @return A pointer to the module.
 */
template <typename T>
T* Engine::GetModule() const
{
	return moduleManager->GetModule<T>();
}

/**
 * @brief Adds a script to the script manager.
 * @tparam T The type of the script.
 */
template <typename T>
void Engine::AddScript() const
{
	scriptManager->AddScript<T>();
}

/**
 * @brief Adds a scene type to the scene manager.
 * @tparam T The type of the scene.
 */
template <typename T>
void Engine::AddSceneType() const
{
	sceneManager->AddSceneType<T>();
}

/**
 * @brief Sets the active scene to the scene of the specified type.
 * @tparam T The type of the scene.
 * @param _replace_scenes Whether to replace all existing scenes with the new scene.
 */
template <typename T>
void Engine::SetScene(bool _replace_scenes) const
{
	sceneManager->SetScene<T>(_replace_scenes);
}

/**
 * @brief Create a thread to execute the function if possible.
 * @tparam T The return type of the function.
 * @param _function The function to execute.
 */
template<typename T>
std::future<T> Engine::ExecuteFunctionOnThread(std::function <T()> _function)
{
	if(actual_nb_threads >= nbCores) {
		T result = _function();
		std::promise<T> promise;
		promise.set_value(result);
		actual_nb_threads++;
		return promise.get_future();
	}
	else {
		std::future<T> future = std::async(std::launch::async, _function);
		actual_nb_threads++;

		return future;
	}
}

/**
 * @brief Wait for all threads in the vector be finished.
 * @tparam T The return type of the threads.
 * @param _threads_vector The vector that contains the threads.
 */
template<typename T>
void Engine::WaitForThreadsEnd(std::vector<std::future<T>>& _threads_vector)
{
	for (std::future<T>& future : _threads_vector) {
		future.get();
		actual_nb_threads--;
	}
}


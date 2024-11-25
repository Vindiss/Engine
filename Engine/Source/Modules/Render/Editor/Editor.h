#pragma once
#include "Core/Interface/IEditor.h"
#include "Scripts/script.h"
#include "Objects/GameObject.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <regex>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/ImGuiFileDialog/ImGuiFileDialog.h"
#include "ImGui/imgui_stdlib.h"


namespace fs = std::filesystem;

namespace Module {

	namespace Render {

		namespace Editor {

			/**
			 * @brief The editor class responsible for handling rendering and editing functionality.
			 */
			class Editor : public Core::Interface::IEditor
			{
			public:
				Editor() = default; /**< Default constructor. */
				~Editor() = default; /**< Default destructor. */

				/**
				 * @brief Initializes the editor.
				 * @return True if initialization is successful, otherwise false.
				 */
				bool Init() override;

				/**
				 * @brief Starts the editor.
				 * @return True if starting is successful, otherwise false.
				 */
				bool Start() override;

				/**
				 * @brief Updates the editor.
				 * @return True if updating is successful, otherwise false.
				 */
				bool Update() override;

				/**
				 * @brief Destructs the editor.
				 * @return True if destruction is successful, otherwise false.
				 */
				bool Destruct() override;

			private:

				/**
				 * @brief Renders the menu bar.
				 * @param io The ImGuiIO instance.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool MenuBar(ImGuiIO& io);

				/**
				 * @brief Renders the inspector.
				 * @param io The ImGuiIO instance.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool Inspector(ImGuiIO& io);

				/**
				 * @brief Renders the project window.
				 * @param io The ImGuiIO instance.
				 * @param directory The current directory.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool Projet(ImGuiIO& io, const std::string& directory);

				/**
				 * @brief Renders the game object list.
				 * @param gameObject The game object.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool GameObjectList(Objects::GameObject* gameObject);

				/**
				 * @brief Renders the game object window.
				 * @param io The ImGuiIO instance.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool GameObjectWindow(ImGuiIO& io);

				/**
				 * @brief Opens a URL.
				 * @param url The URL to open.
				 */
				void OpenUrl(const char* url);

				/**
				 * @brief Asks for a new scene name.
				 * @param io The ImGuiIO instance.
				 * @return True if asking is successful, otherwise false.
				 */
				bool AskNewSceneName(ImGuiIO& io);

				/**
				 * @brief Creates a new scene.
				 * @param _new_scene_name The name of the new scene.
				 * @return True if creation is successful, otherwise false.
				 */
				bool CreateNewScene(std::string _new_scene_name);

				/**
				 * @brief Opens the scene menu.
				 * @param directory The directory of the scene.
				 * @return True if opening is successful, otherwise false.
				 */
				bool OpenSceneMenu(const std::string& directory);

				/**
				 * @brief Selects server or client.
				 * @param io The ImGuiIO instance.
				 * @return True if selection is successful, otherwise false.
				 */
				bool SelectServOrClient(ImGuiIO& io);

				/**
				 * @brief Renders the chat.
				 * @param io The ImGuiIO instance.
				 * @return True if rendering is successful, otherwise false.
				 */
				bool Chat(ImGuiIO& io);

				/**
				 * @brief Finds the scene class name on file.
				 * @param filename The filename.
				 * @return The name of the scene class.
				 */
				std::string FindSceneClassNameOnFile(const std::string& filename);

				/**
				 * @brief Checks for move.
				 * @return True if checking is successful, otherwise false.
				 */
				bool CheckMove();

				/**
				 * @brief Checks if mouse select game object.
				 * @param io The ImGuiIO instance.
				 * @return True if checking is successful, otherwise false.
				 */
				bool CheckIfMouseSelectGameObject(ImGuiIO& io);

				/**
				 * @brief Checks collision with 3D element.
				 * @param nearPoint The near point.
				 * @param farPoint The far point.
				 * @param mesh The mesh.
				 * @return True if collision checking is successful, otherwise false.
				 */
				bool CheckCollisionWith3DElement(DirectX::XMVECTOR nearPoint, DirectX::XMVECTOR farPoint, Core::CoreType::Mesh* mesh);

				/**
				 * @brief Intersects ray with triangle.
				 * @param rayOrigin The ray origin.
				 * @param rayDirection The ray direction.
				 * @param mesh The mesh.
				 * @return True if intersection is successful, otherwise false.
				 */
				bool IntersectRayTriangle(DirectX::FXMVECTOR rayOrigin, DirectX::FXMVECTOR rayDirection, Core::CoreType::Mesh* mesh);

				bool openCreateCubePopup = false; /**< Flag for opening create cube popup. */
				bool openCreatePlanePopup = false; /**< Flag for opening create plane popup. */
				bool openCreatePyramidePopup = false; /**< Flag for opening create pyramide popup. */
				bool openCreateSpherePopup = false; /**< Flag for opening create sphere popup. */
				bool openCreateEmptyPopup = false; /**< Flag for opening create empty popup. */

				Objects::GameObject* selectedGameObject = nullptr; /**< Pointer to the selected game object. */
				Objects::GameObject* actualGameObjectOnMenu = nullptr; /**< Pointer to the actual game object on menu. */
				Objects::GameObject* copiedGameObject = nullptr; /**< Pointer to the copied game object. */

				std::string selectedFilePath; /**< The selected file path. */
				std::string currentDirectory = "EngineTest\\Assets"; /**< The current directory. */
				std::stack<std::string> directoryStack; /**< The directory stack. */

				std::string sceneDirectory = "EngineTest\\Assets\\Scene"; /**< The scene directory. */
				std::string selectedScenePath = "EngineTest\\Assets\\Scene\\DefaultScene.h"; /**< The selected scene path. */
				Core::Manager::SceneManager* sceneManager = nullptr; /**< Pointer to the scene manager. */
				Core::Interface::IScene* selectedScene = nullptr; /**< Pointer to the selected scene. */
				bool showAskNewSceneName = false; /**< Flag for showing ask new scene name. */
				char newSceneName[128] = ""; /**< The new scene name buffer. */

				Engine* engine = nullptr; /**< Pointer to the engine instance. */
				Module::Input::Inputs* inputModule = nullptr; /**< Pointer to the input module. */
				Module::Render::Renderer* renderModule = nullptr; /**< Pointer to the render module. */
				Module::Time::Clock* TimeModule = nullptr; /**< Pointer to the time module. */
				Module::Render::Camera* activeCamera = nullptr; /**< Pointer to the active camera. */

				std::string meshCurrentDirectory = "EngineTest\\Assets\\Mesh"; /**< The current mesh directory. */
				std::stack<std::string> meshDirectoryStack; /**< The mesh directory stack. */
				std::string newMeshSelected; /**< The selected new mesh. */
				bool selectNewMeshPopup = false; /**< Flag for selecting new mesh popup. */

				std::string textureCurrentDirectory = "EngineTest\\Assets\\Texture"; /**< The current texture directory. */
				std::stack<std::string> textureDirectoryStack; /**< The texture directory stack. */
				std::string newTextureSelected; /**< The selected new texture. */
				bool selectNewTexturePopup = false; /**< Flag for selecting new texture popup. */

				std::vector<std::string> chatMessages; /**< The chat messages. */

				int choice_texture_color = 0; /**< The choice of texture color. */
				ImVec4 vec_color; /**< The color vector. */

				std::string directory = "EngineTest\\Assets\\Script\\"; /**< The script directory. */
				std::unordered_map<std::string, bool> scriptCheckState; /**< The script check state. */
				std::vector<std::string> selectedGameObjectScripts; /**< The selected game object scripts. */
				std::vector<std::string> selectedScripts; /**< The selected scripts. */
				std::vector<std::string> scripts; /**< The scripts. */
				bool selectScriptPopup = false; /**< Flag for selecting script popup. */
				bool script_open = false; /**< Flag for script open. */
				bool p_open = false; /**< Flag for p open. */
				char scriptNameBuffer[256] = {}; /**< The script name buffer. */
				char searchBuffer[256] = {}; /**< The search buffer. */

			};
		}
	}
}


class NewScene : public Core::Interface::IScene
{
public:
	/**
	 * @brief Default constructor.
	 */
	NewScene() = default;

	/**
	 * @brief Default destructor.
	 */
	~NewScene() = default;

	/**
	 * @brief Initializes the new scene.
	 * @return True if initialization is successful, otherwise false.
	 */
	bool Init() override
	{
		return true;
	}

	/**
	 * @brief Starts the new scene.
	 * @return True if starting is successful, otherwise false.
	 */
	bool Start() override
	{
		return true;
	}

	/**
	 * @brief Updates the new scene.
	 * @return True if updating is successful, otherwise false.
	 */
	bool Update() override
	{
		return true;
	}

	/**
	 * @brief Stops the new scene.
	 * @return True if stopping is successful, otherwise false.
	 */
	bool Stop() override
	{
		return true;
	}

	/**
	 * @brief Destructs the new scene.
	 * @return True if destruction is successful, otherwise false.
	 */
	bool Destruct() override
	{
		return true;
	}

private:


};

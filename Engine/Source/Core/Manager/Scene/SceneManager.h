#pragma once

#include <functional>
#include <string>
#include <memory>
#include <typeinfo>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include "Core/Interface/IScene.h"

namespace Core
{
    namespace Manager
    {
        /**
         * @brief Manages scenes in the application.
         */
        class SceneManager
        {
        public:
            /**
             * @brief Default constructor.
             */
            SceneManager() = default;

            /**
             * @brief Default destructor.
             */
            ~SceneManager() = default;

            /**
             * @brief Initialize scenes.
             * @return True if initialization is successful, false otherwise.
             */
            bool InitScenes();

            /**
             * @brief Start all scenes.
             * @return True if all scenes are started successfully, false otherwise.
             */
            bool StartScenes();

            /**
             * @brief Create a new scene of type T.
             * @tparam T The type of scene to create.
             * @return A pointer to the created scene.
             */
            template<typename T>
            Core::Interface::IScene* CreateScene();

            /**
             * @brief Update all scenes.
             */
            void UpdateScenes();

            /**
             * @brief Stop all scenes.
             */
            void StopScenes();

            /**
             * @brief Add a scene type to the manager.
             * @tparam T The type of scene to add.
             */
            template<typename T>
            void AddSceneType();

            /**
             * @brief Set the function to be called for the next scene.
             * @param _next_scene The function to be called for the next scene.
             */
            void SetNextScene(std::function<void()> _next_scene) { nextScene = _next_scene; }

            /**
             * @brief Set the current scene to the scene of type T.
             * @tparam T The type of scene to set.
             * @param _replace_scenes Flag indicating whether to replace existing scenes.
             * @return A pointer to the set scene.
             */
            template<typename T>
            Core::Interface::IScene* SetScene(bool _replace_scenes = true);

            /**
             * @brief Set the main scene.
             * @param _scene The main scene to set.
             */
            void SetMainScene(Core::Interface::IScene* _scene) { mainScene = _scene; }

            /**
             * @brief Set the scene by name.
             * @param _name The name of the scene.
             * @param _replace_scenes Flag indicating whether to replace existing scenes.
             * @return A pointer to the set scene.
             */
            Core::Interface::IScene* SetSceneByName(std::string _name, const bool _replace_scenes = true);

            /**
             * @brief Set a new scene.
             * @param scene The new scene to set.
             * @param _replace_scenes Flag indicating whether to replace existing scenes.
             */
            void SetNewScene(Core::Interface::IScene* scene, const bool _replace_scenes = true);

            /**
             * @brief Get the main scene.
             * @return A pointer to the main scene.
             */
            Core::Interface::IScene* GetMainScene() { return mainScene; }

            /**
             * @brief Get a scene by name.
             * @param _scene_name The name of the scene.
             * @return A pointer to the scene if found, nullptr otherwise.
             */
            Core::Interface::IScene* GetScene(const std::string& _scene_name) const;

            /**
             * @brief Get the function for the next scene.
             * @return The function for the next scene.
             */
            std::function<void()> GetNextScene() { return nextScene; }

            /**
             * @brief Get the vector of scenes.
             * @return A pointer to the vector of scenes.
             */
            std::vector<Core::Interface::IScene*>* GetScenesVector() { return &scenes; }

            /**
             * @brief Modify a scene file.
             * @param scene The scene to modify.
             * @return True if the scene file is successfully modified, false otherwise.
             */
            bool ModifySceneFile(Core::Interface::IScene* scene);

        private:
            std::vector<Core::Interface::IScene*> scenes; /**< Vector of scenes. */
            std::vector<std::pair<std::string, std::function<Core::Interface::IScene* ()>>> scenesTypes; /**< Vector of scene types. */
            std::vector<Core::Interface::IScene*> modifiedScenes; /**< Vector of modified scenes. */
            Core::Interface::IScene* mainScene = nullptr; /**< Pointer to the main scene. */
            std::function<void()> nextScene; /**< Function for the next scene. */
        };
    }
}

#include "SceneManager.inl"
#pragma once

#include <string>
#include <vector>
#include <algorithm>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Objects/GameObject.h"

namespace Objects {

    class GameObject;
}
#endif // GAMEOBJECT_H

namespace Core
{

    namespace Interface
    {

        /**
         * @brief Interface for defining a scene.
         */
        class IScene
        {
        public:
            /**
             * @brief Default constructor.
             */
            IScene() = default;

            /**
             * @brief Default destructor.
             */
            virtual ~IScene() = default;

            /**
             * @brief Initializes the scene.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool Init() = 0;

            /**
             * @brief Starts the scene.
             * @return True if starting is successful, false otherwise.
             */
            virtual bool Start() = 0;

            /**
             * @brief Updates the scene.
             * @return True if updating is successful, false otherwise.
             */
            virtual bool Update() = 0;

            /**
             * @brief Stops the scene.
             * @return True if stopping is successful, false otherwise.
             */
            virtual bool Stop() = 0;

            /**
             * @brief Destroys the scene.
             * @return True if destruction is successful, false otherwise.
             */
            virtual bool Destruct() = 0;

            /**
             * @brief Gets the name of the scene.
             * @return The name of the scene.
             */
            std::string GetName() const { return name; }

            /**
             * @brief Gets the file path of the scene.
             * @return The file path of the scene.
             */
            std::string GetFilePath() const { return filepath; }

            /**
             * @brief Sets the name of the scene.
             * @param _name The name to set.
             */
            void SetName(std::string _name) { name = _name; }

            /**
             * @brief Sets the file path of the scene.
             * @param _file_path The file path to set.
             */
            void SetFilePath(std::string _file_path) { filepath = _file_path; }

            /**
             * @brief Gets the vector of game objects in the scene.
             * @return A pointer to the vector of game objects.
             */
            std::vector<Objects::GameObject*>* GetGameObjects() { return &gameObjects; }

            /**
             * @brief Adds a game object to the scene.
             * @param _game_object The game object to add.
             */
            void AddGameObject(Objects::GameObject* _game_object) { gameObjects.push_back(_game_object); }

            /**
             * @brief Deletes a game object from the scene.
             * @param _game_object The game object to delete.
             */
            void DeleteGameObject(Objects::GameObject* _game_object) { gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), _game_object)); }

        private:
            std::string name = ""; /**< The name of the scene. */
            std::vector<Objects::GameObject*> gameObjects; /**< Vector of game objects in the scene. */
            std::string filepath = ""; /**< The file path of the scene. */
        };
    }
}

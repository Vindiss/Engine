/**
 * @file IComponent.h
 * @brief Defines the interface for a component in the engine.
 */

#pragma once

#include <string>

namespace Objects
{
    class GameObject;
}

namespace Core
{
    namespace Interface
    {

        /**
         * @class IComponent
         * @brief Interface for a component in the engine.
         */
        class IComponent
        {
        public:
            /**
             * @brief Default virtual destructor.
             */
            virtual ~IComponent() = default;

            /**
             * @brief Initializes the component.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool Init() = 0;

            /**
             * @brief Starts the component.
             * @return True if starting is successful, false otherwise.
             */
            virtual bool Start() = 0;

            /**
             * @brief Updates the component.
             * @return True if updating is successful, false otherwise.
             */
            virtual bool Update() = 0;

            /**
             * @brief Destructs the component.
             * @return True if destruction is successful, false otherwise.
             */
            virtual bool Destruct() = 0;

            /**
             * @brief Sets the name of the component.
             * @param _new_name The new name for the component.
             */
            void SetComponentName(std::string _new_name = "Unnamed Component") { componentName = _new_name; }

            /**
             * @brief Sets the attached game object for the component.
             * @param _game_object Pointer to the game object to which the component is attached.
             */
            void SetAttachedGameObject(Objects::GameObject* _game_object) { gameObject = _game_object; }

            /**
             * @brief Gets the name of the component.
             * @return The name of the component.
             */
            std::string GetComponentName() const { return componentName; }

            /**
             * @brief Gets the attached game object of the component.
             * @return Pointer to the attached game object.
             */
            Objects::GameObject* GameObject() const { return gameObject; }

        protected:
            Objects::GameObject* gameObject = nullptr; /**< Pointer to the attached game object. */
            std::string componentName = "Unamed Component"; /**< Name of the component. */
        };
    }
}

/**
 * @file IModule.h
 * @brief Defines the interface for a module in the engine.
 */

#pragma once

#include <string>

class Engine;

namespace Core
{
    namespace Interface
    {

        /**
         * @class IModule
         * @brief Interface for a module in the engine.
         */
        class IModule
        {
            friend class ModuleManager;

        public:
            /**
             * @brief Default destructor.
             */
            virtual ~IModule() = default;

            /**
             * @brief Initializes the module.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool Init() = 0;

            /**
             * @brief Starts the module.
             * @return True if starting is successful, false otherwise.
             */
            virtual bool Start() = 0;

            /**
             * @brief Updates the module.
             * @return True if updating is successful, false otherwise.
             */
            virtual bool Update() = 0;

            /**
             * @brief Destructs the module.
             * @return True if destruction is successful, false otherwise.
             */
            virtual bool Destruct() = 0;

            /**
             * @brief Sets the engine instance for the module.
             * @param _engine Pointer to the engine instance.
             */
            void SetEngineInstance(Engine* _engine) { engine = _engine; }

            /**
             * @brief Sets the name of the module.
             * @param _new_name The new name for the module.
             */
            void SetModuleName(std::string& _new_name) { moduleName = _new_name; }

            /**
             * @brief Gets the name of the module.
             * @return The name of the module.
             */
            std::string GetModuleName() { return moduleName; }

        protected:
            std::string moduleName; /**< The name of the module */
            Engine* engine = nullptr; /**< Pointer to the engine instance */
        };
    }
}

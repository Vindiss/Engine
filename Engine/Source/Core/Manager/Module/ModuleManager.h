#pragma once

#include <vector>
#include "Core/Interface/IModule.h"

class Engine;

namespace Core
{
    namespace Manager
    {
        /**
         * @brief Handles the management of modules for the Engine.
         */
        class ModuleManager
        {
        public:
            /**
             * @brief Constructs a ModuleManager object.
             * @param _engine Reference to the Engine instance.
             */
            explicit ModuleManager(Engine* _engine);

            /**
             * @brief Default destructor.
             */
            ~ModuleManager() = default;

            /**
             * @brief Initializes all modules.
             * @return True if initialization is successful, false otherwise.
             */
            bool InitModule();

            /**
             * @brief Starts all modules.
             * @return True if starting is successful, false otherwise.
             */
            bool StartModules();

            /**
             * @brief Creates a new module.
             * @tparam T Type of module to create.
             * @return True if the module is successfully created, false otherwise.
             */
            template<typename T>
            bool CreateModule();

            /**
             * @brief Retrieves a module by its type.
             * @tparam T Type of module to retrieve.
             * @return Pointer to the module if found, nullptr otherwise.
             */
            template<typename T>
            T* GetModule() const;

            /**
             * @brief Updates all modules.
             */
            void UpdateModules();

            /**
             * @brief Stops all modules.
             */
            void StopModules();

        private:
            /**
             * @brief Reference to the Engine instance.
             */
            Engine* engine = nullptr;

            /**
             * @brief List of modules.
             */
            std::vector<Core::Interface::IModule*> modules;
        };
    }
}

#include "ModuleManager.inl"
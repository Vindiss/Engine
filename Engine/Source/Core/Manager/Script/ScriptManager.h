#pragma once

#include <vector>
#include <queue>
#include <string>
#include "Core/Interface/IScript.h"

namespace Core
{
    namespace Manager
    {
        /**
         * @brief Manages scripts implementing the IScript interface.
         */
        class ScriptManager
        {
        public:
            /**
             * @brief Default constructor.
             */
            ScriptManager() = default;

            /**
             * @brief Destructor.
             */
            ~ScriptManager() = default;

            /**
             * @brief Add a script to the manager.
             * @tparam T The type of script to add.
             * @tparam U SFINAE to ensure T is derived from IScript.
             * @return True if the script is added successfully, false otherwise.
             */
            template <typename T, class U = typename std::enable_if<std::is_base_of<Core::Interface::IScript, T>::value>>
            bool AddScript();

            /**
             * @brief Start all scripts managed by the manager.
             * @return True if all scripts start successfully, false otherwise.
             */
            bool StartScripts();

            /**
             * @brief Get a script by its name.
             * @tparam T The type of script to get.
             * @param _name The name of the script.
             * @return A pointer to the script if found, nullptr otherwise.
             */
            template<typename T = Core::Interface::IScript>
            T* GetScriptByName(std::string _name);

            /**
             * @brief Get a script implementing the IScript interface by its name.
             * @param _name The name of the script.
             * @return A pointer to the script if found, nullptr otherwise.
             */
            Core::Interface::IScript* GetIScriptByName(std::string& _name);

            /**
             * @brief Update all scripts managed by the manager.
             */
            void UpdateScripts();

            /**
             * @brief Stop all scripts managed by the manager.
             */
            void StopScripts();

        private:
            std::vector<Core::Interface::IScript*> scripts; /**< Vector of managed scripts. */
            std::queue<Core::Interface::IScript*> scriptsToStart; /**< Queue of scripts to start. */
        };
    }
}

#include "ScriptManager.inl"
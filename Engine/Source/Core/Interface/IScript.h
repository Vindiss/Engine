#pragma once

#include <string>

namespace Core
{
    namespace Interface
    {
        /**
         * @brief Interface for scripting components.
         */
        class IScript
        {
        public:
            /**
             * @brief Default virtual destructor.
             */
            virtual ~IScript() = default;

            /**
             * @brief Initializes the script.
             */
            virtual void Init() {}

            /**
             * @brief Starts the script.
             */
            virtual void Start() {}

            /**
             * @brief Updates the script.
             */
            virtual void Update() {}

            /**
             * @brief Destructs the script.
             */
            virtual void Destruct() {}

            /**
             * @brief Sets the name of the script.
             * @param _newName New name of the script.
             */
            void SetScriptName(std::string _newName) { scriptName = _newName; }

            /**
             * @brief Retrieves the name of the script.
             * @return Name of the script.
             */
            std::string GetScriptName() const { return scriptName; }

        protected:
            /**
             * @brief Name of the script.
             */
            std::string scriptName;
        };
    }
}

/**
 * @file IEditor.h
 * @brief Defines the interface for an editor in the engine.
 */

#pragma once

namespace Core {

    namespace Interface {

        /**
         * @class IEditor
         * @brief Interface for an editor in the engine.
         */
        class IEditor
        {
        public:

            /**
             * @brief Initializes the editor.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool Init() = 0;

            /**
             * @brief Starts the editor.
             * @return True if starting is successful, false otherwise.
             */
            virtual bool Start() = 0;

            /**
             * @brief Updates the editor.
             * @return True if updating is successful, false otherwise.
             */
            virtual bool Update() = 0;

            /**
             * @brief Destructs the editor.
             * @return True if destruction is successful, false otherwise.
             */
            virtual bool Destruct() = 0;

        };

    }
}

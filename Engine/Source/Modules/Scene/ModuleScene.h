#pragma once

#include "Core/Interface/IModule.h"
#include <functional>

namespace Module
{
    namespace Scene
    {
        /**
         * @brief Module responsible for managing scenes.
         */
        class SceneModule : public Core::Interface::IModule
        {
        public:
            /**
             * @brief Default constructor.
             */
            SceneModule() = default;

            /**
             * @brief Default destructor.
             */
            ~SceneModule() = default;

            /**
             * @brief Initializes the scene module.
             * @return True if initialization is successful, false otherwise.
             */
            bool Init() override;

            /**
             * @brief Starts the scene module.
             * @return True if starting is successful, false otherwise.
             */
            bool Start() override;

            /**
             * @brief Updates the scene module.
             * @return True if updating is successful, false otherwise.
             */
            bool Update() override;

            /**
             * @brief Destructs the scene module.
             * @return True if destruction is successful, false otherwise.
             */
            bool Destruct() override;

        private:

        };
    }
}
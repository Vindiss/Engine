#pragma once

#include "Core/Interface/IModule.h"
#include <chrono>

namespace Module
{
    namespace Time
    {
        /**
         * @brief Class representing a clock module for time management.
         */
        class Clock : public Core::Interface::IModule
        {
        public:
            /**
             * @brief Default constructor.
             */
            Clock() = default;

            /**
             * @brief Destructor.
             */
            ~Clock() = default;

            /**
             * @brief Initializes the clock module.
             *
             * @return true if initialization succeeds, false otherwise.
             */
            bool Init() override;

            /**
             * @brief Starts the clock module.
             *
             * @return true if starting succeeds, false otherwise.
             */
            bool Start() override;

            /**
             * @brief Updates the clock module.
             *
             * @return true if updating succeeds, false otherwise.
             */
            bool Update() override;

            /**
             * @brief Destroys the clock module.
             *
             * @return true if destruction succeeds, false otherwise.
             */
            bool Destruct() override;

            /**
             * @brief Retrieves the time elapsed since the last frame.
             *
             * @return The time elapsed since the last frame in seconds.
             */
            float GetDeltaTime() const;

        private:
            /**
             * @brief Computes the delta time between frames.
             */
            void ComputeDeltaTime();

            std::chrono::high_resolution_clock clock; /**< Clock for time measurement. */
            std::chrono::high_resolution_clock::time_point startProgramPoint = std::chrono::high_resolution_clock::now(); /**< Start time of the program. */
            std::chrono::high_resolution_clock::time_point previousTimePoint = std::chrono::high_resolution_clock::now(); /**< Time point of the previous frame. */
            std::chrono::high_resolution_clock::time_point currentTimePoint = std::chrono::high_resolution_clock::now(); /**< Time point of the current frame. */
            std::chrono::high_resolution_clock::duration currentDeltaTime = std::chrono::high_resolution_clock::duration::zero(); /**< Delta time between frames. */

            int currentFrame = 0; /**< Current frame count. */
        };
    }
}

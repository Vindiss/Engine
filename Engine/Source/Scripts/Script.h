#pragma once

#include "Core/Interface/IScript.h"
#include "Modules/Display/Window.h"
#include "Modules/Input/Inputs.h"
#include "Modules/Render/Renderer.h"
#include "Modules/Time/Clock.h"

namespace Scripts
{
    /**
     * @brief Represents a script implementation.
     */
    class Script : public Core::Interface::IScript
    {
    public:
        /**
         * @brief Default constructor.
         */
        Script();

        /**
         * @brief Default destructor.
         */
        ~Script() = default;

        /**
         * @brief Get the clock module.
         * @return A pointer to the clock module.
         */
        Module::Time::Clock* GetClock() const;

        /**
         * @brief Get the window module.
         * @return A pointer to the window module.
         */
        Module::Display::Window* GetWindow() const;

        /**
         * @brief Get the renderer module.
         * @return A pointer to the renderer module.
         */
        Module::Render::Renderer* GetRenderInterface() const;

        /**
         * @brief Get the inputs module.
         * @return A pointer to the inputs module.
         */
        Module::Input::Inputs* GetInputs() const;

    private:
        Module::Time::Clock* clockModuleReference = nullptr; /**< Pointer to the clock module. */
        Module::Display::Window* windowModuleReference = nullptr; /**< Pointer to the window module. */
        Module::Render::Renderer* rendererModuleReference = nullptr; /**< Pointer to the renderer module. */
        Module::Input::Inputs* inputsModuleReference = nullptr; /**< Pointer to the inputs module. */
    };
}
#pragma once
#include "Engine.h"
#include "Objects/Component/GraphicComponent.h"
#include "Core/Interface/IModule.h"
#include "Camera.h"
#include "Core/Interface/IRender.h"
#include "Core/Interface/IEditor.h"
#include <unordered_map>

namespace Module {

    namespace Render {

        /**
         * @brief Enum defining the renderer API.
         */
        enum RendererAPI {
            DirectX12Api /**< DirectX 12 API. */
        };

        /**
         * @brief Enum defining the editor interface.
         */
        enum EditorInterface {
            ImGuiInterface /**< ImGui interface. */
        };

        /**
         * @brief Handles rendering graphics.
         */
        class Renderer : public Core::Interface::IModule
        {
        public:
            /**
             * @brief Initialize the renderer.
             * @return True if initialization is successful, false otherwise.
             */
            bool Init() override;

            /**
             * @brief Start the renderer.
             * @return True if starting is successful, false otherwise.
             */
            bool Start() override;

            /**
             * @brief Update the renderer.
             * @return True if updating is successful, false otherwise.
             */
            bool Update() override;

            /**
             * @brief Destruct the renderer.
             * @return True if destruction is successful, false otherwise.
             */
            bool Destruct() override;

            /**
             * @brief Create a buffer for a graphic component with a texture.
             * @param _component The graphic component.
             * @param _texture_path The path to the texture.
             * @return True if buffer creation is successful, false otherwise.
             */
            bool CreateBuffer(Objects::Component::GraphicComponent* _component, LPCWSTR _texture_path);

            /**
             * @brief Create new index and vertex buffers for a graphic component.
             * @param _component The graphic component.
             * @return True if buffer creation is successful, false otherwise.
             */
            bool MakeNewIndexAndVertexBuffer(Objects::Component::GraphicComponent* _component);

            /**
             * @brief Create a new texture buffer for a graphic component.
             * @param _component The graphic component.
             * @param _texture_path The path to the texture.
             * @return True if buffer creation is successful, false otherwise.
             */
            bool MakeNewTextureBuffer(Objects::Component::GraphicComponent* _component, LPCWSTR _texture_path);

            /**
             * @brief Get the active camera.
             * @return A pointer to the active camera.
             */
            Camera* GetActiveCamera() const;

            /**
             * @brief Get the renderer interface.
             * @return A pointer to the renderer interface.
             */
            Core::Interface::IRender* GetRenderer();

        private:

            RendererAPI api = DirectX12Api; /**< The renderer API. */

            Core::Interface::IRender* renderer = nullptr; /**< Pointer to the renderer interface. */

            EditorInterface editorInterface = ImGuiInterface; /**< The editor interface. */

            Core::Interface::IEditor* editor = nullptr; /**< Pointer to the editor interface. */

            std::unordered_map<unsigned int, Objects::Component::GraphicComponent*>* objects = new std::unordered_map<unsigned, Objects::Component::GraphicComponent*>(); /**< Map of graphic components. */

            Camera* activeCamera = new Camera(); /**< Pointer to the active camera. */
        };
    }
}

#pragma once

#include "Core/CoreType/Transform.h"
#include <string>

namespace Core
{
    namespace CoreType
    {
        struct Vertex;
    }
}

namespace Object
{
    namespace Component
    {
        class GraphicComponent;
    }
}

namespace Core
{
    namespace Interface
    {
        /**
         * @brief Interface for rendering operations.
         */
        class IRender
        {
        public:
            /**
             * @brief Default destructor.
             */
            virtual ~IRender() = default;

            /**
             * @brief Initializes the rendering system.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool Initialize() = 0;

            /**
             * @brief Creates rendering pipeline.
             * @return True if pipeline creation is successful, false otherwise.
             */
            virtual bool CreatePipeline() = 0;

            /**
             * @brief Creates vertex and index buffers.
             * @param _id The ID of the buffer.
             * @param _name The name of the buffer.
             * @param _transform The transform of the buffer.
             * @param _vertices_array The array of vertices.
             * @param _vertices_count The count of vertices.
             * @param _indexs_array The array of indices.
             * @param _indexs_count The count of indices.
             * @param _path The path to the texture.
             * @return True if buffer creation is successful, false otherwise.
             */
            virtual bool CreateBuffers(unsigned int _id, std::string _name, Core::CoreType::Transform* _transform, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array = nullptr, unsigned int _indexs_count = 0, LPCWSTR _path = L"EngineTest\\Assets\\Texture\\default.png") = 0;

            /**
             * @brief Creates new vertex and index buffers.
             * @param _id The ID of the buffer.
             * @param _name The name of the buffer.
             * @param _vertices_array The array of vertices.
             * @param _vertices_count The count of vertices.
             * @param _indexs_array The array of indices.
             * @param _indexs_count The count of indices.
             * @return True if buffer creation is successful, false otherwise.
             */
            virtual bool MakeNewIndexAndVertexBuffers(const unsigned int _id, std::string _name, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array, unsigned int _indexs_count) = 0;

            /**
             * @brief Creates new texture buffers.
             * @param _id The ID of the buffer.
             * @param _name The name of the buffer.
             * @param _path The path to the texture.
             * @return True if texture creation is successful, false otherwise.
             */
            virtual bool MakeNewTextureBuffers(const unsigned int _id, std::string _name, LPCWSTR _path) = 0;

            /**
             * @brief Sets the view matrix for the camera.
             * @param _camera_view The view matrix.
             */
            virtual void SetCameraViewMatrix(DirectX::XMFLOAT4X4 _camera_view) = 0;

            /**
             * @brief Sets the projection matrix for the camera.
             * @param _camera_projection The projection matrix.
             */
            virtual void SetCameraProjectionMatrix(DirectX::XMFLOAT4X4 _camera_projection) = 0;

            /**
             * @brief Renders the scene.
             * @return True if rendering is successful, false otherwise.
             */
            virtual bool Render() = 0;

            /**
             * @brief Cleans up the rendering system.
             * @return True if cleanup is successful, false otherwise.
             */
            virtual bool Cleanup() = 0;

            /**
             * @brief Initializes ImGui.
             * @return True if initialization is successful, false otherwise.
             */
            virtual bool InitImGui() = 0;

            /**
             * @brief Handles window resizing.
             * @param _window_param The window parameter.
             * @param _long_param The long parameter.
             * @return True if resizing is successful, false otherwise.
             */
            virtual bool WindowResize(WPARAM _window_param, LPARAM _long_param) = 0;

            /**
             * @brief Cleans up graphics objects.
             * @return True if cleanup is successful, false otherwise.
             */
            virtual bool CleanGraphicsObjects() = 0;

            /**
             * @brief Deletes graphic objects.
             * @param _id The ID of the object to delete.
             * @return True if deletion is successful, false otherwise.
             */
            virtual bool DeleteGraphicObjects(const unsigned int _id) = 0;

            /**
             * @brief Gets the viewport.
             * @return A pointer to the viewport.
             */
            virtual D3D12_VIEWPORT* GetViewport() = 0;
        };
    }
}

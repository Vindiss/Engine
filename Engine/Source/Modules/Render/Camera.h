#pragma once

#include "Core/CoreType/Transform.h"
#include <DirectXMath.h>

namespace Module {

    namespace Render {

        /**
         * @brief Class representing a camera for rendering.
         */
        class Camera
        {
        public:
            /**
             * @brief Default constructor.
             */
            Camera();

            /**
             * @brief Default destructor.
             */
            ~Camera() = default;

            /**
             * @brief Sets this camera as the active camera in rendering.
             */
            void SetAsActiveCamera();

            /**
             * @brief Retrieves the camera's transformation.
             *
             * @return The camera's transformation.
             */
            Core::CoreType::Transform GetTransform() const;

            /**
             * @brief Retrieves a reference to the camera's transformation.
             *
             * @return A reference to the camera's transformation.
             */
            Core::CoreType::Transform* GetTransformReference();

            /**
             * @brief Retrieves the camera's projection matrix.
             *
             * @return The camera's projection matrix.
             */
            DirectX::XMFLOAT4X4 GetProjectionMatrix() const;

            /**
             * @brief Retrieves the camera's view matrix.
             *
             * @return The camera's view matrix.
             */
            DirectX::XMFLOAT4X4 GetViewMatrix() const;

            /**
             * @brief Retrieves the camera's field of view.
             *
             * @return The camera's field of view.
             */
            float GetFov() const;

            /**
             * @brief Retrieves the camera's near plane distance.
             *
             * @return The camera's near plane distance.
             */
            float GetNear() const;

            /**
             * @brief Retrieves the camera's far plane distance.
             *
             * @return The camera's far plane distance.
             */
            float GetFar() const;

            /**
             * @brief Sets the camera's transformation.
             *
             * @param _transform The new transformation for the camera.
             */
            void SetTransform(Core::CoreType::Transform _transform);

            /**
             * @brief Sets the camera's field of view.
             *
             * @param _fov The new field of view for the camera.
             */
            void SetFov(float _fov);

            /**
             * @brief Sets the camera's near plane distance.
             *
             * @param _near The new near plane distance for the camera.
             */
            void SetNear(float _near);

            /**
             * @brief Sets the camera's far plane distance.
             *
             * @param _far The new far plane distance for the camera.
             */
            void SetFar(float _far);

            /**
             * @brief Rotates the camera according to the specified deltas.
             *
             * @param deltaX The rotation delta around the X axis.
             * @param deltaY The rotation delta around the Y axis.
             */
            void Rotate(float deltaX, float deltaY);

        private:

            bool isActiveCamera = false; /**< Indicates if this camera is active in rendering. */

            float width = 0; /**< Width of the camera's view. */

            float height = 0; /**< Height of the camera's view. */

            float fov = 45.f; /**< Field of view of the camera. */

            float nearRender = 0.1f; /**< Near plane distance of the camera. */

            float farRender = 1000.f; /**< Far plane distance of the camera. */

            float currentAngleX = 0.0f; /**< Current rotation angle around the X axis. */

            float currentAngleY = 0.0f; /**< Current rotation angle around the Y axis. */

            float rotationSpeed = 0.001f; /**< Camera rotation speed. */

            Core::CoreType::Transform transform; /**< Camera transformation. */

            DirectX::XMFLOAT4X4 projectionMatrix; /**< Camera's projection matrix. */

        };
    }
}

#pragma once

#include <DirectXMath.h>

namespace Core
{
    namespace CoreType
    {
        /**
         * @brief Represents a transformation in 3D space including position, rotation, and scale.
         */
        class Transform
        {
        public:
            /**
             * @brief Constructor with an optional parent transform.
             * @param _parent The parent transform.
             */
            explicit Transform(Transform* _parent = nullptr);

            /**
             * @brief Constructor with initial position, rotation, and scale.
             * @param _position The initial position.
             * @param _rotation The initial rotation.
             * @param _scale The initial scale.
             */
            explicit Transform(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _rotation, DirectX::XMFLOAT3 _scale);

            /**
             * @brief Destructor.
             */
            ~Transform() = default;

            DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f); /**< The position of the transform. */
            DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f); /**< The rotation of the transform. */
            DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);    /**< The scale of the transform. */

            /**
             * @brief Get the local transformation matrix.
             * @return The local transformation matrix.
             */
            DirectX::XMFLOAT4X4 GetLocalMatrix() const;

            /**
             * @brief Get the world transformation matrix.
             * @return The world transformation matrix.
             */
            DirectX::XMFLOAT4X4 GetWorldMatrix() const;

            /**
             * @brief Set the parent transform.
             * @param _new_parent The new parent transform.
             */
            void SetParent(Transform* _new_parent);

            /**
             * @brief Move the transform forward in its local space.
             * @param distance The distance to move.
             */
            void MoveForward(float distance);

            /**
             * @brief Move the transform backward in its local space.
             * @param distance The distance to move.
             */
            void MoveBackward(float distance);

            /**
             * @brief Move the transform left in its local space.
             * @param distance The distance to move.
             */
            void MoveLeft(float distance);

            /**
             * @brief Move the transform right in its local space.
             * @param distance The distance to move.
             */
            void MoveRight(float distance);

            /**
             * @brief Move the transform up in its local space.
             * @param distance The distance to move.
             */
            void MoveUp(float distance);

            /**
             * @brief Move the transform down in its local space.
             * @param distance The distance to move.
             */
            void MoveDown(float distance);

        private:
            DirectX::XMFLOAT4X4 localMatrix = DirectX::XMFLOAT4X4(1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f); /**< The local transformation matrix. */
            Transform* parent = nullptr; /**< The parent transform. */
        };
    }
}

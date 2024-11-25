/**
 * @file GraphicComponent.h
 * @brief Defines the graphic component class for objects in the engine.
 */

#pragma once

#include "Core/Interface/IComponent.h"
#include "Core/CoreType/Vertex.h"
#include "Core/CoreType/Mesh.h"
#include "Core/CoreType/Transform.h"
#include "Core/CoreType/Id.h"

namespace Objects {

    namespace Component {

        /**
         * @class GraphicComponent
         * @brief Represents a graphic component for objects in the engine.
         */
        class GraphicComponent : public Core::Interface::IComponent
        {
        public:
            /**
             * @brief Constructs a graphic component with custom vertices and optional indices.
             * @param _vertices_array Array of vertices.
             * @param _vertices_count Number of vertices.
             * @param _indices_array Array of indices (optional).
             * @param _indices_count Number of indices (optional).
             */
            explicit GraphicComponent(Core::CoreType::Vertex _vertices_array[], unsigned int _vertices_count, unsigned int _indices_array[] = {}, unsigned int _indices_count = 0);

            /**
             * @brief Constructs a graphic component with custom vertices, color, and optional indices.
             * @param _vertices_array Array of vertices.
             * @param _vertices_count Number of vertices.
             * @param _color Color of the component.
             * @param _indices_array Array of indices (optional).
             * @param _indices_count Number of indices (optional).
             */
            explicit GraphicComponent(Core::CoreType::Vertex _vertices_array[], unsigned int _vertices_count, DirectX::XMVECTORF32 _color, unsigned int _indices_array[] = {}, unsigned int _indices_count = 0);

            /**
             * @brief Constructs a graphic component with custom vertices, color components, and optional indices.
             * @param _vertices_array Array of vertices.
             * @param _vertices_count Number of vertices.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             * @param _indices_array Array of indices (optional).
             * @param _indices_count Number of indices (optional).
             */
            explicit GraphicComponent(Core::CoreType::Vertex _vertices_array[], unsigned int _vertices_count, float _r, float _g, float _b, float _a, unsigned int _indices_array[] = {}, unsigned int _indices_count = 0);

            /**
             * @brief Constructs a graphic component with a pre-defined mesh.
             * @param _mesh Pre-defined mesh.
             */
            explicit GraphicComponent(Core::CoreType::Mesh _mesh);

            /**
             * @brief Constructs a graphic component with a pre-defined mesh and color.
             * @param _mesh Pre-defined mesh.
             * @param _color Color of the component.
             */
            explicit GraphicComponent(Core::CoreType::Mesh _mesh, DirectX::XMVECTORF32 _color);

            /**
             * @brief Constructs a graphic component with a pre-defined mesh and custom color components.
             * @param _mesh Pre-defined mesh.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             */
            explicit GraphicComponent(Core::CoreType::Mesh _mesh, float _r, float _g, float _b, float _a);

            /**
             * @brief Constructs a graphic component with a pre-defined primitive mesh type.
             * @param _mesh_type Pre-defined primitive mesh type.
             */
            explicit GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type);

            /**
             * @brief Constructs a graphic component with a pre-defined primitive mesh type and color.
             * @param _mesh_type Pre-defined primitive mesh type.
             * @param _color Color of the component.
             */
            explicit GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, DirectX::XMVECTORF32 _color);

            /**
             * @brief Constructs a graphic component with a pre-defined primitive mesh type and custom color components.
             * @param _mesh_type Pre-defined primitive mesh type.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             */
            explicit GraphicComponent(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, float _r, float _g, float _b, float _a);

            /**
             * @brief Default destructor.
             */
            ~GraphicComponent();

            /**
             * @brief Initializes the graphic component.
             * @return True if initialization is successful, false otherwise.
             */
            bool Init() override;

            /**
             * @brief Starts the graphic component.
             * @return True if starting is successful, false otherwise.
             */
            bool Start() override;

            /**
             * @brief Updates the graphic component.
             * @return True if updating is successful, false otherwise.
             */
            bool Update() override;

            /**
             * @brief Destructs the graphic component.
             * @return True if destruction is successful, false otherwise.
             */
            bool Destruct() override;

            /**
             * @brief Gets the transform of the graphic component.
             * @return The transform of the component.
             */
            Core::CoreType::Transform GetTransform() const;

            /**
             * @brief Gets the reference to the transform of the graphic component.
             * @return Pointer to the transform of the component.
             */
            Core::CoreType::Transform* GetTransformReference() const;

            /**
             * @brief Gets the mesh of the graphic component.
             * @return Pointer to the mesh of the component.
             */
            Core::CoreType::Mesh* GetMesh() const;

            /**
             * @brief Gets the ID of the graphic component.
             * @return The ID of the component.
             */
            Core::CoreType::Id GetId() const;

            /**
             * @brief Gets the color of the graphic component.
             * @return Pointer to the array of color components.
             */
            const float* GetColor() const;

            /**
             * @brief Sets the color of the graphic component.
             * @param _color New color for the component.
             */
            void SetColor(DirectX::XMVECTORF32 _color);

            /**
             * @brief Sets the color of the graphic component with custom color components.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             */
            void SetColor(float _r, float _g, float _b, float _a);

            /**
             * @brief Sets the mesh of the graphic component with custom vertices, color, and optional indices.
             * @param _vertices_array Array of vertices.
             * @param _vertices_count Number of vertices.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             * @param _indices_array Array of indices (optional).
             * @param _indices_count Number of indices (optional).
             */
            void SetMesh(Core::CoreType::Vertex _vertices_array[], unsigned int _vertices_count, float _r, float _g, float _b, float _a, unsigned int _indices_array[] = {}, unsigned int _indices_count = 0);

            /**
             * @brief Sets the mesh of the graphic component with a pre-defined mesh and custom color components.
             * @param _mesh Pre-defined mesh.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             */
            void SetMesh(Core::CoreType::Mesh _mesh, float _r, float _g, float _b, float _a);

            /**
             * @brief Sets the mesh of the graphic component with a pre-defined primitive mesh type and custom color components.
             * @param _mesh_type Pre-defined primitive mesh type.
             * @param _r Red component of the color.
             * @param _g Green component of the color.
             * @param _b Blue component of the color.
             * @param _a Alpha component of the color.
             */
            void SetMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, float _r, float _g, float _b, float _a);

        private:

            Core::CoreType::Mesh* mesh = nullptr; /**< Pointer to the mesh of the component. */
            Core::CoreType::Id id = Core::CoreType::Id(Core::CoreType::EObjectSubtype::GraphicComponent); /**< ID of the component. */
            float color[4] = { 0.f, 0.f, 0.f, 0.f }; /**< Array of color components. */
        };
    }
}

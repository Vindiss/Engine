#pragma once
#include "Vertex.h"
#include "DirectXColors.h"
#include "PrimitivesMesh.h"
#include <vector>
#include <string>

namespace Core
{
    namespace CoreType
    {
        /**
         * @brief Represents a mesh consisting of vertices and optional indices.
         */
        class Mesh
        {
        public:
            /**
             * @brief Constructs a mesh from an array of vertices and optional indices.
             * @param _vertices_array Array of vertices.
             * @param _vertices_count Number of vertices in the array.
             * @param _indexes_array Array of indices.
             * @param _indexes_count Number of indices in the array.
             */
            explicit Mesh(Vertex _vertices_array[], unsigned int _vertices_count, unsigned int _indexes_array[] = {}, unsigned int _indexes_count = 0);

            /**
             * @brief Constructs a mesh representing a primitive shape.
             * @param _mesh_type Type of primitive mesh.
             */
            explicit Mesh(PrimitivesMesh::PrimitivesMeshType _mesh_type);

            /**
             * @brief Constructs a mesh by loading it from a file.
             * @param _mesh_path Path to the mesh file.
             */
            explicit Mesh(std::string _mesh_path);

            /**
             * @brief Default destructor.
             */
            ~Mesh() = default;

            /**
             * @brief Sets the color of the mesh.
             * @param _color Color to set.
             */
            void SetColor(DirectX::XMVECTORF32 _color);

            /**
             * @brief Sets the color of the mesh.
             * @param _r Red component.
             * @param _g Green component.
             * @param _b Blue component.
             * @param _a Alpha component.
             */
            void SetColor(float _r, float _g, float _b, float _a);

            /**
             * @brief Checks if the mesh is indexed.
             * @return True if the mesh is indexed, false otherwise.
             */
            bool IsIndexed() const;

            /**
             * @brief Retrieves the array of vertices.
             * @return Pointer to the array of vertices.
             */
            Vertex* GetVertices() const;

            /**
             * @brief Retrieves the array of indices.
             * @return Pointer to the array of indices.
             */
            unsigned int* GetIndexes() const;

            /**
             * @brief Retrieves the number of vertices in the mesh.
             * @return Number of vertices.
             */
            int GetVertexCount() const;

            /**
             * @brief Retrieves the number of indices in the mesh.
             * @return Number of indices.
             */
            int GetIndexCount() const;

            /**
             * @brief Retrieves the name of the mesh.
             * @return Name of the mesh.
             */
            std::string GetName() const;

            /**
             * @brief Retrieves the path of the mesh file.
             * @return Path of the mesh file.
             */
            std::string GetMeshPath() const;

        private:
            /**
             * @brief Name of the mesh.
             */
            std::string name = "Unamed Mesh";

            /**
             * @brief Path to the mesh file.
             */
            std::string mesh_path = "";

            /**
             * @brief Indicates whether the mesh is indexed.
             */
            bool indexed = false;

            /**
             * @brief Copies the array of vertices.
             * @param _vertices_array Array of vertices to copy.
             * @param _vertices_count Number of vertices in the array.
             */
            void CopyVerticesArray(const Vertex _vertices_array[], unsigned int _vertices_count);

            /**
             * @brief Copies the array of indices.
             * @param _indexes_array Array of indices to copy.
             * @param _indexes_count Number of indices in the array.
             */
            void CopyIndexesArray(unsigned int _indexes_array[], unsigned int _indexes_count);

            /**
             * @brief Vector containing the vertices of the mesh.
             */
            std::vector<Vertex> vertices;

            /**
             * @brief Vector containing the indices of the mesh.
             */
            std::vector<unsigned int> indexes;
        };
    }
}
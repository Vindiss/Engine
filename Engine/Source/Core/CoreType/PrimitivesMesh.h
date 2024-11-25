#pragma once

#include "Vertex.h"

namespace Core {

    namespace CoreType {

        namespace PrimitivesMesh {

            /**
             * @brief Enum defining types of primitive meshes.
             */
            enum class PrimitivesMeshType
            {
                Cube,      /**< Cube primitive mesh. */
                Plane,     /**< Plane primitive mesh. */
                Pyramide,  /**< Pyramid primitive mesh. */
                Empty,     /**< Empty primitive mesh. */
            };

            /**
             * @brief Vertices for a cube primitive mesh.
             */
            static const Vertex CubeVertices[] =
            {
				{ -0.5f,  0.5f, -0.5f, 0.0f, 0.0f },
				{  0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
				{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
				{  0.5f,  0.5f, -0.5f, 1.0f, 0.0f },

				// right side face
				{  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
				{  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
				{  0.5f, -0.5f,  0.5f, 1.0f, 1.0f },
				{  0.5f,  0.5f, -0.5f, 0.0f, 0.0f },

				// left side face
				{ -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },
				{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
				{ -0.5f, -0.5f,  0.5f, 0.0f, 1.0f },
				{ -0.5f,  0.5f, -0.5f, 1.0f, 0.0f },

				// back face
				{  0.5f,  0.5f,  0.5f, 0.0f, 0.0f },
				{ -0.5f, -0.5f,  0.5f, 1.0f, 1.0f },
				{  0.5f, -0.5f,  0.5f, 0.0f, 1.0f },
				{ -0.5f,  0.5f,  0.5f, 1.0f, 0.0f },

				// top face
				{ -0.5f,  0.5f, -0.5f, 0.0f, 1.0f },
				{  0.5f,  0.5f,  0.5f, 1.0f, 0.0f },
				{  0.5f,  0.5f, -0.5f, 1.0f, 1.0f },
				{ -0.5f,  0.5f,  0.5f, 0.0f, 0.0f },

				// bottom face
				{  0.5f, -0.5f,  0.5f, 0.0f, 0.0f },
				{ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
				{  0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
				{ -0.5f, -0.5f,  0.5f, 1.0f, 0.0f },
            };

            /**
             * @brief Indices for a cube primitive mesh.
             */
            static unsigned int CubeIndexes[] =
            {
                // front face
				0, 1, 2, 
				0, 3, 1, 

				//// left face
				4, 5, 6, 
				4, 7, 5, 

				//// right face
				8, 9, 10, 
				8, 11, 9, 

				//// back face
				12, 13, 14,
				12, 15, 13, 

				//// top face
				16, 17, 18, 
				16, 19, 17, 

				//// bottom face
				20, 21, 22,
				20, 23, 21, 
            };

            /**
             * @brief Vertices for a plane primitive mesh.
             */
            static const Vertex PlaneVertices[] =
            {
                { -0.5f,  0.f, -0.5f, 0.0f, 0.0f },
                {  0.5f,  0.f,  0.5f, 1.0f, 1.0f },
                {  0.5f,  0.f, -0.5f, 0.0f, 1.0f },
                { -0.5f,  0.f,  0.5f, 1.0f, 0.0f },
            };

            /**
             * @brief Indices for a plane primitive mesh.
             */
            static unsigned int PlaneIndexes[] =
            {
                0, 1, 2,
                0, 3, 1,
                2, 1, 0,
                1, 3, 0,
            };

            /**
             * @brief Vertices for a pyramid primitive mesh.
             */
            static const Vertex PyramideVertices[] =
            {
                { 0.0f, 1.0f, 0.0f, 0.5f, 0.5f },
                { 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, -1.0f, 1.0f, 0.0f },
                { -1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
                { -1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
            };

            /**
             * @brief Indices for a pyramid primitive mesh.
             */
            static unsigned int PyramideIndexes[] =
            {
                0, 1, 2,  // Faces latérales
                0, 2, 3,
                0, 3, 4,
                0, 4, 1,
                1, 3, 4,  // Base (premier triangle)
                1, 2, 3
            };

            /**
             * @brief Vertices for an empty primitive mesh.
             */
            static const Vertex EmptyVertices[] =
            {
                { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f },
            };

            /**
             * @brief Indices for an empty primitive mesh.
             */
            static unsigned int EmptyIndexes[] =
            {
                0
            };
        }
    }
}
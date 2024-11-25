#pragma once
#include <DirectXMath.h>
#include <DirectXColors.h>

namespace Core
{

	namespace CoreType
	{

		/**
		 * @brief Represents a vertex with position, texture coordinates, and color.
		 */
		class Vertex
		{
		public:
			/**
			 * @brief Default constructor.
			 */
			Vertex() = default;

			/**
			 * @brief Constructor with position and texture coordinates.
			 * @param _x The x-coordinate of the vertex position.
			 * @param _y The y-coordinate of the vertex position.
			 * @param _z The z-coordinate of the vertex position.
			 * @param _u The u-coordinate of the texture coordinate.
			 * @param _v The v-coordinate of the texture coordinate.
			 */
			Vertex(float _x, float _y, float _z, float _u, float _v);

			/**
			 * @brief Constructor with position, texture coordinates, and color.
			 * @param _x The x-coordinate of the vertex position.
			 * @param _y The y-coordinate of the vertex position.
			 * @param _z The z-coordinate of the vertex position.
			 * @param _u The u-coordinate of the texture coordinate.
			 * @param _v The v-coordinate of the texture coordinate.
			 * @param _r The red component of the color.
			 * @param _g The green component of the color.
			 * @param _b The blue component of the color.
			 * @param _a The alpha component of the color.
			 */
			Vertex(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a);

			/**
			 * @brief Constructor with position, texture coordinates, and color.
			 * @param _x The x-coordinate of the vertex position.
			 * @param _y The y-coordinate of the vertex position.
			 * @param _z The z-coordinate of the vertex position.
			 * @param _u The u-coordinate of the texture coordinate.
			 * @param _v The v-coordinate of the texture coordinate.
			 * @param _color The color of the vertex.
			 */
			Vertex(float _x, float _y, float _z, float _u, float _v, DirectX::XMVECTORF32 _color);

			/**
			 * @brief Set the location of the vertex.
			 * @param _x The x-coordinate of the vertex position.
			 * @param _y The y-coordinate of the vertex position.
			 * @param _z The z-coordinate of the vertex position.
			 */
			void SetLocation(float _x, float _y, float _z);

			/**
			 * @brief Set the texture coordinates of the vertex.
			 * @param _u The u-coordinate of the texture coordinate.
			 * @param _v The v-coordinate of the texture coordinate.
			 */
			void SetTextureCoord(float _u, float _v);

			/**
			 * @brief Set the color of the vertex.
			 * @param _color The color of the vertex as a DirectX::XMVECTORF32.
			 */
			void SetColor(DirectX::XMVECTORF32 _color);

			/**
			 * @brief Set the color of the vertex.
			 * @param _r The red component of the color.
			 * @param _g The green component of the color.
			 * @param _b The blue component of the color.
			 * @param _a The alpha component of the color.
			 */
			void SetColor(float _r, float _g, float _b, float _a);

			/**
			 * @brief Get the raw data of the vertex.
			 * @return A pointer to the raw data array.
			 */
			const float* GetTable() const { return data; }

			/**
			 * @brief Get the size of the raw data array.
			 * @return The size of the raw data array.
			 */
			static int GetSize() { return sizeof data; }
		private:
			float data[9] = { 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f }; /**< Raw data array containing position, texture coordinates, and color. */
		};
	}
}

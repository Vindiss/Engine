#pragma once

#include <d3d12.h>
#include <string>
#include "Core/CoreType/Vertex.h"
#include "Core/CoreType/Transform.h"
#include "Dx12ConstantBuffer.h"

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

namespace Module {

	namespace Render {

		namespace DirectX12 {

			namespace DxObjects {
				
				/**
				* @brief Class representing a graphic object in DirectX 12.
				*/
				class Dx12GraphicObject
				{
				public:

					/**
					 * @brief Constructor.
					 * @param _id The ID of the graphic object.
					 * @param _name The name of the graphic object.
					 * @param _transform The transform of the graphic object.
					 * @param _command_list The command list associated with the graphic object.
					 * @param _vertices_array The array of vertices.
					 * @param _vertices_count The number of vertices.
					 * @param _vertex_buffer_view The vertex buffer view.
					 * @param _textureDescriptorHeap The descriptor heap for textures.
					 * @param _indexs_array The array of indices (optional).
					 * @param _indexs_count The number of indices (optional).
					 * @param _index_buffer_view The index buffer view (optional).
					 * @param _constant_buffer The constant buffer (optional).
					 */
					Dx12GraphicObject(unsigned int _id, std::string _name, Core::CoreType::Transform* _transform, ID3D12GraphicsCommandList* _command_list, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, D3D12_VERTEX_BUFFER_VIEW* _vertex_buffer_view, ID3D12DescriptorHeap* _textureDescriptorHeap, unsigned int* _indexs_array = nullptr, unsigned int _indexs_count = 0, D3D12_INDEX_BUFFER_VIEW* _index_buffer_view = nullptr, Dx12ConstantBuffer* _constant_buffer = nullptr);

					/**
					 * @brief Destructor.
					 */
					~Dx12GraphicObject();

					/**
					 * @brief Prepares the command list for rendering the graphic object.
					 */
					void PrepareCommandList() const;

					/**
					 * @brief Checks if the graphic object is indexed.
					 * @return True if the object is indexed, false otherwise.
					 */
					bool IsIndexed() const;
					/**
					* @brief Gets the ID of the graphic object.
					* @return The ID of the graphic object.
					*/
					unsigned int GetId() const;
					/**
					* @brief Gets the command list associated with the graphic object.
					* @return The command list.
					*/
					ID3D12GraphicsCommandList* GetCommandList() const;
					/**
					* @brief Gets the transform of the graphic object.
					* @return The transform.
					*/
					Core::CoreType::Transform* GetTransform() const;
					/**
					 * @brief Gets the vertex buffer view.
					 * @return The vertex buffer view.
					 */
					D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const;
					/**
					* @brief Gets the number of vertices.
					* @return The number of vertices.
					*/
					unsigned int GetVertexCount() const;
					/**
					* @brief Gets the index buffer view.
					* @return The index buffer view.
					*/
					D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const;
					/**
					* @brief Gets the number of indices.
					* @return The number of indices.
					*/
					unsigned int GetIndexCount() const;
					/**
					 * @brief Gets the constant buffer.
					 * @return The constant buffer.
					 */
					Dx12ConstantBuffer* GetConstantBuffer() const;
					/**
					 * @brief Gets the descriptor heap for textures.
					 * @return The descriptor heap.
					 */
					ID3D12DescriptorHeap* GetTextureDesciptorHeap() const;
					/**
					* @brief Sets new vertices for the graphic object.
					* @param _vertices The array of new vertices.
					* @param _vertices_count The number of new vertices.
					*/
					void SetVertices(Core::CoreType::Vertex* _vertices, unsigned int _vertices_count);
					/**
					* @brief Sets the vertex buffer view.
					* @param _vertex_buffer_view The new vertex buffer view.
					*/
					void SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW* _vertex_buffer_view);
					/**
					 * @brief Sets new indices for the graphic object.
					 * @param _indexs_array The array of new indices.
					 * @param _indexs_count The number of new indices.
					 */
					void SetIndexs(unsigned int* _indexs_array, unsigned int _indexs_count);
					/**
					* @brief Sets the index buffer view.
					* @param _index_buffer_view The new index buffer view.
					*/
					void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW* _index_buffer_view);
					/**
					 * @brief Sets the constant buffer.
					 * @param _constant_buffer The new constant buffer.
					 */
					void SetConstantBuffer(Dx12ConstantBuffer* _constant_buffer);
					/**
					* @brief Sets the descriptor heap for textures.
					* @param _textureDescriptorHeap The new descriptor heap.
					*/
					void SetTextureDescriptorHeap(ID3D12DescriptorHeap* _textureDescriptorHeap);

				private:

					/**
					 * @brief Unique identifier for the graphic object.
					 */
					unsigned int id = 0;

					/**
					 * @brief Indicates whether the object has been indexed.
					 */
					bool indexed = false;

					/**
					 * @brief Pointer to the DirectX 12 graphics command list.
					 */
					ID3D12GraphicsCommandList* commandList = nullptr;

					/**
					 * @brief View of the DirectX 12 vertex buffer.
					 */
					D3D12_VERTEX_BUFFER_VIEW* vertexBufferView = nullptr;

					/**
					 * @brief View of the DirectX 12 index buffer.
					 */
					D3D12_INDEX_BUFFER_VIEW* indexBufferView = nullptr;

					/**
					 * @brief DirectX 12 constant buffer.
					 */
					Dx12ConstantBuffer* constantBuffer = nullptr;

					/**
					 * @brief DirectX 12 descriptor heap for textures.
					 */
					ID3D12DescriptorHeap* textureDescriptorHeap = nullptr;

					/**
					 * @brief Name of the graphic object.
					 */
					std::string name = "Unnamed DX12 Graphic Object";

					/**
					 * @brief Transformation of the graphic object.
					 */
					Core::CoreType::Transform* transform;

					/**
					 * @brief Array of vertices of the graphic object.
					 */
					Core::CoreType::Vertex* vertices = nullptr;

					/**
					 * @brief Number of vertices in the array.
					 */
					unsigned int verticesCount = 0;

					/**
					 * @brief Array of indices of the graphic object.
					 */
					unsigned int* indexsArray = nullptr;

					/**
					 * @brief Number of indices in the array.
					 */
					unsigned int indexsCount = 0;
				};
			}
		}
	}
}
#pragma once

#include <d3d12.h>

namespace Module {

	namespace Render {

		namespace DirectX12 {

			namespace DxObjects {

				/**
				 * @brief Class representing a DirectX 12 constant buffer.
				 */
				class Dx12ConstantBuffer
				{
				public:
					/**
					 * @brief Constructs a Dx12ConstantBuffer object.
					 * @param _descriptor_heap The descriptor heap associated with the constant buffer.
					 * @param _buffer_upload_heap The upload heap for the constant buffer.
					 */
					explicit Dx12ConstantBuffer(ID3D12DescriptorHeap* _descriptor_heap, ID3D12Resource* _buffer_upload_heap);

					/**
					 * @brief Destructor.
					 */
					~Dx12ConstantBuffer();

					/**
					 * @brief Maps the constant buffer for CPU access.
					 * @return Pointer to the mapped memory.
					 */
					UINT8* Map();

					/**
					 * @brief Unmaps the constant buffer.
					 */
					void Unmap();

					/**
					 * @brief Retrieves the underlying resource of the constant buffer.
					 * @return Pointer to the constant buffer resource.
					 */
					ID3D12Resource* GetResource() const;

					/**
					 * @brief Retrieves the descriptor heap associated with the constant buffer.
					 * @return Pointer to the descriptor heap.
					 */
					ID3D12DescriptorHeap* GetDescriptorHeap() const;

					/**
					 * @brief Retrieves the GPU address of the constant buffer.
					 * @return Pointer to the GPU address.
					 */
					UINT8* GetHeapGpuAdress() const;

				private:
					// Private member variables...

					bool isMap = false; ///< Flag indicating whether the constant buffer is mapped.

					ID3D12Resource* bufferUploadHeap = nullptr; ///< Upload heap for the constant buffer.

					UINT8* heapGpuAdress = nullptr; ///< GPU address of the constant buffer.

					ID3D12DescriptorHeap* descriptorHeap = nullptr; ///< Descriptor heap associated with the constant buffer.
				};
			}
		}
	}
}
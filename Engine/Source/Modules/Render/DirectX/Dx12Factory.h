#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wincodec.h>
#include "Config/d3dx12.h"
#include <string>
#include <vector>
#include "Source/Core/CoreType/Vertex.h"
#include "Modules/Render/DirectX/DxObjects/Dx12ConstantBuffer.h"


#define TRYFUNC(function) hr = 0; hr = function; if(FAILED(hr)) return false;

#define CHECK_BLOB(blob) if(FAILED(hr)) { OutputDebugStringA((char*)(blob)->GetBufferPointer()); return false; }


namespace Core {

	namespace CoreType {

	}
}

namespace Module {
	
	namespace Render {

		namespace DirectX12 {

			/**
			 * @brief Structure representing a pack for DirectX 12 command execution.
			 */
			struct Dx12CommandExecutionPack
			{
				
				/**
				 * @brief Pointer to the DirectX 12 pipeline state object.
				 */
				ID3D12PipelineState* pipelineState;

				/**
				 * @brief Pointer to the DirectX 12 command allocator.
				 */
				ID3D12CommandAllocator* commandAllocator;

				/**
				 * @brief Pointer to the DirectX 12 command queue.
				 */
				ID3D12CommandQueue* commandQueue;

				/**
				 * @brief Pointer to the DirectX 12 fence object.
				 */
				ID3D12Fence* fence;

				/**
				 * @brief Pointer to the value of the fence.
				 */
				UINT64* fenceValue;
			};

			/**
			* @brief Factory class for DirectX 12 objects.
			*/
			class Dx12Factory
			{
			public:

				/**
			   * @brief Default constructor.
			   */

				Dx12Factory() = default;

				/**
				* @brief Default destructor.
				*/

				~Dx12Factory() = default ;

				/**
				* @brief Initializes the DirectX 12 factory.
				* @return True if initialization is successful, false otherwise.
				*/

				/**
				 * @brief Initializes the Direct3D 12 factory.
				 * @return True if the initialization succeeds, false otherwise.
				 */
				bool InitFactory();

				/**
				 * @brief Creates a Direct3D 12 device.
				 * @param[out] _device Pointer to the created device.
				 * @return True if the device creation succeeds, false otherwise.
				 */
				bool MakeDevice(ID3D12Device** _device);

				/**
				 * @brief Creates a Direct3D 12 command queue.
				 * @param[out] _command_queue Pointer to the created command queue.
				 * @return True if the command queue creation succeeds, false otherwise.
				 */
				bool MakeCommandQueue(ID3D12CommandQueue** _command_queue);

				/**
				 * @brief Creates a Direct3D 12 swap chain.
				 * @param[in] _command_queue The command queue to associate with the swap chain.
				 * @param[in] _width Width of the swap chain.
				 * @param[in] _height Height of the swap chain.
				 * @param[in] _frame_buffer_count Number of frames in the swap chain.
				 * @param[in] _window Handle to the window.
				 * @param[in] _fullscreen Whether to create the swap chain in fullscreen mode.
				 * @param[out] _swap_chain Pointer to the created swap chain.
				 * @return True if the swap chain creation succeeds, false otherwise.
				 */
				bool MakeSwapChain(ID3D12CommandQueue* _command_queue, const UINT _width, const UINT _height, const UINT _frame_buffer_count, HWND _window, const BOOL _fullscreen, IDXGISwapChain3** _swap_chain);

				/**
				 * @brief Creates render target views for the swap chain.
				 * @param[in] _frame_buffer_count Number of frames in the swap chain.
				 * @param[out] _rtv_descriptor_heap Pointer to the descriptor heap for render target views.
				 * @param[out] _rtv_descriptor_size Size of a render target view descriptor.
				 * @param[in] _swap_chain The swap chain from which to create render target views.
				 * @param[out] _render_targets Array of render target resources.
				 * @return True if the render target views creation succeeds, false otherwise.
				 */
				bool MakeRenderTargetView(const UINT _frame_buffer_count, ID3D12DescriptorHeap** _rtv_descriptor_heap, UINT* _rtv_descriptor_size, IDXGISwapChain3* _swap_chain, ID3D12Resource** _render_targets);

				/**
				 * @brief Creates command allocators.
				 * @param[in] _frame_buffer_count Number of frames in the swap chain.
				 * @param[out] _command_allocator Pointer to the array of command allocators.
				 * @return True if the command allocators creation succeeds, false otherwise.
				 */
				bool MakeCommandAllocator(const UINT _frame_buffer_count, ID3D12CommandAllocator** _command_allocator);

				/**
				 * @brief Creates a command list.
				 * @param[in] _command_allocator The command allocator from which to create the command list.
				 * @param[out] _command_list Pointer to the created command list.
				 * @return True if the command list creation succeeds, false otherwise.
				 */
				bool MakeCommandList(ID3D12CommandAllocator* _command_allocator, ID3D12GraphicsCommandList** _command_list);

				/**
				 * @brief Creates a fence object.
				 * @param[in] _frame_buffer_count Number of frames in the swap chain.
				 * @param[out] _fence Pointer to the fence object.
				 * @param[out] _fence_value Pointer to the initial fence value.
				 * @param[out] _fence_event Pointer to the fence completion event.
				 * @return True if the fence creation succeeds, false otherwise.
				 */
				bool MakeFence(const UINT _frame_buffer_count, ID3D12Fence** _fence, UINT64* _fence_value, HANDLE* _fence_event);

				/**
				 * @brief Creates a root signature.
				 * @param[out] _root_signature Pointer to the created root signature.
				 * @return True if the root signature creation succeeds, false otherwise.
				 */
				bool MakeRootSignature(ID3D12RootSignature** _root_signature);

				/**
				 * @brief Loads and creates a vertex shader from the specified file.
				 * @param[in] _path The path to the vertex shader file.
				 * @param[out] _shader_bytecode Pointer to the bytecode of the created vertex shader.
				 * @return True if the vertex shader creation succeeds, false otherwise.
				 */
				bool MakeVertexShader(const LPCWSTR _path, D3D12_SHADER_BYTECODE* _shader_bytecode);

				/**
				 * @brief Loads and creates a pixel shader from the specified file.
				 * @param[in] _path The path to the pixel shader file.
				 * @param[out] _shader_bytecode Pointer to the bytecode of the created pixel shader.
				 * @return True if the pixel shader creation succeeds, false otherwise.
				 */
				bool MakePixelShader(const LPCWSTR _path, D3D12_SHADER_BYTECODE* _shader_bytecode);

				/**
				 * @brief Creates a depth stencil buffer.
				 * @param[out] _descriptor Pointer to the descriptor heap for the depth stencil buffer.
				 * @param[out] _buffer Pointer to the created depth stencil buffer resource.
				 * @param[in] _width Width of the depth stencil buffer.
				 * @param[in] _height Height of the depth stencil buffer.
				 * @return True if the depth stencil buffer creation succeeds, false otherwise.
				 */
				bool MakeDepthStencilBuffer(ID3D12DescriptorHeap** _descriptor, ID3D12Resource** _buffer, const UINT _width, const UINT _height);

				/**
				 * @brief Creates a pipeline state object (PSO).
				 * @param[in] _root_signature The root signature for the PSO.
				 * @param[in] _vertex_shader_bytecode Bytecode of the vertex shader.
				 * @param[in] _pixel_shader_bytecode Bytecode of the pixel shader.
				 * @param[out] _pipeline_state_object Pointer to the created PSO.
				 * @return True if the PSO creation succeeds, false otherwise.
				 */
				bool MakePipelineStateObject(ID3D12RootSignature* _root_signature, D3D12_SHADER_BYTECODE* _vertex_shader_bytecode, D3D12_SHADER_BYTECODE* _pixel_shader_bytecode, ID3D12PipelineState** _pipeline_state_object);

				/**
				 * @brief Creates a vertex buffer.
				 * @param[in] _pack The execution pack for creating the buffer.
				 * @param[in] _name Name of the buffer.
				 * @param[in] _vertices_array Array of vertices.
				 * @param[in] _vertices_count Number of vertices in the array.
				 * @param[in] _command_list Command list for the buffer creation.
				 * @param[out] _vertex_buffer_view Pointer to the vertex buffer view.
				 * @return True if the vertex buffer creation succeeds, false otherwise.
				 */
				bool CreateVertexBuffer(Dx12CommandExecutionPack _pack, const std::string _name, Core::CoreType::Vertex* _vertices_array, const unsigned int _vertices_count, ID3D12GraphicsCommandList* _command_list, D3D12_VERTEX_BUFFER_VIEW** _vertex_buffer_view);

				/**
				 * @brief Creates an index buffer.
				 * @param[in] _pack The execution pack for creating the buffer.
				 * @param[in] _name Name of the buffer.
				 * @param[in] _indexes_array Array of indices.
				 * @param[in] _indexes_count Number of indices in the array.
				 * @param[in] _command_list Command list for the buffer creation.
				 * @param[out] _index_buffer_view Pointer to the index buffer view.
				 * @return True if the index buffer creation succeeds, false otherwise.
				 */
				bool CreateIndexBuffer(Dx12CommandExecutionPack _pack, const std::string _name, unsigned int* _indexes_array, const unsigned int _indexes_count, ID3D12GraphicsCommandList* _command_list, D3D12_INDEX_BUFFER_VIEW** _index_buffer_view);

				/**
				 * @brief Creates a texture buffer.
				 * @param[in] _pack The execution pack for creating the buffer.
				 * @param[in] _name Name of the buffer.
				 * @param[in] _command_list Command list for the buffer creation.
				 * @param[in] _path Path to the texture file.
				 * @param[out] _texture_descriptor_heap Pointer to the descriptor heap for the texture.
				 * @return True if the texture buffer creation succeeds, false otherwise.
				 */
				bool CreateTextureBuffer(Dx12CommandExecutionPack _pack, const std::string _name, ID3D12GraphicsCommandList* _command_list, LPCWSTR _path, ID3D12DescriptorHeap** _texture_descriptor_heap);

				/**
				 * @brief Creates a constant buffer.
				 * @tparam T Type of data for the constant buffer.
				 * @param[in] _data Pointer to the data for the constant buffer.
				 * @param[out] _constant_buffer Pointer to the created constant buffer.
				 * @return True if the constant buffer creation succeeds, false otherwise.
				 */
				template <typename T>
				bool CreateConstantBuffer(T* _data, DxObjects::Dx12ConstantBuffer** _constant_buffer);

				/**
				 * @brief Loads image data from a file.
				 * @param[out] imageData Pointer to the image data.
				 * @param[out] resourceDescription Description of the image resource.
				 * @param[in] filename Path to the image file.
				 * @param[out] bytesPerRow Number of bytes per row.
				 * @return 0 if the image data is loaded successfully, non-zero otherwise.
				 */
				int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow);
			private:

				/**
				 * @brief Private function to retrieve a list of all adapters.
				 * @return A vector containing pointers to all available adapters.
				 */
				std::vector<IDXGIAdapter1*> GetAdaptersList() const;

				/**
				 * @brief Private function to retrieve a list of compatible adapters.
				 * @return A vector containing pointers to compatible adapters.
				 */
				std::vector<IDXGIAdapter1*> GetCompatibleAdaptersList() const;


				static std::vector<IDXGIOutput*> GetOutputList(IDXGIAdapter1* _adapter);

				DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
				WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
				int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);

				/**
				 * @brief DirectX 12 factory object.
				 */
				IDXGIFactory4* dxgiFactory = nullptr;

				/**
				 * @brief Direct3D 12 device object.
				 */
				ID3D12Device* device = nullptr;

				/**
				 * @brief Result code of the last DirectX operation.
				 */
				HRESULT hr = 0;

			};

			/**
			 * @brief Creates a constant buffer.
			 * @tparam T Type of data for the constant buffer.
			 * @param[in] _data Pointer to the data for the constant buffer.
			 * @param[out] _constant_buffer Pointer to the created constant buffer.
			 * @return True if the constant buffer creation succeeds, false otherwise.
			 */
			template <typename T>
			bool Dx12Factory::CreateConstantBuffer(T* _data, DxObjects::Dx12ConstantBuffer** _constant_buffer)
			{
				ID3D12DescriptorHeap* heap_descriptor = nullptr;

				D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
				heap_desc.NumDescriptors = 1;
				heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				TRYFUNC(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&heap_descriptor)));

				ID3D12Resource* constant_buffer_upload_heap = nullptr;

				CD3DX12_HEAP_PROPERTIES upload_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
				CD3DX12_RESOURCE_DESC resource_descriptor = CD3DX12_RESOURCE_DESC::Buffer(1024 * 64);
				TRYFUNC(device->CreateCommittedResource(&upload_heap_properties, D3D12_HEAP_FLAG_NONE, &resource_descriptor, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constant_buffer_upload_heap)));
				constant_buffer_upload_heap->SetName(L"Constant Buffer Upload Resource Heap");

				D3D12_CONSTANT_BUFFER_VIEW_DESC constant_buffer_view_desc;
				constant_buffer_view_desc.BufferLocation = constant_buffer_upload_heap->GetGPUVirtualAddress();
				constant_buffer_view_desc.SizeInBytes = sizeof(_data) + 255 & ~255;
				device->CreateConstantBufferView(&constant_buffer_view_desc, heap_descriptor->GetCPUDescriptorHandleForHeapStart());

				*_constant_buffer = new DxObjects::Dx12ConstantBuffer(heap_descriptor, constant_buffer_upload_heap);

				memcpy((*_constant_buffer)->Map(), &_data, sizeof(_data));

				(*_constant_buffer)->Unmap();

				return true;
			}
		}
	}
}
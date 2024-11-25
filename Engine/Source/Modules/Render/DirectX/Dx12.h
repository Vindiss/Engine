#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <unordered_map>
#include "Core/CoreType/Transform.h"
#include "Core/Interface/IRender.h"
#include "Dx12Factory.h"
#include "DxObjects/Dx12GraphicObject.h"
#include "Engine.h"
#include "Modules/Display/Window.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#define FRAME_BUFFER_COUNT 3

namespace Module {

	namespace Render {

		namespace DirectX12 {

			/**
			 * @brief Class for managing DirectX 12 rendering.
			 */
			class DirectX12 : public Core::Interface::IRender
			{
			public:
				/**
				 * @brief Default constructor.
				 */
				DirectX12() = default;

				/**
				 * @brief Default destructor.
				 */
				~DirectX12() = default;

				/**
				 * @brief Initializes DirectX 12.
				 * @return True if initialization is successful, false otherwise.
				 */
				bool Initialize() override;

				/**
				 * @brief Creates the rendering pipeline.
				 * @return True if creation is successful, false otherwise.
				 */
				bool CreatePipeline() override;

				/**
				 * @brief Creates vertex and index buffers for a graphic object.
				 * @param _id The ID of the graphic object.
				 * @param _name The name of the graphic object.
				 * @param _transform The transform of the graphic object.
				 * @param _vertices_array The array of vertices.
				 * @param _vertices_count The number of vertices.
				 * @param _indexes_array The array of indices.
				 * @param _indexs_count The number of indices.
				 * @param _path The texture path (optional).
				 * @return True if creation is successful, false otherwise.
				 */
				bool CreateBuffers(unsigned int _id, std::string _name, Core::CoreType::Transform* _transform, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array = nullptr, unsigned int _indexs_count = 0, LPCWSTR _path = L"EngineTest\\Assets\\Texture\\default.png") override;

				/**
				 * @brief Makes new index and vertex buffers for a graphic object.
				 * @param _id The ID of the graphic object.
				 * @param _name The name of the graphic object.
				 * @param _vertices_array The array of vertices.
				 * @param _vertices_count The number of vertices.
				 * @param _indexs_array The array of indices.
				 * @param _indexs_count The number of indices.
				 * @return True if creation is successful, false otherwise.
				 */
				bool MakeNewIndexAndVertexBuffers(const unsigned int _id, std::string _name, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array, unsigned int _indexs_count) override;

				/**
				 * @brief Makes new texture buffers for a graphic object.
				 * @param _id The ID of the graphic object.
				 * @param _name The name of the graphic object.
				 * @param _path The texture path.
				 * @return True if creation is successful, false otherwise.
				 */
				bool MakeNewTextureBuffers(const unsigned int _id, std::string _name, LPCWSTR _path) override;

				/**
				 * @brief Renders the scene.
				 * @return True if rendering is successful, false otherwise.
				 */
				bool Render() override;

				/**
				 * @brief Cleans up DirectX 12 resources.
				 * @return True if cleanup is successful, false otherwise.
				 */
				bool Cleanup() override;

				/**
				 * @brief Sets the camera view matrix.
				 * @param _camera_view The view matrix.
				 */
				void SetCameraViewMatrix(DirectX::XMFLOAT4X4 _camera_view) override;

				/**
				 * @brief Sets the camera projection matrix.
				 * @param _camera_projection The projection matrix.
				 */
				void SetCameraProjectionMatrix(DirectX::XMFLOAT4X4 _camera_projection) override;

				/**
				 * @brief Initializes ImGui.
				 * @return True if initialization is successful, false otherwise.
				 */
				bool InitImGui() override;

				/**
				 * @brief Handles window resize event.
				 * @param _window_param The window parameter.
				 * @param _long_param The long parameter.
				 * @return True if handling is successful, false otherwise.
				 */
				bool WindowResize(WPARAM _window_param, LPARAM _long_param) override;

				/**
				 * @brief Cleans up graphics objects.
				 * @return True if cleanup is successful, false otherwise.
				 */
				bool CleanGraphicsObjects() override;

				/**
				 * @brief Deletes a graphic object.
				 * @param _id The ID of the graphic object.
				 * @return True if deletion is successful, false otherwise.
				 */
				bool DeleteGraphicObjects(const unsigned int _id) override;

				/**
				 * @brief Gets the viewport.
				 * @return A pointer to the viewport.
				 */
				D3D12_VIEWPORT* GetViewport();

			private:

				/**
				 * @brief Performs pre-rendering operations.
				 * @return True if pre-rendering is successful, false otherwise.
				 */
				bool PreRender();

				/**
				 * @brief Performs post-rendering operations.
				 * @return True if post-rendering is successful, false otherwise.
				 */
				bool PostRender();

				/**
				 * @brief Updates the rendering pipeline.
				 * @return True if update is successful, false otherwise.
				 */
				bool UpdatePipeline();

				/**
				 * @brief Waits for the previous frame to finish rendering.
				 * @return True if waiting is successful, false otherwise.
				 */
				bool WaitForPreviousFrame();

				/**
				 * @brief Prepares the pre-rendering command list.
				 * @return True if preparation is successful, false otherwise.
				 */
				bool PreparePreRenderCommandList();

				/**
				 * @brief Prepares the post-rendering command list.
				 * @return True if preparation is successful, false otherwise.
				 */
				bool PreparePostRenderCommandList();

				/**
				 * @brief Prepares the command list for a graphic object.
				 * @param _graphic_object The graphic object.
				 * @return True if preparation is successful, false otherwise.
				 */
				bool PrepareObjectCommandList(DxObjects::Dx12GraphicObject* _graphic_object);

				/**
				 * @brief Prepares the command list for ImGui.
				 * @param _command_list The command list.
				 * @return True if preparation is successful, false otherwise.
				 */
				bool PrepareImGuiCommandList(ID3D12GraphicsCommandList* _command_list);

				/**
				 * @brief Pointer to the Engine instance.
				 */
				Engine* engine;

				/**
				 * @brief Result code of the last DirectX operation.
				 */
				HRESULT hr = 0;

				/**
				 * @brief Pointer to the DirectX 12 factory.
				 */
				Dx12Factory* factory = nullptr;

				/**
				 * @brief Descriptor heap for render target views (RTVs).
				 */
				ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;

				/**
				 * @brief Size of a render target view descriptor.
				 */
				UINT rtvDescriptorSize = 0;

				/**
				 * @brief Descriptor heap for shader resource views (SRVs).
				 */
				ID3D12DescriptorHeap* SrvDescriptorHeap = nullptr;

				/**
				 * @brief Pointer to the DirectX 12 device.
				 */
				ID3D12Device* device = nullptr;

				/**
				 * @brief Pointer to the DirectX 12 pipeline state object.
				 */
				ID3D12PipelineState* pipelineStateObject = nullptr;

				/**
				 * @brief Pointer to the DirectX 12 root signature.
				 */
				ID3D12RootSignature* rootSignature = nullptr;

				/**
				 * @brief Array of fence objects for synchronizing CPU-GPU operations.
				 */
				ID3D12Fence* fence[FRAME_BUFFER_COUNT];

				/**
				 * @brief Event handle for signaling when a fence is reached.
				 */
				HANDLE fenceEvent = nullptr;

				/**
				 * @brief Array of fence values for each frame buffer.
				 */
				UINT64 fenceValue[FRAME_BUFFER_COUNT]; 

				/**
				 * @brief Viewport for rendering.
				 */
				D3D12_VIEWPORT viewport;

				/**
				 * @brief Scissor rectangle for rendering.
				 */
				D3D12_RECT scissorRect;

				/**
				 * @brief Bytecode for the vertex shader.
				 */
				D3D12_SHADER_BYTECODE vertexShaderBytecode = {};

				/**
				 * @brief Bytecode for the pixel shader.
				 */
				D3D12_SHADER_BYTECODE pixelShaderBytecode = {};

				/**
				 * @brief Pointer to the swap chain for presenting rendered frames.
				 */
				IDXGISwapChain3* swapChain = nullptr;

				/**
				 * @brief Array of render targets for each frame buffer.
				 */
				ID3D12Resource* renderTargets[FRAME_BUFFER_COUNT];

				/**
				 * @brief Pointer to the command queue for submitting rendering commands.
				 */
				ID3D12CommandQueue* commandQueue = nullptr;

				/**
				 * @brief Command allocators for pre-rendering operations.
				 */
				ID3D12CommandAllocator* preRenderCommandAllocator[FRAME_BUFFER_COUNT];

				/**
				 * @brief Command allocators for object rendering operations.
				 */
				ID3D12CommandAllocator* objectCommandAllocator[FRAME_BUFFER_COUNT];

				/**
				 * @brief Command allocators for ImGui rendering operations.
				 */
				ID3D12CommandAllocator* imGuiCommandAllocator[FRAME_BUFFER_COUNT];

				/**
				 * @brief Command allocators for post-rendering operations.
				 */
				ID3D12CommandAllocator* postRenderCommandAllocator[FRAME_BUFFER_COUNT];

				/**
				 * @brief Pointer to the pre-rendering graphics command list.
				 */
				ID3D12GraphicsCommandList* preRenderCommandList = nullptr;

				/**
				 * @brief Pointer to the post-rendering graphics command list.
				 */
				ID3D12GraphicsCommandList* postRenderCommandList = nullptr;

				/**
				 * @brief Pointer to the ImGui graphics command list.
				 */
				ID3D12GraphicsCommandList* ImGuiCommandList = nullptr;

				/**
				 * @brief Pointer to the depth stencil buffer resource.
				 */
				ID3D12Resource* depthStencilBuffer = nullptr;

				/**
				 * @brief Pointer to the depth stencil descriptor heap.
				 */
				ID3D12DescriptorHeap* depthStencilDescriptorHeap = nullptr;

				/**
				 * @brief Map of graphics objects with their unique IDs.
				 */
				std::unordered_map<int, DxObjects::Dx12GraphicObject*> graphicsObjects;

				/**
				 * @brief Index of the current frame.
				 */
				int frameIndex = 0;

				/**
				 * @brief Projection matrix of the camera.
				 */
				DirectX::XMFLOAT4X4 cameraProjectionMatrix;

				/**
				 * @brief View matrix of the camera.
				 */
				DirectX::XMFLOAT4X4 cameraViewMatrix;


				/**
				 * @brief Mutex for controlling access to the command allocator object.
				 */
				std::mutex objectCommandAllocatorMutex;

				/**
				 * @brief Mutex for controlling access to the frame index.
				 */
				std::mutex frameIndexMutex;

				/**
				 * @brief Mutex for controlling access to the pipeline state object.
				 */
				std::mutex pipelineStateObjectMutex;

				/**
				 * @brief Mutex for controlling access to the render targets.
				 */
				std::mutex renderTargetsMutex;

				/**
				 * @brief Mutex for controlling access to the render target view descriptor heap.
				 */
				std::mutex rtvDescriptorHeapMutex;

				/**
				 * @brief Mutex for controlling access to the render target view descriptor size.
				 */
				std::mutex rtvDescriptorSizeMutex;

				/**
				 * @brief Mutex for controlling access to the depth stencil descriptor heap.
				 */
				std::mutex depthStencilDescriptorHeapMutex;

				/**
				 * @brief Mutex for controlling access to the root signature.
				 */
				std::mutex rootSignatureMutex;
			};
		}
	}
}
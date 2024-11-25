#include "Dx12.h"
#include "DirectXColors.h"
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <wrl/client.h>
#pragma comment(lib, "dxguid.lib")
#include "ImGui/imgui_internal.h"
#include "Modules/Input/Inputs.h"

bool Module::Render::DirectX12::DirectX12::Initialize()
{
	engine = Engine::GetInstance();
	return true;
}

bool Module::Render::DirectX12::DirectX12::CreatePipeline()
{
	Display::Window* window = engine->GetModule<Display::Window>();

	const unsigned int width = window->GetWidth();
	const unsigned int height = window->GetHeight();

	viewport.TopLeftX = static_cast<FLOAT>(width) * 200 / 1920;
	viewport.TopLeftY = static_cast<FLOAT>(height) * 8 / 1009;
	viewport.Width = static_cast<FLOAT>(width) * 1670 / 1920;
	viewport.Height = static_cast<FLOAT>(height) * 830 / 1009;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissorRect.left = viewport.TopLeftX;
	scissorRect.top = viewport.TopLeftY;
	scissorRect.right = viewport.Width;
	scissorRect.bottom = viewport.Height;

	factory = new Dx12Factory();
	if (!factory->InitFactory())
		return false;

	factory->MakeDevice(&device);

	factory->MakeCommandQueue(&commandQueue);
	factory->MakeSwapChain(commandQueue, width, height, FRAME_BUFFER_COUNT, *window->GetHandle(), window->IsFullScreen(), &swapChain);
	factory->MakeRenderTargetView(FRAME_BUFFER_COUNT, &rtvDescriptorHeap, &rtvDescriptorSize, swapChain, renderTargets);
	factory->MakeCommandAllocator(FRAME_BUFFER_COUNT, preRenderCommandAllocator);
	factory->MakeCommandAllocator(FRAME_BUFFER_COUNT, objectCommandAllocator);
	factory->MakeCommandAllocator(FRAME_BUFFER_COUNT, imGuiCommandAllocator);
	factory->MakeCommandAllocator(FRAME_BUFFER_COUNT, postRenderCommandAllocator);
	factory->MakeCommandList(preRenderCommandAllocator[0], &preRenderCommandList);
	factory->MakeCommandList(imGuiCommandAllocator[0], &ImGuiCommandList);
	factory->MakeCommandList(postRenderCommandAllocator[0], &postRenderCommandList);
	factory->MakeFence(FRAME_BUFFER_COUNT, fence, fenceValue, &fenceEvent);
	factory->MakeRootSignature(&rootSignature);
	factory->MakeVertexShader(L"Engine\\Config\\Shaders\\VertexShader.hlsl", &vertexShaderBytecode);
	factory->MakePixelShader(L"Engine\\Config\\Shaders\\PixelShader.hlsl", &pixelShaderBytecode);
	factory->MakeDepthStencilBuffer(&depthStencilDescriptorHeap, &depthStencilBuffer, width, height);
	factory->MakePipelineStateObject(rootSignature, &vertexShaderBytecode, &pixelShaderBytecode, &pipelineStateObject);

	preRenderCommandList->SetName(L"Pre-Render Command List");
	postRenderCommandList->SetName(L"Post-Render Command List");

	return true;
}

bool Module::Render::DirectX12::DirectX12::CreateBuffers(const unsigned int _id, std::string _name, Core::CoreType::Transform* _transform, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array, unsigned int _indexs_count, LPCWSTR _path)
{
	ID3D12GraphicsCommandList* command_list = nullptr;
	factory->MakeCommandList(objectCommandAllocator[frameIndex], &command_list);

	D3D12_VERTEX_BUFFER_VIEW* buffer_view = nullptr;
	const Dx12CommandExecutionPack pack = { pipelineStateObject, objectCommandAllocator[frameIndex], commandQueue, fence[frameIndex], &fenceValue[frameIndex] };
	factory->CreateVertexBuffer(pack, _name, _vertices_array, _vertices_count, command_list, &buffer_view);

	D3D12_INDEX_BUFFER_VIEW* index_view = nullptr;
	if (_indexs_array && _indexs_count)
		factory->CreateIndexBuffer(pack, _name, _indexs_array, _indexs_count, command_list, &index_view);

	DxObjects::Dx12ConstantBuffer* constant_buffer;
	DirectX::XMFLOAT4X4 vector4x4 = DirectX::XMFLOAT4X4(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
	factory->CreateConstantBuffer(&vector4x4, &constant_buffer);

	ID3D12DescriptorHeap* texture_descriptor_heap = nullptr;
	factory->CreateTextureBuffer(pack, _name, command_list, _path, &texture_descriptor_heap);

	graphicsObjects[_id] = new DxObjects::Dx12GraphicObject(_id, _name, _transform, command_list, _vertices_array, _vertices_count, buffer_view, texture_descriptor_heap, _indexs_array, _indexs_count, index_view, constant_buffer);

	return true;
}

bool Module::Render::DirectX12::DirectX12::MakeNewIndexAndVertexBuffers(const unsigned int _id, std::string _name, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, unsigned int* _indexs_array, unsigned int _indexs_count)
{
	DxObjects::Dx12GraphicObject* dx12graphics = graphicsObjects.at(_id);
	ID3D12GraphicsCommandList* command_list = dx12graphics->GetCommandList();

	D3D12_VERTEX_BUFFER_VIEW* buffer_view = nullptr;
	const Dx12CommandExecutionPack pack = { pipelineStateObject, objectCommandAllocator[frameIndex], commandQueue, fence[frameIndex], &fenceValue[frameIndex] };
	factory->CreateVertexBuffer(pack, _name, _vertices_array, _vertices_count, command_list, &buffer_view);
	dx12graphics->SetVertexBufferView(buffer_view);
	dx12graphics->SetVertices(_vertices_array, _vertices_count);

	D3D12_INDEX_BUFFER_VIEW* index_view = nullptr;
	if (_indexs_array && _indexs_count) {
		factory->CreateIndexBuffer(pack, _name, _indexs_array, _indexs_count, command_list, &index_view);
	}
	dx12graphics->SetIndexBufferView(index_view);
	dx12graphics->SetIndexs(_indexs_array, _indexs_count);

	return true;
}

bool Module::Render::DirectX12::DirectX12::MakeNewTextureBuffers(const unsigned int _id, std::string _name, LPCWSTR _path)
{
	DxObjects::Dx12GraphicObject* dx12graphics = graphicsObjects.at(_id);
	ID3D12GraphicsCommandList* command_list = dx12graphics->GetCommandList();

	const Dx12CommandExecutionPack pack = { pipelineStateObject, objectCommandAllocator[frameIndex], commandQueue, fence[frameIndex], &fenceValue[frameIndex] };

	ID3D12DescriptorHeap* texture_descriptor_heap = nullptr;
	factory->CreateTextureBuffer(pack, _name, command_list, _path, &texture_descriptor_heap);
	dx12graphics->SetTextureDescriptorHeap(texture_descriptor_heap);

	return true;
}


bool Module::Render::DirectX12::DirectX12::Render()
{
	UpdatePipeline();
	PreRender();

	ID3D12GraphicsCommandList** pp_command_lists = new ID3D12GraphicsCommandList * [graphicsObjects.size() + 3];

	pp_command_lists[0] = preRenderCommandList;

	unsigned int i = 0;
	for (const std::pair<const int, DxObjects::Dx12GraphicObject*> object_command_list : graphicsObjects)
	{
		pp_command_lists[i + 1] = object_command_list.second->GetCommandList();
		i++;
	}

	pp_command_lists[i + 1] = ImGuiCommandList;

	pp_command_lists[i + 2] = postRenderCommandList;

	commandQueue->ExecuteCommandLists(static_cast<UINT>(graphicsObjects.size()) + 3, (ID3D12CommandList* const*)pp_command_lists);


	WaitForPreviousFrame();

	TRYFUNC(swapChain->Present(0, 0));

	delete[] pp_command_lists;

	PostRender();

	return true;
}

bool Module::Render::DirectX12::DirectX12::Cleanup()
{
	WaitForPreviousFrame();
	CloseHandle(fenceEvent);

	BOOL fullscreen = false;
	swapChain->GetFullscreenState(&fullscreen, nullptr);
	if (fullscreen)
		swapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(commandQueue);
	SAFE_RELEASE(preRenderCommandList);
	SAFE_RELEASE(postRenderCommandList);
	SAFE_RELEASE(ImGuiCommandList);

	for (std::pair<const int, DxObjects::Dx12GraphicObject*> object : graphicsObjects)
		delete object.second;

	SAFE_RELEASE(pipelineStateObject);
	SAFE_RELEASE(rootSignature);

	SAFE_RELEASE(rtvDescriptorHeap);
	SAFE_RELEASE(SrvDescriptorHeap);

	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(depthStencilDescriptorHeap);

	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		SAFE_RELEASE(renderTargets[i]);
		SAFE_RELEASE(preRenderCommandAllocator[i]);
		SAFE_RELEASE(objectCommandAllocator[i]);
		SAFE_RELEASE(imGuiCommandAllocator[i]);
		SAFE_RELEASE(postRenderCommandAllocator[i]);
		SAFE_RELEASE(fence[i]);
	};

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	IDXGIDebug1* pDebug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
	{
		pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
		pDebug->Release();
	}

	delete factory;
	SAFE_RELEASE(device);

	return true;
}

void Module::Render::DirectX12::DirectX12::SetCameraViewMatrix(DirectX::XMFLOAT4X4 _camera_view)
{
	cameraViewMatrix = _camera_view;
}

void Module::Render::DirectX12::DirectX12::SetCameraProjectionMatrix(const DirectX::XMFLOAT4X4 _camera_projection)
{
	cameraProjectionMatrix = _camera_projection;
}

bool Module::Render::DirectX12::DirectX12::PreRender()
{

	TRYFUNC(preRenderCommandAllocator[frameIndex]->Reset());
	TRYFUNC(objectCommandAllocator[frameIndex]->Reset());
	TRYFUNC(imGuiCommandAllocator[frameIndex]->Reset());
	TRYFUNC(postRenderCommandAllocator[frameIndex]->Reset());

	std::vector<std::future<bool>>* threads = new std::vector<std::future<bool>>;

	threads->push_back(engine->ExecuteFunctionOnThread<bool>(std::bind(&Module::Render::DirectX12::DirectX12::PreparePreRenderCommandList, this)));
	for (const std::pair<const int, DxObjects::Dx12GraphicObject*> pair : graphicsObjects)
	{
		threads->push_back(engine->ExecuteFunctionOnThread<bool>(std::bind(&Module::Render::DirectX12::DirectX12::PrepareObjectCommandList, this, pair.second)));
	}

	threads->push_back(engine->ExecuteFunctionOnThread<bool>(std::bind(&Module::Render::DirectX12::DirectX12::PrepareImGuiCommandList, this, ImGuiCommandList)));

	threads->push_back(engine->ExecuteFunctionOnThread<bool>(std::bind(&Module::Render::DirectX12::DirectX12::PreparePostRenderCommandList, this)));

	engine->WaitForThreadsEnd(*threads);

	delete threads;

	return true;
}

bool Module::Render::DirectX12::DirectX12::InitImGui()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	TRYFUNC(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&SrvDescriptorHeap)));

	auto window = engine->GetModule<Module::Display::Window>();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());
	HWND* hwnd = window->GetHandle();
	ImGui_ImplWin32_Init(*hwnd);
	ImGui_ImplDX12_Init(device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, SrvDescriptorHeap, SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	ImGui::StyleColorsDark();
	RECT rect;
	GetClientRect(*hwnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;
	PostMessage(*hwnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(width, height));

	return true;
}

bool Module::Render::DirectX12::DirectX12::WindowResize(WPARAM _window_param, LPARAM _long_param)
{
	if (device != nullptr && _window_param != SIZE_MINIMIZED)
	{
		WaitForPreviousFrame();

		for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
			SAFE_RELEASE(renderTargets[i]);
		SAFE_RELEASE(depthStencilBuffer);

		TRYFUNC(swapChain->ResizeBuffers(0, (UINT)LOWORD(_long_param), (UINT)HIWORD(_long_param), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle((rtvDescriptorHeap)->GetCPUDescriptorHandleForHeapStart());

		for (unsigned int i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			TRYFUNC(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));

			device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);

			rtvHandle.Offset(1, rtvDescriptorSize);
		}
		factory->MakeDepthStencilBuffer(&depthStencilDescriptorHeap, &depthStencilBuffer, (UINT)LOWORD(_long_param), (UINT)HIWORD(_long_param));

		viewport.TopLeftX = (FLOAT)LOWORD(_long_param) * 200 / 1920;
		viewport.TopLeftY = (FLOAT)HIWORD(_long_param) * 8 / 1009;
		viewport.Width = (FLOAT)LOWORD(_long_param) * 1670 / 1920;
		viewport.Height = (FLOAT)HIWORD(_long_param) * 830 / 1009;

		scissorRect.left = viewport.TopLeftX;
		scissorRect.top = viewport.TopLeftY;
		scissorRect.right = viewport.Width;
		scissorRect.bottom = viewport.Height;

		return true;
	}

	return false;
}

bool Module::Render::DirectX12::DirectX12::CleanGraphicsObjects()
{
	for (std::pair<const int, DxObjects::Dx12GraphicObject*> object : graphicsObjects)
		delete object.second;
	graphicsObjects.clear();

	return true;
}

bool Module::Render::DirectX12::DirectX12::DeleteGraphicObjects(const unsigned int _id)
{
	graphicsObjects.erase(_id);

	return true;
}

D3D12_VIEWPORT* Module::Render::DirectX12::DirectX12::GetViewport() 
{
	return &viewport;
}


bool Module::Render::DirectX12::DirectX12::PostRender()
{
	return true;
}

bool Module::Render::DirectX12::DirectX12::UpdatePipeline()
{
	frameIndex = swapChain->GetCurrentBackBufferIndex();

	return true;
}

bool Module::Render::DirectX12::DirectX12::WaitForPreviousFrame()
{
	fenceValue[frameIndex]++;

	TRYFUNC(commandQueue->Signal(fence[frameIndex], fenceValue[frameIndex]));

	if (fence[frameIndex]->GetCompletedValue() < fenceValue[frameIndex])
	{
		TRYFUNC(fence[frameIndex]->SetEventOnCompletion(fenceValue[frameIndex], fenceEvent));

		WaitForSingleObject(fenceEvent, INFINITE);
	}

	return true;
}

bool Module::Render::DirectX12::DirectX12::PreparePreRenderCommandList()
{
	std::unique_lock<std::mutex> frameIndexLock(frameIndexMutex);
	std::unique_lock<std::mutex> pipelineStateObjectLock(pipelineStateObjectMutex);

	TRYFUNC(preRenderCommandList->Reset(preRenderCommandAllocator[frameIndex], pipelineStateObject));

	pipelineStateObjectLock.unlock();
	std::unique_lock<std::mutex> renderTargetsLock(renderTargetsMutex);

	CD3DX12_RESOURCE_BARRIER present_to_target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	renderTargetsLock.unlock(); 
	preRenderCommandList->ResourceBarrier(1, &present_to_target_barrier);

	std::unique_lock<std::mutex> rtvDescriptorHeapLock(rtvDescriptorHeapMutex);
	std::unique_lock<std::mutex> rtvDescriptorSizeLock(rtvDescriptorSizeMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view_handle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
	
	rtvDescriptorHeapLock.unlock();
	frameIndexLock.unlock();
	rtvDescriptorSizeLock.unlock();
	std::unique_lock<std::mutex> depthStencilDescriptorHeapLock(depthStencilDescriptorHeapMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_handle(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	depthStencilDescriptorHeapLock.unlock();

	preRenderCommandList->OMSetRenderTargets(1, &render_target_view_handle, FALSE, &depth_stencil_view_handle);

	auto color = DirectX::Colors::LightSkyBlue;
	float clearColor[] = { color[0], color[1], color[2], color[3] };
	preRenderCommandList->ClearRenderTargetView(render_target_view_handle, clearColor, 0, nullptr);
	preRenderCommandList->ClearDepthStencilView(depth_stencil_view_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	std::unique_lock<std::mutex> rootSignatureLock(rootSignatureMutex);

	preRenderCommandList->SetGraphicsRootSignature(rootSignature);

	rootSignatureLock.unlock();

	TRYFUNC(preRenderCommandList->Close());

	return true;
}

bool Module::Render::DirectX12::DirectX12::PreparePostRenderCommandList()
{
	std::unique_lock<std::mutex> frameIndexLock(frameIndexMutex);
	std::unique_lock<std::mutex> pipelineStateObjectLock(pipelineStateObjectMutex);

	TRYFUNC(postRenderCommandList->Reset(postRenderCommandAllocator[frameIndex], pipelineStateObject));

	pipelineStateObjectLock.unlock();
	std::unique_lock<std::mutex> renderTargetsLock(renderTargetsMutex);

	CD3DX12_RESOURCE_BARRIER target_to_present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	
	renderTargetsLock.unlock();
	frameIndexLock.unlock();
	
	postRenderCommandList->ResourceBarrier(1, &target_to_present_barrier);

	TRYFUNC(postRenderCommandList->Close());

	return true;
}

bool Module::Render::DirectX12::DirectX12::PrepareObjectCommandList(DxObjects::Dx12GraphicObject* _graphic_object)
{
	DirectX::XMFLOAT4X4 _world_matrix = _graphic_object->GetTransform()->GetWorldMatrix();
	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&_world_matrix);

	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&cameraViewMatrix);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMLoadFloat4x4(&cameraProjectionMatrix);

	DirectX::XMMATRIX resultMatrix = worldMatrix * viewMatrix * projectionMatrix;

	DirectX::XMMATRIX transposedMatrix = DirectX::XMMatrixTranspose(resultMatrix);

	DirectX::XMFLOAT4X4 finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, transposedMatrix);
	const auto virtual_gpu_adress = _graphic_object->GetConstantBuffer()->Map();
	memcpy(virtual_gpu_adress, &finalMatrix, sizeof finalMatrix);
	_graphic_object->GetConstantBuffer()->Unmap();

	ID3D12GraphicsCommandList* command_list = _graphic_object->GetCommandList();

	std::unique_lock<std::mutex> objectCommandAllcoatorLock(objectCommandAllocatorMutex);
	std::unique_lock<std::mutex> frameIndexLock(frameIndexMutex);
	std::unique_lock<std::mutex> pipelineStateObjectLock(pipelineStateObjectMutex);

	TRYFUNC(command_list->Reset(objectCommandAllocator[frameIndex], pipelineStateObject));

	pipelineStateObjectLock.unlock();
	std::unique_lock<std::mutex> rtvDescriptorHeapLock(rtvDescriptorHeapMutex);
	std::unique_lock<std::mutex> rtvDescriptorSizeLock(rtvDescriptorSizeMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view_handle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

	frameIndexLock.unlock();
	rtvDescriptorHeapLock.unlock();
	rtvDescriptorSizeLock.unlock();
	std::unique_lock<std::mutex> depthStencilDescriptorHeapLock(depthStencilDescriptorHeapMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_handle(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	depthStencilDescriptorHeapLock.unlock();

	command_list->OMSetRenderTargets(1, &render_target_view_handle, FALSE, &depth_stencil_view_handle);

	std::unique_lock<std::mutex> rootSignatureLock(rootSignatureMutex);

	command_list->SetGraphicsRootSignature(rootSignature);

	rootSignatureLock.unlock();

	ID3D12DescriptorHeap* descriptor_heaps[] = { _graphic_object->GetTextureDesciptorHeap() };
	command_list->SetDescriptorHeaps(_countof(descriptor_heaps), descriptor_heaps);
	command_list->SetGraphicsRootDescriptorTable(1, _graphic_object->GetTextureDesciptorHeap()->GetGPUDescriptorHandleForHeapStart());
	command_list->RSSetViewports(1, &viewport);
	command_list->RSSetScissorRects(1, &scissorRect);

	_graphic_object->PrepareCommandList();
	TRYFUNC(command_list->Close());
	objectCommandAllcoatorLock.unlock();

	return true;
}

bool Module::Render::DirectX12::DirectX12::PrepareImGuiCommandList(ID3D12GraphicsCommandList* _command_list)
{
	std::unique_lock<std::mutex> frameIndexLock(frameIndexMutex);
	std::unique_lock<std::mutex> pipelineStateObjectLock(pipelineStateObjectMutex);

	TRYFUNC(_command_list->Reset(imGuiCommandAllocator[frameIndex], pipelineStateObject));

	pipelineStateObjectLock.unlock();
	std::unique_lock<std::mutex> rtvDescriptorHeapLock(rtvDescriptorHeapMutex);
	std::unique_lock<std::mutex> rtvDescriptorSizeLock(rtvDescriptorSizeMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE render_target_view_handle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

	frameIndexLock.unlock();
	rtvDescriptorHeapLock.unlock();
	rtvDescriptorSizeLock.unlock();
	std::unique_lock<std::mutex> depthStencilDescriptorHeapLock(depthStencilDescriptorHeapMutex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_handle(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	depthStencilDescriptorHeapLock.unlock();

	_command_list->OMSetRenderTargets(1, &render_target_view_handle, FALSE, &depth_stencil_view_handle);
	_command_list->SetDescriptorHeaps(1, &SrvDescriptorHeap);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _command_list);

	TRYFUNC(_command_list->Close());

	return true;

}
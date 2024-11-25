#include <d3dcompiler.h>
#include <string>

#include "Config/d3dx12.h"
#include "Dx12Factory.h"
#include "Source/Core/CoreType/Vertex.h"
#include "Engine.h"
#include "Modules/Display/Window.h"
#include <wrl/client.h>

bool Module::Render::DirectX12::Dx12Factory::InitFactory() 
{
	TRYFUNC(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

	Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeDevice(ID3D12Device** _device)
{
	std::vector<IDXGIAdapter1*> compatible_adapters = GetCompatibleAdaptersList();

	if (compatible_adapters.size() == 0)
		return false;

	TRYFUNC(D3D12CreateDevice(compatible_adapters[0], D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));

	*_device = device;

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeCommandQueue(ID3D12CommandQueue** _command_queue) 
{
	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cqDesc.NodeMask = 1;

	TRYFUNC(device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(_command_queue)));

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeSwapChain(ID3D12CommandQueue* _command_queue, const UINT _width, const UINT _height, const UINT _frame_buffer_count, HWND _window, const BOOL _fullscreen, IDXGISwapChain3** _swap_chain)
{
	DXGI_SAMPLE_DESC sample_desc;
	sample_desc.Count = 1;
	sample_desc.Quality = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
	fullscreen_desc.Windowed = !_fullscreen;

	DXGI_SWAP_CHAIN_DESC1 swap_chain_descriptor = {};
	swap_chain_descriptor.BufferCount = _frame_buffer_count;
	swap_chain_descriptor.Width = _width;
	swap_chain_descriptor.Height = _height;
	swap_chain_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_descriptor.SampleDesc = sample_desc;
	swap_chain_descriptor.Scaling = DXGI_SCALING_STRETCH;
	swap_chain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

	IDXGISwapChain1* swap_chain = nullptr;

	TRYFUNC(dxgiFactory->CreateSwapChainForHwnd(_command_queue, _window, &swap_chain_descriptor, &fullscreen_desc, nullptr, &swap_chain));
	*_swap_chain = static_cast<IDXGISwapChain3*>(swap_chain);

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeRenderTargetView(const UINT _frame_buffer_count, ID3D12DescriptorHeap** _rtv_descriptor_heap, UINT* _rtv_descriptor_size, IDXGISwapChain3* _swap_chain, ID3D12Resource** _render_targets)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = _frame_buffer_count; 
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 1;

	TRYFUNC(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_rtv_descriptor_heap)));

	*_rtv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle((*_rtv_descriptor_heap)->GetCPUDescriptorHandleForHeapStart());

	for (unsigned int i = 0; i < _frame_buffer_count; i++)
	{
		TRYFUNC(_swap_chain->GetBuffer(i, IID_PPV_ARGS(&_render_targets[i])));

		device->CreateRenderTargetView(_render_targets[i], nullptr, rtvHandle);

		rtvHandle.Offset(1, *_rtv_descriptor_size);
	}

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeCommandAllocator(const UINT _frame_buffer_count, ID3D12CommandAllocator** _command_allocator)
{
	for (unsigned int i = 0; i < _frame_buffer_count; i++)
	{
		TRYFUNC(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_command_allocator[i])));
	}

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeCommandList(ID3D12CommandAllocator* _command_allocator, ID3D12GraphicsCommandList** _command_list)
{
	TRYFUNC(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _command_allocator, nullptr, IID_PPV_ARGS(_command_list)));

	(*_command_list)->Close();

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeFence(const UINT _frame_buffer_count, ID3D12Fence** _fence, UINT64* _fence_value, HANDLE* _fence_event)
{
	for (unsigned int i = 0; i < _frame_buffer_count; i++)
	{
		TRYFUNC(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence[i])));
		_fence_value[i] = 0;
	}

	*_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeRootSignature(ID3D12RootSignature** _root_signature)
{
	D3D12_ROOT_DESCRIPTOR root_cbv_descriptor;
	root_cbv_descriptor.RegisterSpace = 0;
	root_cbv_descriptor.ShaderRegister = 0;

	D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[1]; 
	descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; 
	descriptorTableRanges[0].NumDescriptors = 1; 
	descriptorTableRanges[0].BaseShaderRegister = 0; 
	descriptorTableRanges[0].RegisterSpace = 0; 
	descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges);
	descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; 

	D3D12_ROOT_PARAMETER  rootParameters[2];
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; 
	rootParameters[0].Descriptor = root_cbv_descriptor; 
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; 

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; 
	rootParameters[1].DescriptorTable = descriptorTable;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init(_countof(rootParameters), rootParameters, 1, &sampler, 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

	ID3DBlob* serialized_root_signature = nullptr;
	D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized_root_signature, nullptr);

	TRYFUNC(device->CreateRootSignature(0, serialized_root_signature->GetBufferPointer(), serialized_root_signature->GetBufferSize(), IID_PPV_ARGS(_root_signature)));
	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeVertexShader(const LPCWSTR _path, D3D12_SHADER_BYTECODE* _shader_bytecode)
{
	ID3DBlob* vertexShader;
	ID3DBlob* errorBuff;
	hr = D3DCompileFromFile(_path, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
	CHECK_BLOB(errorBuff);

	_shader_bytecode->BytecodeLength = vertexShader->GetBufferSize();
	_shader_bytecode->pShaderBytecode = vertexShader->GetBufferPointer();

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakePixelShader(const LPCWSTR _path, D3D12_SHADER_BYTECODE* _shader_bytecode)
{
	ID3DBlob* pixelShader;
	ID3DBlob* errorBuff;
	hr = D3DCompileFromFile(_path, nullptr, nullptr, "main","ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
	CHECK_BLOB(errorBuff);

	_shader_bytecode->BytecodeLength = pixelShader->GetBufferSize();
	_shader_bytecode->pShaderBytecode = pixelShader->GetBufferPointer();

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakeDepthStencilBuffer(ID3D12DescriptorHeap** _descriptor, ID3D12Resource** _buffer, const UINT _width, const UINT _height)
{
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
	heap_desc.NumDescriptors = 1;
	heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	TRYFUNC(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(_descriptor)));

	D3D12_RESOURCE_DESC depth_stencil_descriptor = {};
	depth_stencil_descriptor.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depth_stencil_descriptor.Alignment = 0;
	depth_stencil_descriptor.Width = _width;
	depth_stencil_descriptor.Height = _height;
	depth_stencil_descriptor.DepthOrArraySize = 1;
	depth_stencil_descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_descriptor.SampleDesc.Count = 1;
	depth_stencil_descriptor.SampleDesc.Quality = 0;
	depth_stencil_descriptor.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depth_stencil_descriptor.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE depth_optimized_clear_value;
	depth_optimized_clear_value.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_optimized_clear_value.DepthStencil.Depth = 1.0f;
	depth_optimized_clear_value.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heap_properties(D3D12_HEAP_TYPE_DEFAULT);
	device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &depth_stencil_descriptor, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depth_optimized_clear_value, IID_PPV_ARGS(_buffer));
	(*_descriptor)->SetName(L"Depth/Stencil Resource Heap");
	(*_buffer)->SetName(L"Depth/Stencil Texture");

	device->CreateDepthStencilView(*_buffer, nullptr, (*_descriptor)->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::MakePipelineStateObject(ID3D12RootSignature* _root_signature, D3D12_SHADER_BYTECODE* _vertex_shader_bytecode, D3D12_SHADER_BYTECODE* _pixel_shader_bytecode, ID3D12PipelineState** _pipeline_state_object)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	}; 

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; 
	psoDesc.InputLayout = inputLayoutDesc;
	psoDesc.pRootSignature = _root_signature;
	psoDesc.VS = *_vertex_shader_bytecode;
	psoDesc.PS = *_pixel_shader_bytecode;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc = sampleDesc; 
	psoDesc.SampleMask = 0xffffffff;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TRYFUNC(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(_pipeline_state_object)));

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::CreateVertexBuffer(Dx12CommandExecutionPack _pack, const std::string _name,Core::CoreType::Vertex* _vertices_array, const unsigned int _vertices_count, ID3D12GraphicsCommandList* _command_list, D3D12_VERTEX_BUFFER_VIEW** _vertex_buffer_view)
{
	TRYFUNC(_command_list->Reset(_pack.commandAllocator, _pack.pipelineState));

	Core::CoreType::Vertex* vertex_list = new Core::CoreType::Vertex[_vertices_count];

	for (unsigned int i = 0; i < _vertices_count; ++i) {
		vertex_list[i] = _vertices_array[i];
	}

	int vBufferSize = _vertices_count * sizeof(Core::CoreType::Vertex);

	CD3DX12_HEAP_PROPERTIES default_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES upload_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(vBufferSize);

	ID3D12Resource* vertexBuffer = nullptr;
	device->CreateCommittedResource(&default_heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&vertexBuffer));
	vertexBuffer->SetName((LPCWSTR)(_name + (" - Vertex Buffer")).c_str());

	ID3D12Resource* vBufferUploadHeap;
	device->CreateCommittedResource(&upload_heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vBufferUploadHeap));
	vBufferUploadHeap->SetName((LPCWSTR)(_name + (" - Vertex Upload")).c_str());

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(vertex_list);
	vertexData.RowPitch = vBufferSize; 
	vertexData.SlicePitch = vBufferSize;

	UpdateSubresources(_command_list, vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);

	CD3DX12_RESOURCE_BARRIER resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	_command_list->ResourceBarrier(1, &resource_barrier);

	_command_list->Close();
	ID3D12CommandList* ppCommandLists[] = { _command_list };
	_pack.commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	HANDLE fence_event = nullptr;

	(*_pack.fenceValue)++;
	TRYFUNC(_pack.commandQueue->Signal(_pack.fence, *_pack.fenceValue));
	_pack.fence->SetEventOnCompletion(*_pack.fenceValue, fence_event);
	WaitForSingleObject(fence_event, INFINITE);

	*_vertex_buffer_view = new D3D12_VERTEX_BUFFER_VIEW;
	(*_vertex_buffer_view)->BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	(*_vertex_buffer_view)->StrideInBytes = sizeof(Core::CoreType::Vertex);
	(*_vertex_buffer_view)->SizeInBytes = vBufferSize;

	delete[] vertex_list;

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::CreateIndexBuffer(Dx12CommandExecutionPack _pack, const std::string _name, unsigned int* _indexes_array, const unsigned int _indexes_count, ID3D12GraphicsCommandList* _command_list, D3D12_INDEX_BUFFER_VIEW** _index_buffer_view)
{
	TRYFUNC(_command_list->Reset(_pack.commandAllocator, _pack.pipelineState));

	unsigned int* index_list = new unsigned int[_indexes_count];

	for (unsigned int i = 0; i < _indexes_count; ++i)
		index_list[i] = _indexes_array[i];

	int buffer_size = _indexes_count * sizeof(unsigned int);

	CD3DX12_HEAP_PROPERTIES default_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES upload_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC buffer_size_desc = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

	ID3D12Resource* index_buffer = nullptr;
	device->CreateCommittedResource(&default_heap_properties, D3D12_HEAP_FLAG_NONE, &buffer_size_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&index_buffer));
	index_buffer->SetName(((LPCWSTR)(_name + " - Index Buffer").c_str()));

	ID3D12Resource* index_buffer_upload_heap;
	device->CreateCommittedResource(&upload_heap_properties, D3D12_HEAP_FLAG_NONE, &buffer_size_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&index_buffer_upload_heap));
	index_buffer_upload_heap->SetName(((LPCWSTR)(_name + " - Index Upload").c_str()));

	D3D12_SUBRESOURCE_DATA index_data;
	index_data.pData = reinterpret_cast<BYTE*>(index_list);
	index_data.RowPitch = buffer_size;
	index_data.SlicePitch = buffer_size;

	UpdateSubresources(_command_list, index_buffer, index_buffer_upload_heap, 0, 0, 1, &index_data);

	CD3DX12_RESOURCE_BARRIER transition_barrier = CD3DX12_RESOURCE_BARRIER::Transition(index_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	_command_list->ResourceBarrier(1, &transition_barrier);

	_command_list->Close();
	ID3D12CommandList* pp_command_lists[] = { _command_list };
	_pack.commandQueue->ExecuteCommandLists(_countof(pp_command_lists), pp_command_lists);

	HANDLE fence_event = nullptr;

	(*_pack.fenceValue)++;
	TRYFUNC(_pack.commandQueue->Signal(_pack.fence, *_pack.fenceValue));
	_pack.fence->SetEventOnCompletion(*_pack.fenceValue, fence_event);
	WaitForSingleObject(fence_event, INFINITE);

	*_index_buffer_view = new D3D12_INDEX_BUFFER_VIEW;
	(*_index_buffer_view)->BufferLocation = index_buffer->GetGPUVirtualAddress();
	(*_index_buffer_view)->Format = DXGI_FORMAT_R32_UINT;
	(*_index_buffer_view)->SizeInBytes = buffer_size;

	delete[] index_list;

	return true;
}

bool Module::Render::DirectX12::Dx12Factory::CreateTextureBuffer(Dx12CommandExecutionPack _pack, const std::string _name, ID3D12GraphicsCommandList* _command_list, LPCWSTR _path, ID3D12DescriptorHeap** _texture_descriptor_heap)
{
	TRYFUNC(_command_list->Reset(_pack.commandAllocator, _pack.pipelineState));

	D3D12_RESOURCE_DESC textureDesc;
	int imageBytesPerRow;
	BYTE* imageData;
	int imageSize = LoadImageDataFromFile(&imageData, textureDesc, _path, imageBytesPerRow);

	if (imageSize <= 0)
	{
		return false;
	}

	CD3DX12_HEAP_PROPERTIES default_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES upload_heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
   
	ID3D12Resource* textureBuffer;
	device->CreateCommittedResource(&default_heap_properties, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST,  nullptr, IID_PPV_ARGS(&textureBuffer));
	textureBuffer->SetName(((LPCWSTR)(_name + " - Texture Buffer").c_str()));

	UINT64 textureUploadBufferSize;
	device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

	CD3DX12_RESOURCE_DESC buffer_size_desc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);
	ID3D12Resource* textureBufferUploadHeap;
	device->CreateCommittedResource( &upload_heap_properties, D3D12_HEAP_FLAG_NONE, &buffer_size_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureBufferUploadHeap));
	textureBufferUploadHeap->SetName(((LPCWSTR)(_name + " - Texture Upload").c_str()));

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &imageData[0]; 
	textureData.RowPitch = imageBytesPerRow;
	textureData.SlicePitch = imageBytesPerRow * textureDesc.Height; 

	UpdateSubresources(_command_list, textureBuffer, textureBufferUploadHeap, 0, 0, 1, &textureData);

	CD3DX12_RESOURCE_BARRIER transition_barrier = CD3DX12_RESOURCE_BARRIER::Transition(textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	_command_list->ResourceBarrier(1, &transition_barrier);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	TRYFUNC(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_texture_descriptor_heap)));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(textureBuffer, &srvDesc, (*_texture_descriptor_heap)->GetCPUDescriptorHandleForHeapStart());

	_command_list->Close();
	ID3D12CommandList* pp_command_lists[] = { _command_list };
	_pack.commandQueue->ExecuteCommandLists(_countof(pp_command_lists), pp_command_lists);

	HANDLE fence_event = nullptr;

	(*_pack.fenceValue)++;
	TRYFUNC(_pack.commandQueue->Signal(_pack.fence, *_pack.fenceValue));
	_pack.fence->SetEventOnCompletion(*_pack.fenceValue, fence_event);
	WaitForSingleObject(fence_event, INFINITE);

	delete imageData;

	return true;
}

int Module::Render::DirectX12::Dx12Factory::LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow)
{
	static IWICImagingFactory* wicFactory;

	IWICBitmapDecoder* wicDecoder = NULL;
	IWICBitmapFrameDecode* wicFrame = NULL;
	IWICFormatConverter* wicConverter = NULL;

	bool imageConverted = false;

	if (wicFactory == NULL)
	{
		CoInitialize(NULL);

		TRYFUNC(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));
	}

	TRYFUNC(wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder));

	TRYFUNC(wicDecoder->GetFrame(0, &wicFrame));

	WICPixelFormatGUID pixelFormat;
	TRYFUNC(wicFrame->GetPixelFormat(&pixelFormat));

	UINT textureWidth, textureHeight;
	TRYFUNC(wicFrame->GetSize(&textureWidth, &textureHeight));

	DXGI_FORMAT dxgiFormat = GetDXGIFormatFromWICFormat(pixelFormat);

	if (dxgiFormat == DXGI_FORMAT_UNKNOWN)
	{
		WICPixelFormatGUID convertToPixelFormat = GetConvertToWICFormat(pixelFormat);

		if (convertToPixelFormat == GUID_WICPixelFormatDontCare) return 0;

		dxgiFormat = GetDXGIFormatFromWICFormat(convertToPixelFormat);

		TRYFUNC(wicFactory->CreateFormatConverter(&wicConverter));

		BOOL canConvert = FALSE;
		hr = wicConverter->CanConvert(pixelFormat, convertToPixelFormat, &canConvert);
		if (FAILED(hr) || !canConvert) return 0;

		TRYFUNC(wicConverter->Initialize(wicFrame, convertToPixelFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom));

		imageConverted = true;
	}

	int bitsPerPixel = GetDXGIFormatBitsPerPixel(dxgiFormat);
	bytesPerRow = (textureWidth * bitsPerPixel) / 8;
	int imageSize = bytesPerRow * textureHeight;

	*imageData = (BYTE*)malloc(imageSize);

	if (imageConverted)
	{
		TRYFUNC(wicConverter->CopyPixels(0, bytesPerRow, imageSize, *imageData));
	}
	else
	{
		TRYFUNC(wicFrame->CopyPixels(0, bytesPerRow, imageSize, *imageData));
	}

	resourceDescription = {};
	resourceDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDescription.Alignment = 0;
	resourceDescription.Width = textureWidth;
	resourceDescription.Height = textureHeight;
	resourceDescription.DepthOrArraySize = 1;
	resourceDescription.MipLevels = 1;
	resourceDescription.Format = dxgiFormat;
	resourceDescription.SampleDesc.Count = 1;
	resourceDescription.SampleDesc.Quality = 0;
	resourceDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

	return imageSize;
}

std::vector<IDXGIAdapter1*> Module::Render::DirectX12::Dx12Factory::GetAdaptersList() const
{
	std::vector<IDXGIAdapter1*> adapters;
	IDXGIAdapter1* adapter;
	int adapter_index = 0;

	while (dxgiFactory->EnumAdapters1(adapter_index, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(adapter);
		++adapter_index;
	}

	return adapters;
}

std::vector<IDXGIAdapter1*> Module::Render::DirectX12::Dx12Factory::GetCompatibleAdaptersList() const
{
	std::vector<IDXGIAdapter1*> adapters = GetAdaptersList();
	std::vector<IDXGIAdapter1*> compatible_adapters;

	for (IDXGIAdapter1* adapter : adapters)
	{
		const HRESULT adapter_compatible = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(adapter_compatible))
			compatible_adapters.push_back(adapter);
	}

	return compatible_adapters;
}

std::vector<IDXGIOutput*> Module::Render::DirectX12::Dx12Factory::GetOutputList(IDXGIAdapter1* _adapter)
{
	std::vector<IDXGIOutput*> outputs;
	IDXGIOutput* output;
	int output_index = 0;

	while (_adapter->EnumOutputs(output_index, &output) != DXGI_ERROR_NOT_FOUND)
	{
		outputs.push_back(output);
		++output_index;
	}

	return outputs;
}

DXGI_FORMAT Module::Render::DirectX12::Dx12Factory::GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFloat) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAHalf) return DXGI_FORMAT_R16G16B16A16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBA) return DXGI_FORMAT_R16G16B16A16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA) return DXGI_FORMAT_R8G8B8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGRA) return DXGI_FORMAT_B8G8R8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR) return DXGI_FORMAT_B8G8R8X8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102XR) return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;

	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102) return DXGI_FORMAT_R10G10B10A2_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGRA5551) return DXGI_FORMAT_B5G5R5A1_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR565) return DXGI_FORMAT_B5G6R5_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFloat) return DXGI_FORMAT_R32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayHalf) return DXGI_FORMAT_R16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGray) return DXGI_FORMAT_R16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppGray) return DXGI_FORMAT_R8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppAlpha) return DXGI_FORMAT_A8_UNORM;

	else return DXGI_FORMAT_UNKNOWN;
}

WICPixelFormatGUID Module::Render::DirectX12::Dx12Factory::GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormatBlackWhite) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat1bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppIndexed) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppGray) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppGray) return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayFixedPoint) return GUID_WICPixelFormat16bppGrayHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFixedPoint) return GUID_WICPixelFormat32bppGrayFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR555) return GUID_WICPixelFormat16bppBGRA5551;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR101010) return GUID_WICPixelFormat32bppRGBA1010102;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppBGR) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppRGB) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPBGRA) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPRGBA) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGB) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGR) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPBGRA) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGRFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRAFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBFixedPoint) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBHalf) return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppPRGBAFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFloat) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFixedPoint) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBE) return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppCMYK) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppCMYK) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat40bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat80bppCMYKAlpha) return GUID_WICPixelFormat64bppRGBA;

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGB) return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGB) return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBAHalf) return GUID_WICPixelFormat64bppRGBAHalf;
#endif

	else return GUID_WICPixelFormatDontCare;
}

int Module::Render::DirectX12::Dx12Factory::GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat)
{
	if (dxgiFormat == DXGI_FORMAT_R32G32B32A32_FLOAT) return 128;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_FLOAT) return 64;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_UNORM) return 64;
	else if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8A8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8X8_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM) return 32;

	else if (dxgiFormat == DXGI_FORMAT_R10G10B10A2_UNORM) return 32;
	else if (dxgiFormat == DXGI_FORMAT_B5G5R5A1_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_B5G6R5_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R32_FLOAT) return 32;
	else if (dxgiFormat == DXGI_FORMAT_R16_FLOAT) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R16_UNORM) return 16;
	else if (dxgiFormat == DXGI_FORMAT_R8_UNORM) return 8;
	else if (dxgiFormat == DXGI_FORMAT_A8_UNORM) return 8;
}
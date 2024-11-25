#include "Dx12GraphicObject.h"

Module::Render::DirectX12::DxObjects::Dx12GraphicObject::Dx12GraphicObject(unsigned int _id, std::string _name, Core::CoreType::Transform* _transform, ID3D12GraphicsCommandList* _command_list, Core::CoreType::Vertex* _vertices_array, unsigned int _vertices_count, D3D12_VERTEX_BUFFER_VIEW* _vertex_buffer_view, ID3D12DescriptorHeap* _textureDescriptorHeap, unsigned int* _indexs_array, unsigned int _indexs_count, D3D12_INDEX_BUFFER_VIEW* _index_buffer_view, Dx12ConstantBuffer* _constant_buffer) : id(_id), commandList(_command_list), vertexBufferView(_vertex_buffer_view), indexBufferView(_index_buffer_view), constantBuffer(_constant_buffer), name(_name), transform(_transform), vertices(_vertices_array), verticesCount(_vertices_count), indexsArray(_indexs_array), indexsCount(_indexs_count), textureDescriptorHeap(_textureDescriptorHeap)
{
	commandList->SetName((LPCWSTR)(_name ," Command List"));
	if (_indexs_array && _indexs_count)
		indexed = true;
}

Module::Render::DirectX12::DxObjects::Dx12GraphicObject::~Dx12GraphicObject()
{
	SAFE_RELEASE(commandList);
	SAFE_RELEASE(textureDescriptorHeap);

	delete vertexBufferView;
	delete indexBufferView;
	delete constantBuffer;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::PrepareCommandList() const
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, vertexBufferView);

	if (constantBuffer)
		commandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetResource()->GetGPUVirtualAddress());

	if (IsIndexed())
	{
		commandList->IASetIndexBuffer(indexBufferView);
		commandList->DrawIndexedInstanced(indexsCount, 1, 0, 0, 0);
	}
	else
		commandList->DrawInstanced(verticesCount / 3, 1, 0, 0);
}

bool Module::Render::DirectX12::DxObjects::Dx12GraphicObject::IsIndexed() const
{
	return indexed;
}

unsigned Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetId() const
{
	return id;
}

ID3D12GraphicsCommandList* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetCommandList() const
{
	return commandList;
}

Core::CoreType::Transform* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetTransform() const
{
	return transform;
}

D3D12_VERTEX_BUFFER_VIEW* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetVertexBufferView() const
{
	return vertexBufferView;
}

unsigned Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetVertexCount() const
{
	return verticesCount;
}

unsigned Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetIndexCount() const
{
	return indexsCount;
}

Module::Render::DirectX12::DxObjects::Dx12ConstantBuffer* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetConstantBuffer() const
{
	return constantBuffer;
}

D3D12_INDEX_BUFFER_VIEW* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetIndexBufferView() const
{
	return indexBufferView;
}

ID3D12DescriptorHeap* Module::Render::DirectX12::DxObjects::Dx12GraphicObject::GetTextureDesciptorHeap() const 
{
	return textureDescriptorHeap;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetVertices(Core::CoreType::Vertex* _vertices, unsigned int _vertices_count)
{
	vertices = _vertices;
	verticesCount = _vertices_count;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW* _vertex_buffer_view) 
{
	vertexBufferView = _vertex_buffer_view;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetIndexs(unsigned int* _indexs_array, unsigned int _indexs_count)
{
	indexsArray = _indexs_array;
	indexsCount = _indexs_count;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW* _index_buffer_view) 
{
	indexBufferView = _index_buffer_view;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetConstantBuffer(Dx12ConstantBuffer* _constant_buffer) 
{
	constantBuffer = _constant_buffer;
}

void Module::Render::DirectX12::DxObjects::Dx12GraphicObject::SetTextureDescriptorHeap(ID3D12DescriptorHeap* _textureDescriptorHeap) 
{
	textureDescriptorHeap = _textureDescriptorHeap;
}
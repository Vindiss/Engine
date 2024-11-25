#include "Modules/Render/Renderer.h"
#include "DirectX/Dx12.h"
#include "Editor/Editor.h"

bool Module::Render::Renderer::Init()
{
	switch (api) { 
		case DirectX12Api: 
			renderer = new DirectX12::DirectX12(); 
	}
	switch (editorInterface) {
		case ImGuiInterface:
			editor = new Editor::Editor();
	}

	editor->Init();

	return renderer->Initialize();
}

bool Module::Render::Renderer::Start()
{
	renderer->CreatePipeline();
	renderer->InitImGui();
	editor->Start();

	return true;
}

bool Module::Render::Renderer::Update()
{
	renderer->SetCameraProjectionMatrix(activeCamera->GetProjectionMatrix());
	renderer->SetCameraViewMatrix(activeCamera->GetViewMatrix());
	editor->Update();
	renderer->Render();
	return true;
}

bool Module::Render::Renderer::Destruct()
{
	delete activeCamera;
	return renderer->Cleanup();
	delete renderer;
	editor->Destruct();
	delete editor;
	delete objects;
}

bool Module::Render::Renderer::CreateBuffer(Objects::Component::GraphicComponent* _component, LPCWSTR _texture_path)
{
	objects->insert_or_assign(_component->GetId().GetInstanceNumber(), _component );
	return renderer->CreateBuffers(_component->GetId().GetInstanceNumber(), _component->GetComponentName(), _component->GetTransformReference(), _component->GetMesh()->GetVertices(), _component->GetMesh()->GetVertexCount(), _component->GetMesh()->GetIndexes(), _component->GetMesh()->GetIndexCount(), _texture_path);
}

bool Module::Render::Renderer::MakeNewIndexAndVertexBuffer(Objects::Component::GraphicComponent* _component)
{
	return renderer->MakeNewIndexAndVertexBuffers(_component->GetId().GetInstanceNumber(), _component->GetComponentName(), _component->GetMesh()->GetVertices(), _component->GetMesh()->GetVertexCount(), _component->GetMesh()->GetIndexes(), _component->GetMesh()->GetIndexCount());
}

bool Module::Render::Renderer::MakeNewTextureBuffer(Objects::Component::GraphicComponent* _component, LPCWSTR _texture_path)
{
	return renderer->MakeNewTextureBuffers(_component->GetId().GetInstanceNumber(), _component->GetComponentName(), _texture_path);
}

Module::Render::Camera* Module::Render::Renderer::GetActiveCamera() const
{
	return activeCamera;
}

Core::Interface::IRender* Module::Render::Renderer::GetRenderer() 
{
	return renderer;
}

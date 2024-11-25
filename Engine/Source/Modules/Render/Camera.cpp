#include <algorithm>
#include "Camera.h"
#include "Modules/Display/Window.h"
#include "Engine.h"


Module::Render::Camera::Camera()
{
	const Display::Window* window = Engine::GetInstance()->GetModule<Display::Window>();
	width = static_cast<FLOAT>(window->GetWidth());
	height = static_cast<FLOAT>(window->GetHeight());
}

void Module::Render::Camera::SetAsActiveCamera()
{
	isActiveCamera = true;
}

Core::CoreType::Transform Module::Render::Camera::GetTransform() const
{
	return transform;
}

Core::CoreType::Transform* Module::Render::Camera::GetTransformReference() 
{
	return &transform;
}

DirectX::XMFLOAT4X4 Module::Render::Camera::GetProjectionMatrix() const
{
	DirectX::XMFLOAT4X4 projectionMatrix = DirectX::XMFLOAT4X4();

	const float h = 1.f / tanf(fov * 0.5f);

	projectionMatrix._11 = h / (width / height);
	projectionMatrix._22 = h;
	projectionMatrix._33 = farRender / (farRender - nearRender);
	projectionMatrix._34 = 1;
	projectionMatrix._43= -nearRender * farRender / (farRender - nearRender);

	return projectionMatrix;
}

DirectX::XMFLOAT4X4 Module::Render::Camera::GetViewMatrix() const
{
	// Récupérer la position de la caméra à partir de la transformation
	DirectX::XMFLOAT3 position = transform.position;

	// Créer une matrice de rotation à partir des angles currentAngleX et currentAngleY
	DirectX::XMFLOAT4X4 rotationMatrix;
	DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixRotationRollPitchYaw(currentAngleY, currentAngleX, 0.0f));

	// Créer une matrice de translation pour positionner la caméra
	DirectX::XMFLOAT4X4 translationMatrix;
	DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z));

	// Calculer la matrice de vue en combinant la rotation et la translation
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rotationMatrix), DirectX::XMLoadFloat4x4(&translationMatrix)));

	return viewMatrix;
}

float Module::Render::Camera::GetFov() const
{
	return fov;
}

float Module::Render::Camera::GetNear() const
{
	return nearRender;
}

float Module::Render::Camera::GetFar() const
{
	return farRender;
}


void Module::Render::Camera::SetFov(float _fov)
{
	fov = _fov;
}

void Module::Render::Camera::SetNear(const float _near)
{
	nearRender = _near;
}

void Module::Render::Camera::SetFar(const float _far)
{
	farRender = _far;
}

void Module::Render::Camera::SetTransform(const Core::CoreType::Transform _transform)
{
	transform = _transform;
}




void Module::Render::Camera::Rotate(float deltaX, float deltaY)
{
	// Convertir les mouvements de la souris en angles de rotation
	float angleX = deltaX * rotationSpeed;
	float angleY = deltaY * rotationSpeed;

	// Calculer les nouveaux angles de rotation de la caméra
	float newAngleX = currentAngleX + angleX;
	float newAngleY = currentAngleY + angleY;

	// Mettre à jour les angles de rotation de la caméra
	currentAngleX = newAngleX;
	currentAngleY = newAngleY;

	// Autres instructions de la fonction Rotate...
}


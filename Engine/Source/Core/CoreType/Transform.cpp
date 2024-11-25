#include "Transform.h"

Core::CoreType::Transform::Transform(Transform* _parent) 
{
    parent = _parent;
}

Core::CoreType::Transform::Transform(DirectX::XMFLOAT3 _position, DirectX::XMFLOAT3 _rotation, DirectX::XMFLOAT3 _scale)
{
    position = _position;
    rotation = _rotation;
    scale = _scale;
}

DirectX::XMFLOAT4X4 Core::CoreType::Transform::GetLocalMatrix() const
{
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f));
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    DirectX::XMMATRIX localMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    DirectX::XMFLOAT4X4 result;
    DirectX::XMStoreFloat4x4(&result, localMatrix);

    return result;
}

DirectX::XMFLOAT4X4 Core::CoreType::Transform::GetWorldMatrix() const
{
    DirectX::XMFLOAT4X4 worldMatrix;

    if (parent)
    {
        DirectX::XMFLOAT4X4 _local_matrix = GetLocalMatrix();
        DirectX::XMMATRIX localMatrix = XMLoadFloat4x4(&_local_matrix);
        DirectX::XMFLOAT4X4 _parent_world_matrix = parent->GetWorldMatrix();
        DirectX::XMMATRIX parentWorldMatrix = XMLoadFloat4x4(&_parent_world_matrix);
        DirectX::XMMATRIX resultMatrix = localMatrix * parentWorldMatrix;
        XMStoreFloat4x4(&worldMatrix, resultMatrix);
    }
    else
    {
        worldMatrix = GetLocalMatrix();
    }

    return worldMatrix;
}

void Core::CoreType::Transform::SetParent(Transform* _new_parent)
{
    parent = _new_parent;
}

void Core::CoreType::Transform::MoveForward(float distance)
{
    // Calculer l'angle de rotation en radians (ajout de PI pour inverser la direction)
    float angle = DirectX::XMConvertToRadians(rotation.y) + DirectX::XM_PI;

    // Calculer les composantes de direction inversées
    float directionX = -sinf(angle); // Inversion de sin
    float directionZ = -cosf(angle); // Inversion de cos

    // Créer un vecteur de direction normalisé
    DirectX::XMVECTOR direction = DirectX::XMVectorSet(directionX, 0.0f, directionZ, 0.0f);
    direction = DirectX::XMVector3Normalize(direction);

    // Calculer la translation en fonction de la direction et de la distance
    DirectX::XMVECTOR translation = DirectX::XMVectorScale(direction, distance);

    // Extraire les composantes de translation
    float translateX = DirectX::XMVectorGetX(translation);
    float translateY = DirectX::XMVectorGetY(translation);
    float translateZ = DirectX::XMVectorGetZ(translation);

    // Créer la matrice de translation
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translateX, translateY, translateZ);

    // Appliquer la translation à la position actuelle pour obtenir la nouvelle position
    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVector3Transform(currentPosition, translationMatrix);

    // Stocker la nouvelle position
    DirectX::XMStoreFloat3(&position, newPosition);
}


void Core::CoreType::Transform::MoveBackward(float distance)
{
    // Calculer l'angle de rotation en radians
    float angle = DirectX::XMConvertToRadians(rotation.y);

    // Calculer les composantes de direction inversées
    float directionX = -sinf(angle); // Inversion de sin
    float directionZ = -cosf(angle); // Inversion de cos

    // Créer un vecteur de direction normalisé
    DirectX::XMVECTOR direction = DirectX::XMVectorSet(directionX, 0.0f, directionZ, 0.0f);
    direction = DirectX::XMVector3Normalize(direction);

    // Calculer la translation en fonction de la direction et de la distance
    DirectX::XMVECTOR translation = DirectX::XMVectorScale(direction, distance);

    // Extraire les composantes de translation
    float translateX = DirectX::XMVectorGetX(translation);
    float translateY = DirectX::XMVectorGetY(translation);
    float translateZ = DirectX::XMVectorGetZ(translation);

    // Créer la matrice de translation
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translateX, translateY, translateZ);

    // Appliquer la translation à la position actuelle pour obtenir la nouvelle position
    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVector3Transform(currentPosition, translationMatrix);

    // Stocker la nouvelle position
    DirectX::XMStoreFloat3(&position, newPosition);
}


void Core::CoreType::Transform::MoveLeft(float distance)
{
    // Calculer l'angle de rotation en radians
    float angle = DirectX::XMConvertToRadians(rotation.y);

    // Calculer les composantes de direction inversées
    float directionX = -cos(angle);  // Inversion de cos
    float directionZ = sin(angle);   // Pas d'inversion de sin

    // Créer un vecteur de direction normalisé
    DirectX::XMVECTOR direction = DirectX::XMVectorSet(directionX, 0.0f, directionZ, 0.0f);
    direction = DirectX::XMVector3Normalize(direction);

    // Calculer la translation en fonction de la direction et de la distance
    DirectX::XMVECTOR displacement = DirectX::XMVectorScale(direction, distance);

    // Ajouter la translation à la position actuelle pour obtenir la nouvelle position
    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosition, displacement);

    // Stocker la nouvelle position
    DirectX::XMStoreFloat3(&position, newPosition);
}



void Core::CoreType::Transform::MoveRight(float distance)
{
    // Calculer l'angle de rotation en radians
    float angle = DirectX::XMConvertToRadians(rotation.y);

    // Calculer les composantes de direction inversées
    float directionX = cos(angle);   // Inversion de cos
    float directionZ = -sin(angle);  // Inversion de sin

    // Créer un vecteur de direction normalisé
    DirectX::XMVECTOR direction = DirectX::XMVectorSet(directionX, 0.0f, directionZ, 0.0f);
    direction = DirectX::XMVector3Normalize(direction);

    // Calculer la translation en fonction de la direction et de la distance
    DirectX::XMVECTOR displacement = DirectX::XMVectorScale(direction, distance);

    // Ajouter la translation à la position actuelle pour obtenir la nouvelle position
    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosition, displacement);

    // Stocker la nouvelle position
    DirectX::XMStoreFloat3(&position, newPosition);
}


void Core::CoreType::Transform::MoveUp(float distance)
{
    // Inverser le signe de la composante Y du vecteur de déplacement
    DirectX::XMVECTOR displacement = DirectX::XMVectorSet(0.0f, distance, 0.0f, 0.0f); // Inversion du signe

    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosition, displacement);

    DirectX::XMStoreFloat3(&position, newPosition);
}


void Core::CoreType::Transform::MoveDown(float distance)
{
    // Inverser le signe de la composante Y du vecteur de déplacement
    DirectX::XMVECTOR displacement = DirectX::XMVectorSet(0.0f, -distance, 0.0f, 0.0f); // Inversion du signe

    DirectX::XMVECTOR currentPosition = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosition, displacement);

    DirectX::XMStoreFloat3(&position, newPosition);
}

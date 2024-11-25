/**
 * @file GameObject.h
 * @brief Defines the game object class.
 */

#pragma once

#include <string>
#include <vector>
#include <d3d12.h>
#include "Core/Interface/IComponent.h"
#include "Core/CoreType/Transform.h"
#include "Core/CoreType/Id.h"
#include "Core/CoreType/Mesh.h"
#include "Core/Interface/IScene.h"

namespace Core {

    namespace Interface {

        class IComponent;
        class IScene;

    }
}

namespace Objects
{

    /**
     * @class GameObject
     * @brief Represents a game object in the engine.
     */
    class GameObject
    {
    public:
        /**
         * @brief Constructs a game object with the specified scene.
         * @param scene Pointer to the scene that the object belongs to.
         */
        GameObject(Core::Interface::IScene* scene);

        /**
         * @brief Constructs a game object by copying another game object.
         * @param scene Pointer to the scene that the object belongs to.
         * @param other Pointer to the other game object to copy.
         */
        GameObject(Core::Interface::IScene* scene, Objects::GameObject* other);

        /**
         * @brief Default destructor.
         */
        ~GameObject();

        /**
         * @brief Gets the name of the game object.
         * @return The name of the game object.
         */
        std::string GetName();

        /**
         * @brief Gets the transform of the game object.
         * @return The transform of the game object.
         */
        Core::CoreType::Transform GetTransform() const;

        /**
         * @brief Gets the reference to the transform of the game object.
         * @return Pointer to the transform of the game object.
         */
        Core::CoreType::Transform* GetTransformReference();

        /**
         * @brief Gets the ID of the game object.
         * @return The ID of the game object.
         */
        Core::CoreType::Id GetId() const;

        /**
         * @brief Gets the parent of the game object.
         * @return Pointer to the parent game object.
         */
        GameObject* GetParent() const;

        /**
         * @brief Gets the list of child game objects.
         * @return Pointer to the vector containing child game objects.
         */
        std::vector<GameObject*>* GetChild();

        /**
         * @brief Gets the texture path of the game object.
         * @return The texture path of the game object.
         */
        std::string GetTexturePath() const;

        /**
         * @brief Sets the transform of the game object.
         * @param _transform New transform for the game object.
         */
        void SetTransform(Core::CoreType::Transform _transform);

        /**
         * @brief Sets the color of the game object.
         * @param _color New color for the game object.
         */
        void SetColor(DirectX::XMVECTORF32 _color);

        /**
         * @brief Sets the color of the game object with custom color components.
         * @param _r Red component of the color.
         * @param _g Green component of the color.
         * @param _b Blue component of the color.
         * @param _a Alpha component of the color.
         */
        void SetColor(float _r, float _g, float _b, float _a);

        /**
         * @brief Sets the parent of the game object.
         * @param _parent Pointer to the parent game object.
         */
        void SetParent(GameObject* _parent);

        /**
         * @brief Sets the name of the game object.
         * @param _name New name for the game object.
         */
        void SetName(std::string _name);

        /**
         * @brief Sets the texture path of the game object.
         * @param _texture_path New texture path for the game object.
         */
        void SetTexturePath(std::string _texture_path);

        /**
         * @brief Creates and adds a new component to the game object.
         * @tparam T Type of the component to create.
         * @tparam PARAMS Parameter types for the component constructor.
         * @param PARAMS Constructor parameters for the component.
         * @return Pointer to the newly created component.
         */
        template <typename T, typename... PARAMS, class U = typename std::enable_if<std::is_base_of<Core::Interface::IComponent, T>::value, T>::type>
        T* CreateComponent(PARAMS...);

        /**
         * @brief Gets the component of the specified type attached to the game object.
         * @tparam T Type of the component to get.
         * @return Pointer to the component of the specified type, or nullptr if not found.
         */
        template<typename T>
        T* GetComponent();

        /**
         * @brief Adds a component to the game object.
         * @param _component Pointer to the component to add.
         */
        void AddComponent(Core::Interface::IComponent* _component);

        /**
         * @brief Removes a component from the game object.
         * @param _component Pointer to the component to remove.
         */
        void RemoveComponent(Core::Interface::IComponent* _component);

        /**
         * @brief Updates the game object and its components.
         */
        void Update();

        /**
         * @brief Checks if the game object has a parent.
         * @return True if the game object has a parent, false otherwise.
         */
        bool HaveParent();

        /**
         * @brief Checks if the game object has child game objects.
         * @return True if the game object has child game objects, false otherwise.
         */
        bool HaveChilds();

        /**
         * @brief Creates a mesh for the game object using a predefined primitive mesh type.
         * @param _mesh_type Pre-defined primitive mesh type.
         */
        void MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh);

        /**
         * @brief Creates a mesh for the game object using a predefined primitive mesh type and color.
         * @param _mesh_type Pre-defined primitive mesh type.
         * @param _color Color of the mesh.
         */
        void MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, DirectX::XMVECTORF32 _color);

        /**
         * @brief Creates a mesh for the game object using a predefined primitive mesh type and custom color components.
         * @param _mesh_type Pre-defined primitive mesh type.
         * @param _r Red component of the color.
         * @param _g Green component of the color.
         * @param _b Blue component of the color.
         * @param _a Alpha component of the color.
         */
        void MakeMesh(Core::CoreType::PrimitivesMesh::PrimitivesMeshType _mesh_type, float _r, float _g, float _b, float _a);

        /**
         * @brief Creates a mesh for the game object using a mesh file.
         * @param _mesh_path Path to the mesh file.
         */
        void MakeMesh(const std::string _mesh_path);

        /**
         * @brief Creates a mesh for the game object using a mesh file and color.
         * @param _mesh_path Path to the mesh file.
         * @param _color Color of the mesh.
         */
        void MakeMesh(const std::string _mesh_path, DirectX::XMVECTORF32 _color);

        /**
         * @brief Creates a mesh for the game object using a mesh file and custom color components.
         * @param _mesh_path Path to the mesh file.
         * @param _r Red component of the color.
         * @param _g Green component of the color.
         * @param _b Blue component of the color.
         * @param _a Alpha component of the color.
         */
        void MakeMesh(const std::string _mesh_path, float _r, float _g, float _b, float _a);

        /**
         * @brief Creates a texture for the game object using a texture file.
         * @param _texture_path Path to the texture file.
         */
        void MakeTexture(std::string _texture_path);

        /**
         * @brief Flag indicating if the game object is visible.
         */
        bool isVisible = true;

    private:
        /**
         * @brief Adds a child game object to the current game object.
         * @param _child Pointer to the child game object to add.
         */
        void AddChild(GameObject* _child);

        /**
         * @brief Removes a child game object from the current game object.
         * @param _child Pointer to the child game object to remove.
         */
        void RemoveChild(GameObject* _child);

        std::string name = ""; /**< Name of the game object. */
        Core::CoreType::Id id = Core::CoreType::Id(Core::CoreType::EObjectSubtype::DefaultGameObject); /**< ID of the game object. */
        Core::CoreType::Transform transform; /**< Transform of the game object. */
        GameObject* parent = nullptr; /**< Pointer to the parent game object. */
        std::vector<GameObject*>* childs = new std::vector<GameObject*>(); /**< Vector containing child game objects. */
        std::vector<Core::Interface::IComponent*>* components = new std::vector<Core::Interface::IComponent*>(); /**< Vector containing attached components. */
        std::string texture_path = "EngineTest\\Assets\\Texture\\default.png"; /**< Texture path of the game object. */

    };
}

#include "Objects/GameObject.inl"
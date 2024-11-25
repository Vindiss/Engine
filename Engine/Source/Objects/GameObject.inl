#pragma once

template <typename T, typename... PARAMS, class U >
T* Objects::GameObject::CreateComponent(PARAMS... params)
{
	U* component = new U(params...);
	Core::Interface::IComponent* component_cast = static_cast<Core::Interface::IComponent*>(component);

	if (component_cast)
	{
		component_cast->SetAttachedGameObject(this);

		component_cast->Init();

		components->push_back(component_cast);
		return component;
	}

	return nullptr;
}

template <typename T>
T* Objects::GameObject::GetComponent()
{
	for (Core::Interface::IComponent* component : *components)
	{
		if (typeid(*component) == typeid(T))
			return static_cast<T*>(component);
	}

	return nullptr;
}
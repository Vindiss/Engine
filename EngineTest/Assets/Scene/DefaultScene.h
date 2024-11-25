#pragma once
#include "Core/Interface/IScene.h"



class DefaultScene : public Core::Interface::IScene
{
public:
	DefaultScene() = default;
	~DefaultScene() = default;

	bool Init() override 
	{
		SetName("DefaultScene");
		SetFilePath("EngineTest/Assets/Scene/DefaultScene.h");
		return true;
	}

	bool Start() override
	{
		return true;
	}

	bool Update() override
	{
		return true;
	}

	bool Stop() override
	{

		return true;
	}

	bool Destruct() override
	{

		return true;
	}

private:

};

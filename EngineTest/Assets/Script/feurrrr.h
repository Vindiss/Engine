#pragma once
#include "Scripts/Script.h"



class feurrrr : public Scripts::Script
{
public:
	bool Init() override 
	{
		SetScriptName("feurrrr");
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

#pragma once
#include "Scripts/Script.h"



class Test : public Scripts::Script
{
public:

    void Init() override 
    {
        SetScriptName("Test");
    }

    void Start() override
    {
        OutputDebugString(L"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
    }

    void Update() override
    {
        
    }

    void Destruct() override 
    {
        
    }
private:

};
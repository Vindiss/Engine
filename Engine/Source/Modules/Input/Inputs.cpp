#include "Inputs.h"
#include <Windows.h>
#include <iostream>
#include "Engine.h"
#include "system_error"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

bool Module::Input::Inputs::Init() 
{
    return true;
}

bool Module::Input::Inputs::Start()
{
    return true;
}

bool Module::Input::Inputs::IsKeyDown(KeyboardKey key)
{
    unsigned windowsKey = keyToWindowsMap[key];
    return GetAsyncKeyState(windowsKey) & 0x8000;
}

bool Module::Input::Inputs::IsKeyUp(KeyboardKey key)
{
    unsigned windowsKey = keyToWindowsMap[key];
    return !(GetAsyncKeyState(windowsKey) & 0x8000);
}

bool Module::Input::Inputs::IsKeyPressed(KeyboardKey key)
{
    return IsKeyDown(key) && !m_prevKeyStates[key];
}

bool Module::Input::Inputs::IsKeyReleased(KeyboardKey key)
{
    return IsKeyUp(key) && m_prevKeyStates[key];
}

bool Module::Input::Inputs::Update()
{
    // Mettre à jour les états des touches du clavier
    for (const auto& pair : m_keyStates) {
        m_prevKeyStates[pair.first] = pair.second;
    }

    // Met à jour les états actuels des touches
    for (auto& pair : m_keyStates) {
        pair.second = IsKeyDown(pair.first);
    }




    POINT point;
    GetCursorPos(&point);

    mouseDelta[0] = point.x - mousePos[0];
    mouseDelta[1] = point.y - mousePos[1];
    mousePos[0] = point.x;
    mousePos[1] = point.y;


    leftButtonDown = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
    rightButtonDown = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;

    return true;
}

bool Module::Input::Inputs::Destruct()
{
    return true;
}
 
/**
 * @file Inputs.h
 * @brief Defines the Inputs module for handling user input.
 */

#pragma once
#pragma once
#include <Windows.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include "Core/Interface/IModule.h"

namespace Module
{
    namespace Input
    {

        /**
         * @enum KeyboardKey
         * @brief Enumerates all possible keyboard keys.
         */
        enum class KeyboardKey
        {
            Unknown = -1, /**< Unknown key */
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, /**< Alphabetic keys */
            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, /**< Number keys */
            Escape, LControl, LShift, LAlt, RControl, RShift, RAlt, Menu, /**< Special keys */
            LBracket, RBracket, SemiColon, Comma, Period, Quote, Slash, BackSlash, Tilde, /**< Punctuation keys */
            Space, Return, BackSpace, Tab, PageUp, PageDown, End, Home, Insert, Delete, /**< Control keys */
            Add, Subtract, Multiply, Divide, /**< Arithmetic keys */
            Left, Right, Up, Down, /**< Arrow keys */
            Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, /**< Numpad keys */
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, /**< Function keys */
            Pause /**< Pause key */
        };

        /**
         * @class Inputs
         * @brief Represents the module for handling user input.
         */
        class Inputs : public Core::Interface::IModule
        {
        public:
            /**
             * @brief Default constructor.
             */
            Inputs()
            {
			keyToWindowsMap = {
					{KeyboardKey::Unknown, -1},
					{KeyboardKey::A, 'A'},
					{KeyboardKey::B, 'B'},
					{KeyboardKey::C, 'C'},
					{KeyboardKey::D, 'D'},
					{KeyboardKey::E, 'E'},
					{KeyboardKey::F, 'F'},
					{KeyboardKey::G, 'G'},
					{KeyboardKey::H, 'H'},
					{KeyboardKey::I, 'I'},
					{KeyboardKey::J, 'J'},
					{KeyboardKey::K, 'K'},
					{KeyboardKey::L, 'L'},
					{KeyboardKey::M, 'M'},
					{KeyboardKey::N, 'N'},
					{KeyboardKey::O, 'O'},
					{KeyboardKey::P, 'P'},
					{KeyboardKey::Q, 'Q'},
					{KeyboardKey::R, 'R'},
					{KeyboardKey::S, 'S'},
					{KeyboardKey::T, 'T'},
					{KeyboardKey::U, 'U'},
					{KeyboardKey::V, 'V'},
					{KeyboardKey::W, 'W'},
					{KeyboardKey::X, 'X'},
					{KeyboardKey::Y, 'Y'},
					{KeyboardKey::Z, 'Z'},
					{KeyboardKey::Num0, '0'},
					{KeyboardKey::Num1, '1'},
					{KeyboardKey::Num2, '2'},
					{KeyboardKey::Num3, '3'},
					{KeyboardKey::Num4, '4'},
					{KeyboardKey::Num5, '5'},
					{KeyboardKey::Num6, '6'},
					{KeyboardKey::Num7, '7'},
					{KeyboardKey::Num8, '8'},
					{KeyboardKey::Num9, '9'},
					{KeyboardKey::Escape, VK_ESCAPE},
					{KeyboardKey::LControl, VK_LCONTROL},
					{KeyboardKey::LShift, VK_LSHIFT},
					{KeyboardKey::LAlt, VK_LMENU},
					{KeyboardKey::RControl, VK_RCONTROL},
					{KeyboardKey::RShift, VK_RSHIFT},
					{KeyboardKey::RAlt, VK_RMENU},
					{KeyboardKey::Menu, VK_MENU},
					{KeyboardKey::LBracket, VK_OEM_4},
					{KeyboardKey::RBracket, VK_OEM_6},
					{KeyboardKey::SemiColon, VK_OEM_1},
					{KeyboardKey::Comma, VK_OEM_COMMA},
					{KeyboardKey::Period, VK_OEM_PERIOD},
					{KeyboardKey::Quote, VK_OEM_7},
					{KeyboardKey::Slash, VK_OEM_2},
					{KeyboardKey::BackSlash, VK_OEM_5},
					{KeyboardKey::Tilde, VK_OEM_3},
					{KeyboardKey::Space, VK_SPACE},
					{KeyboardKey::Return, VK_RETURN},
					{KeyboardKey::BackSpace, VK_BACK},
					{KeyboardKey::Tab, VK_TAB},
					{KeyboardKey::PageUp, VK_PRIOR},
					{KeyboardKey::PageDown, VK_NEXT},
					{KeyboardKey::End, VK_END},
					{KeyboardKey::Home, VK_HOME},
					{KeyboardKey::Insert, VK_INSERT},
					{KeyboardKey::Delete, VK_DELETE},
					{KeyboardKey::Add, VK_ADD},
					{KeyboardKey::Subtract, VK_SUBTRACT},
					{KeyboardKey::Multiply, VK_MULTIPLY},
					{KeyboardKey::Divide, VK_DIVIDE},
					{KeyboardKey::Left, VK_LEFT},
					{KeyboardKey::Right, VK_RIGHT},
					{KeyboardKey::Up, VK_UP},
					{KeyboardKey::Down, VK_DOWN},
					{KeyboardKey::Numpad0, VK_NUMPAD0},
					{KeyboardKey::Numpad1, VK_NUMPAD1},
					{KeyboardKey::Numpad2, VK_NUMPAD2},
					{KeyboardKey::Numpad3, VK_NUMPAD3},
					{KeyboardKey::Numpad4, VK_NUMPAD4},
					{KeyboardKey::Numpad5, VK_NUMPAD5},
					{KeyboardKey::Numpad6, VK_NUMPAD6},
					{KeyboardKey::Numpad7, VK_NUMPAD7},
					{KeyboardKey::Numpad8, VK_NUMPAD8},
					{KeyboardKey::Numpad9, VK_NUMPAD9},
					{KeyboardKey::F1, VK_F1},
					{KeyboardKey::F2, VK_F2},
					{KeyboardKey::F3, VK_F3},
					{KeyboardKey::F4, VK_F4},
					{KeyboardKey::F5, VK_F5},
					{KeyboardKey::F6, VK_F6},
					{KeyboardKey::F7, VK_F7},
					{KeyboardKey::F8, VK_F8},
					{KeyboardKey::F9, VK_F9},
					{KeyboardKey::F10, VK_F10},
					{KeyboardKey::F11, VK_F11},
					{KeyboardKey::F12, VK_F12},
					{KeyboardKey::F13, VK_F13},
					{KeyboardKey::F14, VK_F14},
					{KeyboardKey::F15, VK_F15},
					{KeyboardKey::Pause, VK_PAUSE}
			};
		};

            /**
             * @brief Checks if the specified key is currently pressed.
             * @param key The key to check.
             * @return True if the key is pressed, false otherwise.
             */
            bool IsKeyDown(KeyboardKey key);

            /**
             * @brief Checks if the specified key is currently released.
             * @param key The key to check.
             * @return True if the key is released, false otherwise.
             */
            bool IsKeyUp(KeyboardKey key);

            /**
             * @brief Checks if the specified key was pressed since the last update.
             * @param key The key to check.
             * @return True if the key was pressed, false otherwise.
             */
            bool IsKeyPressed(KeyboardKey key);

            /**
             * @brief Checks if the specified key was released since the last update.
             * @param key The key to check.
             * @return True if the key was released, false otherwise.
             */
            bool IsKeyReleased(KeyboardKey key);

            /**
             * @brief Checks if the left mouse button is currently pressed.
             * @return True if the left mouse button is pressed, false otherwise.
             */
			      bool isLeftButtonDown() const { return leftButtonDown; }

            /**
             * @brief Checks if the right mouse button is currently pressed.
             * @return True if the right mouse button is pressed, false otherwise.
             */
            bool isRightButtonDown() const { return rightButtonDown; }

            /**
             * @brief Gets the current position of the mouse cursor.
             * @return An array containing the X and Y coordinates of the mouse cursor.
             */
            const int* GetMousePos() const { return mousePos; }

            /**
             * @brief Gets the delta movement of the mouse cursor since the last update.
             * @return An array containing the X and Y delta movement of the mouse cursor.
             */
            const int* GetMouseDelta() const { return mouseDelta; }

            /**
             * @brief Initializes the Inputs module.
             * @return True if initialization is successful, false otherwise.
             */
            bool Init() override;

            /**
             * @brief Starts the Inputs module.
             * @return True if starting is successful, false otherwise.
             */
            bool Start() override;

            /**
             * @brief Updates the Inputs module.
             * @return True if updating is successful, false otherwise.
             */
            bool Update() override;

            /**
             * @brief Destructs the Inputs module.
             * @return True if destruction is successful, false otherwise.
             */
            bool Destruct() override;

        private:

            std::unordered_map<KeyboardKey, bool> m_keyStates; /**< Map of current key states */
            std::unordered_map<KeyboardKey, bool> m_prevKeyStates; /**< Map of previous key states */

            std::unordered_map<KeyboardKey, unsigned> keyToWindowsMap; /**< Mapping of KeyboardKey enum to Windows virtual key codes */

            bool leftButtonDown, rightButtonDown; /**< Flag indicating if the left mouse button is pressed or if the right mouse button is pressed */
            int mousePos[2]; /**< Array containing the X and Y coordinates of the mouse cursor */
            int mouseDelta[2]; /**< Array containing the X and Y delta movement of the mouse cursor */
        };
    }
}

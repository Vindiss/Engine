#pragma once
#include <windows.h>
#include "Core/Interface/IModule.h"

namespace Module {
	
	namespace Display {

		class Window : public Core::Interface::IModule
		{
		public:

			/*
				brief Default constructor
			*/
			Window() = default;

			/*
				brief Destructor
			*/
			~Window();

			bool Init() override;

			bool Start() override;
			
			bool Update() override;

			bool Destruct() override;

			/*
				brief Callback function for windows messages
				param _window_h Handle of the window
				param _windo_msg Message of the window
				param _window_param Message Parameter
				param _long_param Message Parameter
				return Is message processed
			*/
			static LRESULT CALLBACK WndProc(HWND _window_h, UINT _window_msg, WPARAM _window_param, LPARAM _long_param);

			/*
				brief Get and process messages from window
			*/
			void GetMessages();

			/*	brief Is window is fullscreen
				return Is window is fullscreen
			*/
			bool IsFullScreen() const { return fullscreen; }

			/*
				brief Return width of the window
				return Width of the window
			*/
			int GetWidth() const { return width; }

			/*
				brief Return height of the window
				return Height of the window
			*/
			int GetHeight() const { return height; }

			HWND* GetHandle() { return &windowHandler; }


		private:

			/*
				brief Create a window class
				return Is window prepared
			*/
			bool PrepareWindow(const int _width, const int _height);

			/*
				brief Create and display the window
				return Is window created
			*/
			bool MakeWindow();

			HINSTANCE hInstance = nullptr; //brief Handle of the instance

			HWND windowHandler = nullptr; //brief Handler of the window 

			WNDCLASSEX windowClass{}; //brief Class of the window

			const wchar_t* windowName = L"WindowName"; //brief Name of the window

			const wchar_t* windowTitle = L"WindowTitle"; //brief Title of the window

			int width = 1920; //brief Width of the window

			int height = 1080; //brief Height of the window

			bool fullscreen = false; //brief Fullscreen state of the window


		};
	}
}
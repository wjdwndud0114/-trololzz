// Author: wjdwndud0114, Kevin Jeong
// Crappy code and crappy commenting written a long time ago :/
#include "Start.h"

int Width = 800;
int Height = 600;

const MARGINS Margin = { 0, 0, Width, Height };

char lWindowName[256] = "Trololzz v1.4";
HWND hWnd;

char tWindowName[256] = "Counter-Strike: Global Offensive"; /* tWindowName ? Target Window Name */
HWND tWnd;
RECT tSize;

Main stuff;

MSG Message;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hSecInstance, LPSTR nCmdLine, INT nCmdShow){
	PlaySound(TEXT("Sound/Init.wav"), NULL, SND_FILENAME | SND_ASYNC);

	//DX
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = lWindowName;
	wClass.lpszMenuName = lWindowName;
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass))
		exit(1);

	tWnd = FindWindow(0, tWindowName);
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize);
		Width = tSize.right - tSize.left;
		Height = tSize.bottom - tSize.top;
		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, 1, 1, Width, Height, 0, 0, 0, 0);
		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow(hWnd, SW_SHOW);
	}

	stuff.DirectXInit(hWnd);//DX
	
	while(true){
		
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
		if (GetAsyncKeyState(VK_F4) & 1){
			return 0;
		}
		if (tWnd == GetForegroundWindow()){
			
			if (GetAsyncKeyState(VK_NUMPAD1) & 1) //number 1 = esp
			{
				stuff.eE = !stuff.eE;
				if (stuff.eE == false){ PlaySound(TEXT("Sound/eD.wav"), NULL, SND_FILENAME | SND_ASYNC); }
				else if (stuff.eE == true){ PlaySound(TEXT("Sound/eE.wav"), NULL, SND_FILENAME | SND_ASYNC); }
			}

			if (GetAsyncKeyState(VK_NUMPAD2) & 1) //number 2 = bone esp
			{
				stuff.bE = !stuff.bE;
				if (stuff.bE == false){ PlaySound(TEXT("Sound/bD.wav"), NULL, SND_FILENAME | SND_ASYNC); }
				else if (stuff.bE == true){ PlaySound(TEXT("Sound/bE.wav"), NULL, SND_FILENAME | SND_ASYNC); }
			}

			if (GetAsyncKeyState(VK_XBUTTON1) & 1 || GetAsyncKeyState(VK_NUMPAD3) & 1) //backward mouse button and number 3 = trigger
			{
				stuff.tE = !stuff.tE;
				if (stuff.tE == false){ PlaySound(TEXT("Sound/tD.wav"), NULL, SND_FILENAME | SND_ASYNC); }
				else if (stuff.tE == true){ PlaySound(TEXT("Sound/tE.wav"), NULL, SND_FILENAME | SND_ASYNC); }
			}

			if (GetAsyncKeyState(VK_NUMPAD4) & 1) //number 4 = crosshair
			{
				stuff.cE = !stuff.cE;
				if (stuff.cE == false){ PlaySound(TEXT("Sound/cD.wav"), NULL, SND_FILENAME | SND_ASYNC); }
				else if (stuff.cE == true){ PlaySound(TEXT("Sound/cE.wav"), NULL, SND_FILENAME | SND_ASYNC); }
			}

			if (GetAsyncKeyState(VK_NUMPAD5) & 1 || GetAsyncKeyState(VK_MBUTTON) & 1) //number 5 = recoil reducer and mousewheel click
			{
				stuff.rE = !stuff.rE;
				if (stuff.rE == false){ PlaySound(TEXT("Sound/rD.wav"), NULL, SND_FILENAME | SND_ASYNC); }
				else if (stuff.rE == true){ PlaySound(TEXT("Sound/rE.wav"), NULL, SND_FILENAME | SND_ASYNC); }
			}

			if (GetAsyncKeyState(VK_ADD) & 1) //+ = increase
			{
				stuff.MaxAimbot += 5;
			}

			if (GetAsyncKeyState(VK_SUBTRACT) & 1) //- = decrease
			{
				stuff.MaxAimbot -= 5;
			}
		}
		Sleep(1);
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		stuff.Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void SetWindowToTarget(){
	while (true)
	{
		tWnd = FindWindow(0, tWindowName);
		if (tWnd)
		{
			GetWindowRect(tWnd, &tSize);
			Width = tSize.right - tSize.left;
			Height = tSize.bottom - tSize.top;
			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				tSize.top += 23;
				Height -= 23;
			}
			MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
		}
		else
		{
			char ErrorMsg[125];
			sprintf(ErrorMsg, "Make sure %s is running!", tWindowName);
			MessageBox(0, ErrorMsg, "Error - Cannot find the game!", MB_OK | MB_ICONERROR);
			exit(1);
		}
		Sleep(100);
	}
}

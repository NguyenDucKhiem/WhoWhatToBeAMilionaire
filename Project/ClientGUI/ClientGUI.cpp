// WSAAsyncSelectServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClientGUI.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#pragma warning(disable: 4996)
#pragma comment(lib, "Ws2_32.lib")

#define WM_SOCKET WM_USER + 1
#define SERVER_PORT 5500
#define MAX_CLIENT 1024
#define BUFF_SIZE 2048

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	windowProc(HWND, UINT, WPARAM, LPARAM);

SOCKET client[MAX_CLIENT];
SOCKET listenSock;
sockaddr_in serverAddr;

unsigned __stdcall workerThread(LPVOID lpParameter) {
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		return 0;
	}

	//Construct socket	
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//requests Windows message-based notification of network events for listenSock


	//Bind address to socket

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	unsigned long ul = 0;
	ioctlsocket(listenSock, FIONBIO, (unsigned long *)&ul);
	int ret = connect(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	int error = GetLastError();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_beginthreadex(0, 0, workerThread, NULL, 0, 0);
	MSG msg;
	HWND serverWindow;

	//Registering the Window Class
	MyRegisterClass(hInstance);

	//Create the window
	if ((serverWindow = InitInstance(hInstance, nCmdShow)) == NULL)
		return FALSE;

	//Initiate WinSock
	
	
	WSAAsyncSelect(listenSock, serverWindow, WM_SOCKET,FD_CONNECT | FD_CLOSE | FD_READ);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENTGUI));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	int i;
	for (i = 0; i <MAX_CLIENT; i++)
		client[i] = 0;
	hWnd = CreateWindow(L"WindowClass", L"WSAAsyncSelect TCP Server", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_SOCKET	- process the events on the sockets
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET connSock;
	sockaddr_in clientAddr;
	int ret, clientAddrLen = sizeof(clientAddr), i;
	char rcvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];

	switch (message) {
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam)) {
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam) {
					closesocket(client[i]);
					client[i] = 0;
					continue;
				}
		}

		int ret = WSAGETSELECTEVENT(lParam);

		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
		{
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam)
					break;

			ret = recv(client[i], rcvBuff, BUFF_SIZE, 0);
			if (ret > 0) {
				//echo to client
				memcpy(sendBuff, rcvBuff, ret);
				send(client[i], sendBuff, ret, 0);
			}
		}
		break;

		case FD_CLOSE:
		{
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam) {
					closesocket(client[i]);
					client[i] = 0;
					break;
				}
		}
		break;
		}
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		shutdown(listenSock, SD_BOTH);
		closesocket(listenSock);
		WSACleanup();
		return 0;
	}
	break;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		shutdown(listenSock, SD_BOTH);
		closesocket(listenSock);
		WSACleanup();
		return 0;
	}
	break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
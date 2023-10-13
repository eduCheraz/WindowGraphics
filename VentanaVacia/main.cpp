#include <Windows.h>

bool FULL_SCREEN = true;
bool isShutdown = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	HWND hWnd = NULL;
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings; // Que es un DEVMODE
	int posX, posY;
	HINSTANCE mhInstance = GetModuleHandle(NULL);//Checar que es esto ??
	int screenWidth = 0, screenHeight = 0;
	MSG msg;
	bool done, result;

	LPCWSTR applicationName = L"Motor Grafico";

	//Configurar un ventana
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	//Resolucion de la pantalla
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);


	if (FULL_SCREEN) {
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else {
		//Resolucion por defecto
		screenWidth = 800;
		screenHeight = 600;

		// Posicionar en centro de la pantalla
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Crear el manejador de la ventana
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		return false;
	}

	//Mostrar en primer plano y hacer focus a la pantalla
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	// Hide the mouse cursor.
	ShowCursor(false);


	//Bucle de eventos
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			//Realiza grafico y checa si no presionas ESCAPE
			if (isShutdown)
			{
				done = true;
			}
		}

	}


	//Shutdwon apagar pantalla
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(hWnd);
	hWnd = NULL;

	// Remove the application instance.
	UnregisterClass(applicationName, hInstance);
	hInstance = NULL;

	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			if ((unsigned int)wparam == VK_ESCAPE) {
				isShutdown = true;
			}
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}
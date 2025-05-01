// BotGui.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "BotGui.h"
#include "curl/curl.h"
#include "string.h"
#include "stdio.h"
#define MAX_LOADSTRING 100

char* TradeHistory = (char*)malloc(980 * sizeof(char));
double tickers = 0;

/*char* curlASite(char* url, HWND* hwnd) {
    char error[] = "Oh no theres an error\n";
    CURL* curl = curl_easy_init();
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    res = curl_easy_perform(curl);

    if (!res) {
        return error;
    }
   
}*/

char* getlinesfromfile() {
    FILE* file;
    errno_t err = fopen_s(&file, "TradeHistory.txt", "r");
    if (err) {
        return NULL;
    }
   
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize * sizeof(char));
   size_t buffSize = fread(buffer, 1, fileSize, file);
   buffer[buffSize] = '\0';
   
 
  
   const char* ptr = buffer;

   while (*ptr != '\0') {
       if (strncmp(ptr, "Ticker", 6) == 0) {
           tickers++;
           ptr += 7;
       }
       else {
           ptr++;
       }

   }
  
   
    return buffer;
}


    


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

 

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOTGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOTGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOTGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOTGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

wchar_t boxText[] = L"hello world";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;

        // Create a brush with light blue color
        HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230)); // Light blue RGB code
        RECT rect;
        GetClientRect(hWnd, &rect); // Get the window dimensions

        // Fill the background with the brush
        FillRect(hdc, &rect, hBrush);

        DeleteObject(hBrush); // Delete the brush to avoid memory leaks
        return 1; // Indicate that the background has been erased
    }

    case WM_CREATE:
    {
        TradeHistory = getlinesfromfile();
        if (TradeHistory == NULL) {
            MessageBoxA(hWnd, "Error: file doesn't exist", "Error", MB_OK | MB_ICONERROR);
            return -1;  // Return -1 to prevent further initialization if necessary
        }

        HWND hEdit = CreateWindowEx(
            0,                           // Extended styles
            L"EDIT",                     // Class name
            L"",                         // Initial text
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            15,                          // X position
            15,                          // Y position
            300,                         // Width
            300,                         // Height
            hWnd,                        // Parent window
            NULL,                        // No menu
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL                         // No special data
        );

        if (hEdit) {
            // Set the content of the text box
            SetWindowTextA(hEdit, TradeHistory);
        }
        // Process TradeHistory or initialize GUI components with its data here

        HWND hticker = CreateWindowEx(
            0,                           // Extended styles
            L"EDIT",                     // Class name
            L"",                         // Initial text
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            750,                          // X position
            360,                          // Y position
            50,                         // Width
            50,                         // Height
            hWnd,                        // Parent window
            NULL,                        // No menu
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL                         // No special data
        );

        if (hticker) {
            // Set the content of the text box
            char* buffer = (char*)malloc(50 * sizeof(char));
            snprintf(buffer, 49, "%f", tickers);
            SetWindowTextA(hticker, buffer);
        }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Define a larger font
        HFONT hFont = CreateFont(
            48,               // Height of font (e.g., 48 pixels)
            0,                // Width of font (0 = default/proportional width)
            0,                // Escapement angle
            0,                // Orientation angle
            FW_BOLD,          // Font weight (FW_NORMAL or FW_BOLD)
            FALSE,            // Italic
            TRUE,            // Underline
            FALSE,            // Strikeout
            DEFAULT_CHARSET,  // Character set
            OUT_DEFAULT_PRECIS, // Output precision
            CLIP_DEFAULT_PRECIS, // Clipping precision
            DEFAULT_QUALITY,  // Output quality
            DEFAULT_PITCH | FF_SWISS, // Pitch and family
            L"Arial"           // Font face name
        );

        // Select the new font into the device context
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        // Define a rectangle in the bottom-left corner
        RECT textBoxRect = { 10, clientRect.bottom - 200, clientRect.right - 20, clientRect.bottom };
        RECT tickerRect = { 500, clientRect.bottom - 150, clientRect.right - 100, clientRect.bottom - 100 };
        RECT logo = { 550, clientRect.bottom - 400, clientRect.right - 150, clientRect.bottom - 350 };
        // Fill the rectangle background
        HBRUSH hLightBlueBrush = CreateSolidBrush(RGB(173, 216, 230));
        HBRUSH hlogobrush = CreateSolidBrush(RGB(173, 200, 400));
        FillRect(hdc, &textBoxRect, hLightBlueBrush);
        FillRect(hdc, &tickerRect, hLightBlueBrush);
        FillRect(hdc, &logo, hlogobrush);
        // Set background and draw the text
        SetBkMode(hdc, TRANSPARENT);
        //RECT textBoxRect = { 10, 10, 400, 100 }; // Adjust as needed
        DrawTextA(hdc, "Trade History:", -1, &textBoxRect, DT_LEFT | DT_VCENTER);
        DrawTextA(hdc, "Trades Hit:", -1, &tickerRect, DT_LEFT | DT_VCENTER);
        DrawTextA(hdc, "STOCKNET", -1, &logo, DT_LEFT | DT_VCENTER);
        // Restore original font and clean up
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);

        EndPaint(hWnd, &ps);

    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        //free(TradeHistory);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

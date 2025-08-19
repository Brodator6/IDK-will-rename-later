#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif 

#include <iostream> //dependencies 
#include <functional>
#include <chrono>
#include <thread>
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void mouseClick(INPUT* mouseInput, int key){//function for mouse emulation
    POINT mousePos;
    GetCursorPos(&mousePos);//position update
    mouseInput[0].mi.dx = (mousePos.x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    mouseInput[0].mi.dy = (mousePos.y * 65535) / GetSystemMetrics(SM_CXSCREEN);
    SendInput(1, mouseInput, sizeof(INPUT));
    mouseInput[0].mi.dwFlags = key/2;
    SendInput(1, mouseInput, sizeof(INPUT));
    mouseInput[0].mi.dwFlags = key;
    SendInput(1, mouseInput, sizeof(INPUT));
 }

void clickshit(bool* clickState, std::chrono::milliseconds delay, std::function<void()> clickFunction) { //main shit
    while(true){//cycle for app
        if (*clickState){
            clickFunction();
        }
        std::this_thread::sleep_for(delay);
    }
 }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    int CPSInput = 100;//click rate vals
    std::chrono::milliseconds delay = std::chrono::milliseconds(1000/CPSInput);
    int key4Input = 4;//click vals
    bool clickState = false;

    INPUT in[1];//data for input function
    in[0].type = INPUT_MOUSE;
    in[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
    in[0].mi.time = 0;
    
    std::function<void()> clickFucntion = [&in, key4Input]()mutable{//packing function as variable
        mouseClick(in, key4Input);
    };


    //creating window and buttons
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ASS";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, L"ASS", L"Sus", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);
    HWND hEdit = CreateWindow(L"EDIT", L"100", WS_VISIBLE | WS_CHILD, 30, 5, 200, 25, hwnd, HMENU(1001), NULL, NULL);
    HWND hButton1 = CreateWindow(L"BUTTON", L"Press me", WS_VISIBLE | WS_CHILD, 10, 120, 80, 25, hwnd, HMENU(1002), NULL, NULL);
    HWND hButton2 = CreateWindow(L"BUTTON", L"Press me", WS_VISIBLE | WS_CHILD, 400, 200, 80, 25, hwnd, HMENU(1003), NULL, NULL);

    RegisterHotKey(NULL, 1, 0, VK_F6); //register F6 as a hotkey
    std::thread clickThread(clickshit, &clickState, delay, clickFucntion); //start thread for clicking
    clickThread.detach(); //detach thread to run independently

    MSG msg = {};//message loop anf it`s variables
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        if (msg.message == WM_HOTKEY && msg.wParam == 1) { // Handle hotkey
            clickState = !clickState;
        }
        DispatchMessage(&msg);
    }
    return 0;
}

 LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        //MessageBox(NULL, L"STFU!", L"Message Box", MB_OK | MB_ICONINFORMATION); 
        PostQuitMessage(0);
        return 0;
        
    case WM_CREATE:
        {
            LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pcs->lpCreateParams);
        }
        return 0;

    case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED) {
                wchar_t* st = new wchar_t [GetWindowTextLength(GetDlgItem(hwnd, 1001))+1];
                GetWindowText(GetDlgItem(hwnd, 1001), st, GetWindowTextLength(GetDlgItem(hwnd, 1001)) + 1);
                // Handle button click
                MessageBox(hwnd, st, L"not", MB_OK | MB_ICONINFORMATION);
            }
        }
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
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
HWND hComboxClickType; //global variable for combobox
HWND hComBoxClickSignal;
std::chrono::milliseconds delay = std::chrono::milliseconds(1000/2);//click rate vals aka delay
int key4Input = 4;//click vals
int inputSize;//input size for mouse click

std::function<void()> CreateClickPattern;

INPUT* in;//data for input function

long  dx = 0;//gotta use pointers/refferences, idk how though
long  dy = 0;//but it still works

void mouseClick(INPUT* mouseInput, long& x, long& y, int key){//function for mouse emulation
    POINT mousePos;
    GetCursorPos(&mousePos);//position update
    x = (mousePos.x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    y = (mousePos.y * 65535) / GetSystemMetrics(SM_CXSCREEN);
    SendInput(inputSize, mouseInput, sizeof(INPUT));
 }

void clickshit(bool* clickState, std::chrono::milliseconds* delay, std::function<void()>* clickFunction) { //main shit
    while(true){//cycle for app
        if (*clickState){
            (*clickFunction)();//gotta find better way to pause/resume this shatan machine
        }
        std::this_thread::sleep_for(*delay);
    }
 }

 void initaiateSingleKeySet(int& size) {//I hate myself for this
    size = 3;
    in = new INPUT[size];
    in[0].type = INPUT_MOUSE;
    in[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
    in[0].mi.time = 0;
    in[0].mi.dx = dx;
    in[0].mi.dy = dy;
    in[1].type = INPUT_MOUSE;
    in[1].mi.dwFlags = key4Input/2;
    in[1].mi.time = 0;
    in[1].mi.dx = dx;
    in[1].mi.dy = dy;
    in[2].type = INPUT_MOUSE;
    in[2].mi.dwFlags = key4Input;
    in[2].mi.time = 0;
    in[2].mi.dx = dx;
    in[2].mi.dy = dy;
}

 void initaiateDoubleKeySet(int& size) {//I hate myself for this
    size = 5;
    in = new INPUT[size];
    in[0].type = INPUT_MOUSE;
    in[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
    in[0].mi.time = 0;
    in[0].mi.dx = dx;
    in[0].mi.dy = dy;
    in[1].type = INPUT_MOUSE;
    in[1].mi.dwFlags = key4Input/2;
    in[1].mi.time = 0;
    in[1].mi.dx = dx;
    in[1].mi.dy = dy;
    in[2].type = INPUT_MOUSE;
    in[2].mi.dwFlags = key4Input;
    in[2].mi.time = 0;
    in[2].mi.dx = dx;
    in[2].mi.dy = dy;
    in[3].type = INPUT_MOUSE;
    in[3].mi.dwFlags = key4Input/2;
    in[3].mi.time = 0;
    in[3].mi.dx = dx;
    in[3].mi.dy = dy;
    in[4].type = INPUT_MOUSE;
    in[4].mi.dwFlags = key4Input;
    in[4].mi.time = 0;
    in[4].mi.dx = dx;
    in[4].mi.dy = dy;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    bool clickState = false;
    
    CreateClickPattern = []()mutable {
        initaiateSingleKeySet(inputSize);
    };

    CreateClickPattern(); //initialize click pattern
    
    std::function<void()> clickFucntion = []()mutable{//packing function as variable
        mouseClick(in, dx, dy, key4Input);
    };


    //creating window and buttons
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"The Hood";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"Sturdust Clicker", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);
    HWND TClicksPerSecond = CreateWindow(L"Static", L"Clicks per second:", WS_VISIBLE | WS_CHILD, 120, 10, 150, 30, hwnd, NULL, NULL, NULL);
    HWND EClicksPerSecond = CreateWindow(L"EDIT", L"10", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 280, 10, 100, 30, hwnd, HMENU(1001), NULL, NULL);
    HWND hButton = CreateWindow(L"Button", L"Start/Stop ", WS_VISIBLE | WS_CHILD, 220, 425, 80, 25, hwnd, HMENU(1003), NULL, NULL);
    hComboxClickType = CreateWindow(L"COMBOBOX", TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 330, 50, 150, 200, hwnd, HMENU(2), NULL, NULL);

    SendMessage(hComboxClickType, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"single click");
    SendMessage(hComboxClickType, (UINT)CB_SETCURSEL, (WPARAM)0, 0);
    SendMessage(hComboxClickType, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)L"double click");

    hComBoxClickSignal = CreateWindow(L"COMBOBOX", TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 330, 80, 150, 200, hwnd, HMENU(3), NULL, NULL);

    SendMessage(hComBoxClickSignal, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"left click");
    SendMessage(hComBoxClickSignal, (UINT)CB_SETCURSEL, (WPARAM)0, 0);
    SendMessage(hComBoxClickSignal, (UINT)CB_ADDSTRING, (WPARAM)1, (LPARAM)L"right click");
    SendMessage(hComBoxClickSignal, (UINT)CB_ADDSTRING, (WPARAM)2, (LPARAM)L"middle click");
    
    RegisterHotKey(NULL, 1, 0, VK_F6); //register F6 as a hotkey
    std::thread clickThread(&clickshit, &clickState, &delay, &clickFucntion); //start thread for clicking
    clickThread.detach(); //detach thread to run independently

    MSG msg = {};//message loop anf it`s variables
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        if (msg.message == WM_HOTKEY && msg.wParam == 1) { // Handle a hotkey
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
            if(HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == 1001) {//clickrate input
                wchar_t* st = new wchar_t [GetWindowTextLength(GetDlgItem(hwnd, 1001))+1];
                GetWindowText(GetDlgItem(hwnd, 1001), st, GetWindowTextLength(GetDlgItem(hwnd, 1001)) + 1);
                delay = std::chrono::milliseconds(1000/_wtoi(st));//gotta fix null st scenario
            }
            
            if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 2) { // Handle combo box selection for type of click 
                int selectedIndex = SendMessage(hComboxClickType, CB_GETCURSEL, 0, 0);
                switch (selectedIndex)
                {
                case 0:
                    CreateClickPattern = []()mutable {
                    initaiateSingleKeySet(inputSize);
                    };
                    CreateClickPattern();
                    MessageBox(NULL, L"Single click selected", L"Combo Box", MB_OK | MB_ICONINFORMATION);
                    break;

                case 1:
                    CreateClickPattern = []()mutable {
                    initaiateDoubleKeySet(inputSize);
                    };
                    CreateClickPattern();
                    MessageBox(NULL, L"Double click selected", L"Combo Box", MB_OK | MB_ICONINFORMATION);
                    break;
                }
            }

            if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 3) { // Handle combo box selection for type of click 
                int selectedIndex = SendMessage(hComBoxClickSignal, CB_GETCURSEL, 0, 0);
                switch (selectedIndex)
                {
                case 0://I hate myself for this, but it is what it is
                    key4Input = 4;// left click
                    CreateClickPattern();
                    MessageBox(NULL, L"Left click selected", L"Combo Box", MB_OK | MB_ICONINFORMATION);
                    break;

                case 1:
                    key4Input = 16;// right click
                    CreateClickPattern();
                    MessageBox(NULL, L"Right click selected", L"Combo Box", MB_OK | MB_ICONINFORMATION);
                    break;

                case 2:
                    key4Input = 64;// middle click
                    CreateClickPattern();
                    MessageBox(NULL, L"Middle click selected", L"Combo Box", MB_OK | MB_ICONINFORMATION);
                    break;
                }
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
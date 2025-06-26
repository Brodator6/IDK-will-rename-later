#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <windows.h>

 void mouseClick(INPUT* mouseInput, int key){
    POINT mousePos;
    GetCursorPos(&mousePos);//оновлення позиції миші
    mouseInput[0].mi.dx = (mousePos.x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    mouseInput[0].mi.dy = (mousePos.y * 65535) / GetSystemMetrics(SM_CXSCREEN);
    SendInput(1, mouseInput, sizeof(INPUT));
    mouseInput[0].mi.dwFlags = key/2;
    SendInput(1, mouseInput, sizeof(INPUT));
    mouseInput[0].mi.dwFlags = key;
    SendInput(1, mouseInput, sizeof(INPUT));
 }

int main(){
    int key4Input = 4;
    int CPSInput = 100;
    std::chrono::milliseconds delay = std::chrono::milliseconds(1000/CPSInput);
    int startButton = 115;
    int i = 0;
    int s = 0;




    bool startIsPressed = false;
    bool clickState = false;

    INPUT in[1];// дані для натиску кнопок миші в місці знаходження курсору
    in[0].type = INPUT_MOUSE;
    in[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
    in[0].mi.time = 0;
    

    std::function<void()> clickFucntion = [&in, key4Input]()mutable{
        mouseClick(in, key4Input);
    };

    while(true){

        if((GetAsyncKeyState(startButton) & 0x8000) && (GetAsyncKeyState(startButton) != s)){
            s = GetAsyncKeyState(startButton);
            clickState = !clickState;
            continue;
        }else{
            s = GetAsyncKeyState(startButton);
        }

        std::this_thread::sleep_for(delay);
        if (clickState){
            clickFucntion();
        }
    }
    return 0;
 }

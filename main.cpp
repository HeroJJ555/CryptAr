#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

const int XOR_HASH_KEY = 42;
const int ADDITION_HASH_KEY = 13;

std::string customXORHash(const std::string& input);
std::string customAdditionHash(const std::string& input);

HWND inputEdit, outputEdit, hashButton;

HFONT hFont;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {

            inputEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 10, 10, 300, 25, hwnd, (HMENU)1, nullptr, nullptr);
            hashButton = CreateWindow("BUTTON", "Szyfruj", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 320, 10, 80, 25, hwnd, (HMENU)2, nullptr, nullptr);
            outputEdit = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE, 10, 50, 390, 200, hwnd, (HMENU)3, nullptr, nullptr);
            SendMessage(outputEdit, EM_SETREADONLY, TRUE, 0);

            hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

            SendMessage(hashButton, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 2) {
                char inputBuffer[1024];
                GetWindowText(inputEdit, inputBuffer, sizeof(inputBuffer));
                std::string inputText(inputBuffer);

                std::string xorHashed = customXORHash(inputText);

                std::string additionHashed = customAdditionHash(inputText);

                std::stringstream ss;
                ss << "Metoda XOR Hash: " << xorHashed << "\r\n";
                ss << "Metoda Addition Hash: " << additionHashed << "\r\n";
                SendMessage(outputEdit, EM_REPLACESEL, 0, (LPARAM)ss.str().c_str());
            }
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

std::string customXORHash(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c ^= XOR_HASH_KEY;
    }
    return result;
}

std::string customAdditionHash(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c += ADDITION_HASH_KEY;
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInst;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "HashAppClass";
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "CryptAr by HeroJJ", WS_OVERLAPPEDWINDOW, 100, 100, 450, 300, nullptr, nullptr, hInst, nullptr);
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

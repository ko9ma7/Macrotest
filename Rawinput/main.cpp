#include <Windows.h>
#include <iostream>

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main() {
    // ������ Ŭ���� ����
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    // ������ Ŭ���� ���
    RegisterClass(&wc);

    // ������ ����
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Raw Input Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (hwnd == NULL) {
        std::cerr << "Failed to create window\n";
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Raw Input ��ġ ���
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;    // Generic desktop controls
    rid.usUsage = 0x06;        // Keyboard
    rid.dwFlags = 0;           // No flags
    rid.hwndTarget = hwnd;     // Handle to the target window

    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
        std::cerr << "Failed to register raw input device\n";
        return 0;
    }

    // �޽��� ����
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INPUT: {
        // Raw Input ������ ó��
        UINT dwSize;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[dwSize];
        if (lpb == NULL) {
            return 0;
        }

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
            std::cerr << "GetRawInputData does not return correct size\n";
        }

        RAWINPUT* raw = (RAWINPUT*)lpb;

        if (raw->header.dwType == RIM_TYPEKEYBOARD) {
            RAWKEYBOARD rawKB = raw->data.keyboard;
            std::cout << "MakeCode: " << rawKB.MakeCode << ", Flags: " << rawKB.Flags
                << ", Reserved: " << rawKB.Reserved << ", VKey: " << rawKB.VKey
                << ", Message: " << rawKB.Message << std::endl;
        }

        delete[] lpb;
        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

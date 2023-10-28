#pragma once

#include <Windows.h>
#include <dinput.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl.h>


class Input
{

public: // メンバ関数
    // namespace省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    // 初期化
    void Initialize(HINSTANCE hInstance, HWND hwnd);
    // 更新
    void Update();
private:
    ComPtr<IDirectInputDevice8> keyboard;
};


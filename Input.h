#pragma once

#include <Windows.h>
#include <dinput.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl.h>


class Input
{

public: // �����o�֐�
    // namespace�ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    // ������
    void Initialize(HINSTANCE hInstance, HWND hwnd);
    // �X�V
    void Update();
private:
    ComPtr<IDirectInputDevice8> keyboard;
};


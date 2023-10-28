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

    /// <summary>
    /// �L�[�̉������`�F�b�N
    /// </summary>
    /// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
    /// <returns>������Ă��邩</returns>
    bool PushKey(BYTE keyNumber);

    /// <summary>
    /// �L�[�̃g���K�[���`�F�b�N
    /// </summary>
    /// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
    /// <returns>�g���K�[��</returns>
    bool TriggerKey(BYTE keyNumber);



private:
    ComPtr<IDirectInputDevice8> keyboard;
    BYTE key[256] = {};
    // �O��̑S�L�[�̏��
    BYTE keyPre[256] = {};
    ComPtr<IDirectInput8> directInput;


};


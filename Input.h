#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl.h>

#include "WinApp.h"

class Input
{

public: // メンバ関数
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// 初期化
	void Initialize(WinApp* winApp);
	// 更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);


private:
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	// 前回の全キーの状態
	BYTE keyPre[256] = {};
	ComPtr<IDirectInput8> directInput;
	WinApp* winApp_ = nullptr;

};


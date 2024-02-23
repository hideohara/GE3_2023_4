#pragma once

//#include <string>
//#include <wrl.h>
//#include <dxcapi.h>

#include <Windows.h>
#include <wrl.h>
#include <string>
#include <d3dx12.h>
#include "DirectXCommon.h"
#include <DirectXTex.h>
#include <dxcapi.h>

// ���ʕ���
class SpriteCommon
{
private:
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Initialize(DirectXCommon* dxCommon);

    ID3D12RootSignature* GetRootSignature() {
        return rootSignature.Get();
    }
    ID3D12PipelineState* GetGraphicsPipelineState() {
        return graphicsPipelineState.Get();
    }
private:
    static IDxcBlob* CompileShader(
        // Compiler����Shader�t�@�C���ւ̃p�X
        const std::wstring& filePath,
        // Compiler�Ɏg�p����Profile
        const wchar_t* profile,
        // �������Ő����������̂�3��
        IDxcUtils* dxcUtils,
        IDxcCompiler3* dxcCompiler,
        IDxcIncludeHandler* includeHandler);

    DirectXCommon* dxCommon_;

    ComPtr<ID3D12RootSignature> rootSignature;
    ComPtr<ID3D12PipelineState> graphicsPipelineState;
};


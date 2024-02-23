#include "SpriteCommon.h"
#include <cassert>
#pragma comment(lib,"dxcompiler.lib")

using namespace Microsoft::WRL;

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
	HRESULT result;

	dxCommon_ = dxCommon;

	// dxcCompiler��������
	ComPtr<IDxcUtils> dxcUtils;
	ComPtr<IDxcCompiler3> dxcCompiler;

	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(result));

	// �����_��include�͂��Ȃ����Ainclude�ɑΉ����邽�߂̐ݒ���s���Ă���
	ComPtr <IDxcIncludeHandler> includeHandler;
	result = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(result));


	// RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// �V���A���C�Y���ăo�C�i���ɂ���
	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(result)) {
		assert(false);
	}
	// �o�C�i�������ɐ���

	result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};
	// ���ׂĂ̐F�v�f����������
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// ���ʁi���v���j��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// �O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shader���R���p�C������
	IDxcBlob* vertexShaderBlob = CompileShader(L"Resources/SpriteVS.hlsl", L"vs_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(vertexShaderBlob != nullptr);

	IDxcBlob* pixelShaderBlob = CompileShader(L"Resources/SpritePS.hlsl", L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();// RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;// InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };// VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };// PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;// BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;// RasterizerState
	// ��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// ���p����g�|���W�i�`��j�̃^�C�v�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// �ǂ̂悤�ɉ�ʂɐF��ł����ނ��̐ݒ�i�C�ɂ��Ȃ��ėǂ��j
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ���ۂɐ���

	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(result));


}

IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	// ���ꂩ��V�F�[�_�[���R���p�C������|�����O�ɏo��
	//Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlsl�t�@�C����ǂ�
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT result = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// �ǂ߂Ȃ�������~�߂�
	assert(SUCCEEDED(result));

	// �ǂݍ��񂾃t�@�C���̓��e��ݒ肷��
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8�̕����R�[�h�ł��邱�Ƃ�ʒm

	LPCWSTR arguments[] = {
	filePath.c_str(), // �R���p�C���Ώۂ�hlsl�t�@�C����
	L"-E", L"main", // �G���g���[�|�C���g�̎w��B��{�I��main�ȊO�ɂ͂��Ȃ�
	L"-T", profile, // ShaderProfile�̐ݒ�
	L"-Zi", L"-Qembed_debug",   // �f�o�b�O�p�̏��𖄂ߍ���
	L"-Od",     // �œK�����O���Ă���
	L"-Zpr",    // ���������C�A�E�g�͍s�D��
	};
	// ���ۂ�Shader���R���p�C������
	IDxcResult* shaderResult = nullptr;
	result = dxcCompiler->Compile(
		&shaderSourceBuffer,    // �ǂݍ��񂾃t�@�C��
		arguments,              // �R���p�C���I�v�V����
		_countof(arguments),    // �R���p�C���I�v�V�����̐�
		includeHandler,         // include���܂܂ꂽ���X
		IID_PPV_ARGS(&shaderResult) // �R���p�C������
	);
	// �R���p�C���G���[�ł͂Ȃ�dxc���N���ł��Ȃ��Ȃǒv���I�ȏ�
	assert(SUCCEEDED(result));

	// �x���E�G���[���o�Ă��烍�O�ɏo���Ď~�߂�
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		//Log(shaderError->GetStringPointer());
		// �x���E�G���[�_���[�b�^�C
		assert(false);
	}

	// �R���p�C�����ʂ�����s�p�̃o�C�i���������擾
	IDxcBlob* shaderBlob = nullptr;
	result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(result));
	// �����������O���o��
	//Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// �����g��Ȃ����\�[�X�����
	shaderSource->Release();
	shaderResult->Release();
	// ���s�p�̃o�C�i����ԋp
	return shaderBlob;


	return nullptr;
}

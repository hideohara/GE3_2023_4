#include "Sprite.h"


#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	HRESULT result;

	dxCommon_ = dxCommon;
	common_ = common;

	// ���_���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeap���g��
	// ���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// �o�b�t�@���\�[�X�B�e�N�X�`���̏ꍇ�͂܂��ʂ̐ݒ������
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(XMFLOAT4) * 3;// ���\�[�X�̃T�C�Y�B�����Vector4��3���_��
	// �o�b�t�@�̏ꍇ�͂�����1�ɂ��錈�܂�
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// �o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���ۂɒ��_���\�[�X�����

	result = dxCommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[���쐬����

	// ���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// �g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	vertexBufferView.SizeInBytes = sizeof(XMFLOAT4) * 3;
	// 1���_������̃T�C�Y
	vertexBufferView.StrideInBytes = sizeof(XMFLOAT4);


}

void Sprite::Draw()
{
	XMFLOAT4* vertexData = nullptr;
	//ComPtr<ID3D12Resource> vertexResource;

	// �������ނ��߂̃A�h���X���擾
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	// ����
	vertexData[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	// ��
	vertexData[1] = { 0.0f, 0.5f, 0.0f, 1.0f };
	// �E��
	vertexData[2] = { 0.5f, -0.5f, 0.0f, 1.0f };

	// RootSignature��ݒ�BPSO�ɐݒ肵�Ă��邯�Ǖʓr�ݒ肪�K�v
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetGraphicsPipelineState());   // PSO��ݒ�
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);   // VBV��ݒ�

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �`��I�iDrawCall/�h���[�R�[���j�B3���_��1�̃C���X�^���X�B�C���X�^���X�ɂ��Ă͍���
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

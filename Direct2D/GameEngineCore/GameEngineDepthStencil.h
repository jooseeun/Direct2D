#pragma once
#include "GameEngineRes.h"

// �������ٽ� �����̶�
// �������ٽ� �ؽ�ó��



// ���� : ���� �ȼ����̴��� ��°���� ����Ÿ�ٿ� ����Ҷ� ���� ���̰� ���� �������� ���� ó��
class GameEngineDepthStencil : public GameEngineRes<GameEngineDepthStencil>
{
//public:
//	static GameEngineDepthStencil* Create(const float4 _Scale);

public:
	static GameEngineDepthStencil* Create(
		const std::string& _Name, const D3D11_DEPTH_STENCIL_DESC& _Desc);

public:
	// constrcuter destructer
	GameEngineDepthStencil();
	~GameEngineDepthStencil();

	// delete Function
	GameEngineDepthStencil(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil(GameEngineDepthStencil&& _Other) noexcept = delete;
	GameEngineDepthStencil& operator=(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil& operator=(GameEngineDepthStencil&& _Other) noexcept = delete;

	void Setting();

protected:

private:
	D3D11_DEPTH_STENCIL_DESC Desc;
	ID3D11DepthStencilState* State;

	void Create(const D3D11_DEPTH_STENCIL_DESC& _Desc);
};


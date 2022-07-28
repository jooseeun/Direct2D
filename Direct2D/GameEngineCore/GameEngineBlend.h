#pragma once
#include "GameEngineRes.h"

// ���� : ���� �ȼ����̴��� ��°���� ����Ÿ�ٿ� ����Ҷ� ���� ���տ� ����
class GameEngineBlend : public GameEngineRes<GameEngineBlend>
{
public:
	static GameEngineBlend* Create(const std::string& _Name, const D3D11_BLEND_DESC& _Desc);

public:
	// constrcuter destructer
	GameEngineBlend();
	~GameEngineBlend();

	// delete Function
	GameEngineBlend(const GameEngineBlend& _Other) = delete;
	GameEngineBlend(GameEngineBlend&& _Other) noexcept = delete;
	GameEngineBlend& operator=(const GameEngineBlend& _Other) = delete;
	GameEngineBlend& operator=(GameEngineBlend&& _Other) noexcept = delete;

	void Setting();

protected:

private:
	ID3D11BlendState* State;
	D3D11_BLEND_DESC Desc;
	float4 Factor;
	unsigned int Mask;

	void Create(const D3D11_BLEND_DESC& _Info);

};


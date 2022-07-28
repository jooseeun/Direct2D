#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineShaderResourcesHelper.h"

// ���� :
class GameEngineRenderingPipeLine;
class GameEngineDefaultRenderer : public GameEngineRenderer
{
	// �ϳ��� ������ ������������
	// �ϳ��� �Ž�
	// �ϳ��� ���׸���

	// �ϳ��� ����Ÿ��


public:
	GameEngineShaderResourcesHelper ShaderResources;

	// constrcuter destructer
	GameEngineRenderingPipeLine* GetPipeLine()
	{
		return PipeLine;
	}

	GameEngineDefaultRenderer();
	~GameEngineDefaultRenderer();

	// delete Function
	GameEngineDefaultRenderer(const GameEngineDefaultRenderer& _Other) = delete;
	GameEngineDefaultRenderer(GameEngineDefaultRenderer&& _Other) noexcept = delete;
	GameEngineDefaultRenderer& operator=(const GameEngineDefaultRenderer& _Other) = delete;
	GameEngineDefaultRenderer& operator=(GameEngineDefaultRenderer&& _Other) noexcept = delete;

	virtual void Render(float _DeltaTime);

	void SetPipeLine(const std::string& _Name);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}


private:
	// �������� �޸� �����ϴ� �޷θ����� ��� �ְ�
	GameEngineRenderingPipeLine* PipeLine;

};


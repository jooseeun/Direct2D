#include "PreCompile.h"
#include "GameEngineDefaultRenderer.h"
#include "GameEngineRenderingPipeLine.h"

GameEngineDefaultRenderer::GameEngineDefaultRenderer() 
	:PipeLine(nullptr)
{
}

GameEngineDefaultRenderer::~GameEngineDefaultRenderer() 
{
}

void GameEngineDefaultRenderer::Start()
{
	GameEngineRenderer::Start();

	// ���� �� ������ �ִٸ� ���⼭ �ض�.
}

void GameEngineDefaultRenderer::SetPipeLine(const std::string& _Name)
{
	PipeLine = GameEngineRenderingPipeLine::Find(_Name);

	if (nullptr == PipeLine)
	{
		MsgBoxAssert("�������� �ʴ� ������������ �����Ϸ��� �߽��ϴ�.");
		return;
	}

	ShaderResources.ResourcesCheck(PipeLine);

	if (true == ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	{
		ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &GetTransformData(), sizeof(GetTransformData()));
	}

	if (true == ShaderResources.IsConstantBuffer("RENDEROPTION"))
	{
		ShaderResources.SetConstantBufferLink("RENDEROPTION", &Option, sizeof(Option));
	}

}

void GameEngineDefaultRenderer::Render(float _DeltaTime) 
{
	if (nullptr == PipeLine)
	{
		MsgBoxAssert("������ ������������ ���õ��� ������ �������� �Ҽ� �����ϴ�.");
	}

	// �غ�� ��� ���ҽ����� �� �������ش�.
	ShaderResources.AllResourcesSetting();
	PipeLine->Rendering();
}



GameEngineRenderingPipeLine* GameEngineDefaultRenderer::GetPipeLine()
{
	if (false == PipeLine->IsOriginal())
	{
		return PipeLine;
	}
	
	PipeLine = GetClonePipeLine(PipeLine);
	return PipeLine;
}
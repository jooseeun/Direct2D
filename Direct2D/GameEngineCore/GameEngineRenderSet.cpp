#include "PreCompile.h"
#include "GameEngineRenderSet.h"

GameEngineRenderSet::GameEngineRenderSet() 
{
}

GameEngineRenderSet::~GameEngineRenderSet() 
{
}


void GameEngineRenderSet::SetPipeLine(const std::string& _Name)
{
	PipeLine = GameEngineRenderingPipeLine::Find(_Name);

	if (nullptr == PipeLine)
	{
		MsgBoxAssert("�������� �ʴ� ������������ �����Ϸ��� �߽��ϴ�.");
		return;
	}

	ShaderResources.ResourcesCheck(PipeLine);

	//if (true == ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	//{
	//	ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &GetTransformData(), sizeof(GetTransformData()));
	//}

	//if (true == ShaderResources.IsConstantBuffer("RENDEROPTION"))
	//{
	//	ShaderResources.SetConstantBufferLink("RENDEROPTION", &Option, sizeof(Option));
	//}

}

void GameEngineRenderSet::Render()
{
	if (nullptr == PipeLine)
	{
		MsgBoxAssert("������ ������������ ���õ��� ������ �������� �Ҽ� �����ϴ�.");
	}

	// �غ�� ��� ���ҽ����� �� �������ش�.
	ShaderResources.AllResourcesSetting();
	PipeLine->Rendering();
	ShaderResources.AllResourcesReset();
}

GameEngineRenderingPipeLine* GameEngineRenderSet::GetPipeLine()
{
	if (false == PipeLine->IsOriginal())
	{
		return PipeLine;
	}

	GameEngineRenderingPipeLine* Clone = GameEngineRenderingPipeLine::Create();
	Clone->Copy(PipeLine);
	PipeLine = Clone;
	return PipeLine;
}
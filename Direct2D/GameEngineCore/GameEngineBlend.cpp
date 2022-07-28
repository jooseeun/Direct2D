#include "PreCompile.h"
#include "GameEngineBlend.h"

GameEngineBlend::GameEngineBlend() 
	: Mask(0xffffffff)
{
}

GameEngineBlend::~GameEngineBlend() 
{
	if (nullptr != State)
	{
		State->Release();
	}
}


GameEngineBlend* GameEngineBlend::Create(
	const std::string& _Name, const D3D11_BLEND_DESC& _Desc
)
{
	GameEngineBlend* NewRes = CreateResName(_Name);
	NewRes->Create(_Desc);
	return NewRes;
}

void GameEngineBlend::Create(const D3D11_BLEND_DESC& _Desc)
{
	Desc = _Desc;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBlendState(&Desc, &State))
	{
		MsgBoxAssert("���ؽ� ���� ������ �����߽��ϴ�.");
	}
}

void GameEngineBlend::Setting()
{

	if (nullptr == State)
	{
		MsgBoxAssert("�����Ͷ������� nullptr �Դϴ�. �����Ҽ� �����ϴ�.");
		return;
	}

	GameEngineDevice::GetContext()->OMSetBlendState(State, Factor.Arr1D, Mask);

}

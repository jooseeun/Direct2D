#include "PreCompile.h"
#include "GameEngineSampler.h"

GameEngineSampler* GameEngineSampler::Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info)
{
	GameEngineSampler* NewRes = CreateResName(_Name);
	NewRes->Create(_Info);
	return NewRes;
}

GameEngineSampler::GameEngineSampler() 
	: State(nullptr)
	, Desc{}
{
}

GameEngineSampler::~GameEngineSampler() 
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}



void GameEngineSampler::Create(const D3D11_SAMPLER_DESC& _Info)
{
	Desc = _Info;

	if (S_OK != GameEngineDevice::GetDevice()->CreateSamplerState(&_Info, &State))
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� ����� �� �����ϴ�.");
	}
}


void GameEngineSampler::VSSetting(int _BindPoint)
{
	if (nullptr == State)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->VSSetSamplers(_BindPoint, 1, &State);
}

void GameEngineSampler::PSSetting(int _BindPoint)
{
	if (nullptr == State)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->PSSetSamplers(_BindPoint, 1, &State);
}
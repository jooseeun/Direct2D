#include "PreCompile.h"
#include "BlueLight.h"
#include <GameEngineBase/GameEngineWindow.h>

BlueLight::BlueLight()
	: CopyTarget(nullptr)
{
}

BlueLight::~BlueLight()
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}

void BlueLight::EffectInit()
{
	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("BlueLight");

	// EffetSet
}

void BlueLight::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);


	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));
	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
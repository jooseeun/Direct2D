#include "PreCompile.h"
#include "ColorOverlay.h"
#include <GameEngineBase/GameEngineWindow.h>

ColorOverlay::ColorOverlay()
	: CopyTarget(nullptr)
{
}

ColorOverlay::~ColorOverlay()
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}

void ColorOverlay::EffectInit()
{
	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

	EffectSet.SetPipeLine("ColorOverlay");

	// EffetSet
}

void ColorOverlay::Effect(GameEngineRenderTarget* _Target)
{
	CopyTarget->Copy(_Target);


	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));
	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);
}
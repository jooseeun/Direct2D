#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>

// Ό³Έν :
class ColorOverlay : public GameEnginePostEffect
{
public:
	// constrcuter destructer
	ColorOverlay();
	~ColorOverlay();

	// delete Function
	ColorOverlay(const ColorOverlay& _Other) = delete;
	ColorOverlay(ColorOverlay&& _Other) noexcept = delete;
	ColorOverlay& operator=(const ColorOverlay& _Other) = delete;
	ColorOverlay& operator=(ColorOverlay&& _Other) noexcept = delete;

	virtual void EffectInit();
	virtual void Effect(GameEngineRenderTarget* _Render);

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;

private:
};


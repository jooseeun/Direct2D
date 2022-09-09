#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineRenderSet.h>

// Ό³Έν :
class BlueLight : public GameEnginePostEffect
{
public:
	// constrcuter destructer
	BlueLight();
	~BlueLight();

	// delete Function
	BlueLight(const BlueLight& _Other) = delete;
	BlueLight(BlueLight&& _Other) noexcept = delete;
	BlueLight& operator=(const BlueLight& _Other) = delete;
	BlueLight& operator=(BlueLight&& _Other) noexcept = delete;

	virtual void EffectInit();
	virtual void Effect(GameEngineRenderTarget* _Render);

protected:
	GameEngineRenderTarget* CopyTarget;
	GameEngineRenderSet EffectSet;

private:
};


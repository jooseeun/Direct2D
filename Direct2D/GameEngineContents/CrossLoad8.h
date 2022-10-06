#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad8 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad8();
	~CrossLoad8();

	// delete Function
	CrossLoad8(const CrossLoad8& _Other) = delete;
	CrossLoad8(CrossLoad8&& _Other) noexcept = delete;
	CrossLoad8& operator=(const CrossLoad8& _Other) = delete;
	CrossLoad8& operator=(CrossLoad8&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;
	void LevelEndEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


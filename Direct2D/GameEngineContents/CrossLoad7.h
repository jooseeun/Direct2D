#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad7 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad7();
	~CrossLoad7();

	// delete Function
	CrossLoad7(const CrossLoad7& _Other) = delete;
	CrossLoad7(CrossLoad7&& _Other) noexcept = delete;
	CrossLoad7& operator=(const CrossLoad7& _Other) = delete;
	CrossLoad7& operator=(CrossLoad7&& _Other) noexcept = delete;

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


#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad6 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad6();
	~CrossLoad6();

	// delete Function
	CrossLoad6(const CrossLoad6& _Other) = delete;
	CrossLoad6(CrossLoad6&& _Other) noexcept = delete;
	CrossLoad6& operator=(const CrossLoad6& _Other) = delete;
	CrossLoad6& operator=(CrossLoad6&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


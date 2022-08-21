#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Tutorial3Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Tutorial3Level();
	~Tutorial3Level();

	// delete Function
	Tutorial3Level(const Tutorial3Level& _Other) = delete;
	Tutorial3Level(Tutorial3Level&& _Other) noexcept = delete;
	Tutorial3Level& operator=(const Tutorial3Level& _Other) = delete;
	Tutorial3Level& operator=(Tutorial3Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Town3Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Town3Level();
	~Town3Level();

	// delete Function
	Town3Level(const Town3Level& _Other) = delete;
	Town3Level(Town3Level&& _Other) noexcept = delete;
	Town3Level& operator=(const Town3Level& _Other) = delete;
	Town3Level& operator=(Town3Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


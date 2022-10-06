#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Tutorial4Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Tutorial4Level();
	~Tutorial4Level();

	// delete Function
	Tutorial4Level(const Tutorial4Level& _Other) = delete;
	Tutorial4Level(Tutorial4Level&& _Other) noexcept = delete;
	Tutorial4Level& operator=(const Tutorial4Level& _Other) = delete;
	Tutorial4Level& operator=(Tutorial4Level&& _Other) noexcept = delete;
	GameEngineSoundPlayer BgmPlayer;

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


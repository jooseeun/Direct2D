#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Tutorial2Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Tutorial2Level();
	~Tutorial2Level();

	// delete Function
	Tutorial2Level(const Tutorial2Level& _Other) = delete;
	Tutorial2Level(Tutorial2Level&& _Other) noexcept = delete;
	Tutorial2Level& operator=(const Tutorial2Level& _Other) = delete;
	Tutorial2Level& operator=(Tutorial2Level&& _Other) noexcept = delete;
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


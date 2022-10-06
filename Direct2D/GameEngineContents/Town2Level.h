#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Town2Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Town2Level();
	~Town2Level();

	// delete Function
	Town2Level(const Town2Level& _Other) = delete;
	Town2Level(Town2Level&& _Other) noexcept = delete;
	Town2Level& operator=(const Town2Level& _Other) = delete;
	Town2Level& operator=(Town2Level&& _Other) noexcept = delete;
	static GameEngineSoundPlayer TownBgmPlayer;

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


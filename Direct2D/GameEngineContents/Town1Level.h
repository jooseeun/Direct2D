#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class Town1Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Town1Level();
	~Town1Level();

	// delete Function
	Town1Level(const Town1Level& _Other) = delete;
	Town1Level(Town1Level&& _Other) noexcept = delete;
	Town1Level& operator=(const Town1Level& _Other) = delete;
	Town1Level& operator=(Town1Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


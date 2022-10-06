#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :

class GameEngineCameraActor;
class Tutorial1Level : public PlayLevelManager
{
public:
	// constrcuter destructer
	Tutorial1Level();
	~Tutorial1Level();

	// delete Function
	Tutorial1Level(const Tutorial1Level& _Other) = delete;
	Tutorial1Level(Tutorial1Level&& _Other) noexcept = delete;
	Tutorial1Level& operator=(const Tutorial1Level& _Other) = delete;
	Tutorial1Level& operator=(Tutorial1Level&& _Other) noexcept = delete;
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


#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"


class GameEngineCameraActor;
class CrossLoadBossLevel : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoadBossLevel();
	~CrossLoadBossLevel();

	// delete Function
	CrossLoadBossLevel(const CrossLoadBossLevel& _Other) = delete;
	CrossLoadBossLevel(CrossLoadBossLevel&& _Other) noexcept = delete;
	CrossLoadBossLevel& operator=(const CrossLoadBossLevel& _Other) = delete;
	CrossLoadBossLevel& operator=(CrossLoadBossLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


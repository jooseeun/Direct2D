#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad10 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad10();
	~CrossLoad10();

	// delete Function
	CrossLoad10(const CrossLoad10& _Other) = delete;
	CrossLoad10(CrossLoad10&& _Other) noexcept = delete;
	CrossLoad10& operator=(const CrossLoad10& _Other) = delete;
	CrossLoad10& operator=(CrossLoad10&& _Other) noexcept = delete;

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


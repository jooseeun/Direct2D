#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad2 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad2();
	~CrossLoad2();

	// delete Function
	CrossLoad2(const CrossLoad2& _Other) = delete;
	CrossLoad2(CrossLoad2&& _Other) noexcept = delete;
	CrossLoad2& operator=(const CrossLoad2& _Other) = delete;
	CrossLoad2& operator=(CrossLoad2&& _Other) noexcept = delete;

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


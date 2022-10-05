#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad5 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad5();
	~CrossLoad5();

	// delete Function
	CrossLoad5(const CrossLoad5& _Other) = delete;
	CrossLoad5(CrossLoad5&& _Other) noexcept = delete;
	CrossLoad5& operator=(const CrossLoad5& _Other) = delete;
	CrossLoad5& operator=(CrossLoad5&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


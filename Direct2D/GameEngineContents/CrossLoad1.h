#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad1 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad1();
	~CrossLoad1();

	// delete Function
	CrossLoad1(const CrossLoad1& _Other) = delete;
	CrossLoad1(CrossLoad1&& _Other) noexcept = delete;
	CrossLoad1& operator=(const CrossLoad1& _Other) = delete;
	CrossLoad1& operator=(CrossLoad1&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void LevelStartEvent() override;

private:

	GameEngineCameraActor* Camera;
	float4 MapSize;

};


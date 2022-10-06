#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad9 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad9();
	~CrossLoad9();

	// delete Function
	CrossLoad9(const CrossLoad9& _Other) = delete;
	CrossLoad9(CrossLoad9&& _Other) noexcept = delete;
	CrossLoad9& operator=(const CrossLoad9& _Other) = delete;
	CrossLoad9& operator=(CrossLoad9&& _Other) noexcept = delete;

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


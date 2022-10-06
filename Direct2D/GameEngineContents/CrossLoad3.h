#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad3 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad3();
	~CrossLoad3();

	// delete Function
	CrossLoad3(const CrossLoad3& _Other) = delete;
	CrossLoad3(CrossLoad3&& _Other) noexcept = delete;
	CrossLoad3& operator=(const CrossLoad3& _Other) = delete;
	CrossLoad3& operator=(CrossLoad3&& _Other) noexcept = delete;

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


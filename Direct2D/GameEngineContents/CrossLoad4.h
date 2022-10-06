#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "PlayLevelManager.h"
// Ό³Έν :
class GameEngineCameraActor;
class CrossLoad4 : public PlayLevelManager
{
public:
	// constrcuter destructer
	CrossLoad4();
	~CrossLoad4();

	// delete Function
	CrossLoad4(const CrossLoad4& _Other) = delete;
	CrossLoad4(CrossLoad4&& _Other) noexcept = delete;
	CrossLoad4& operator=(const CrossLoad4& _Other) = delete;
	CrossLoad4& operator=(CrossLoad4&& _Other) noexcept = delete;

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


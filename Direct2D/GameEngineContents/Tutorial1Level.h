#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// Ό³Έν :
class GameEngineCameraActor;
class Tutorial1Level : public GameEngineLevel
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void OnEvent() override;

private:

	GameEngineCameraActor* Camera;
};


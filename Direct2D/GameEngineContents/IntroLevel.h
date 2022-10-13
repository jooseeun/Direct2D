#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineBase/GameEngineSound.h>

// Ό³Έν :
class GameEngineCameraActor;
class TitleObject;
class IntroLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	IntroLevel();
	~IntroLevel();

	// delete Function
	IntroLevel(const IntroLevel& _Other) = delete;
	IntroLevel(IntroLevel&& _Other) noexcept = delete;
	IntroLevel& operator=(const IntroLevel& _Other) = delete;
	IntroLevel& operator=(IntroLevel&& _Other) noexcept = delete;

	GameEngineSoundPlayer BgmPlayer;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	void LevelStartEvent() override;
	void LevelEndEvent() override;

	float Time;
	TitleObject* TitleCom;
	GameEngineCameraActor* Camera;
};


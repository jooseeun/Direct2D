#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class Elderbug : public GameEngineActor
{
public:
	// constrcuter destructer
	Elderbug();
	~Elderbug();

	// delete Function
	Elderbug(const Elderbug& _Other) = delete;
	Elderbug(Elderbug&& _Other) noexcept = delete;
	Elderbug& operator=(const Elderbug& _Other) = delete;
	Elderbug& operator=(Elderbug&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	void StartTalk();
	void CheckDir();
	PLAYERDIR CurDir;
	
	CollisionReturn PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other);
	
	
	GameEngineCollision* ElderbugCollision;
	GameEngineTextureRenderer* ElderbugRenderer;
	GameEngineTextureRenderer* PromptRenderer;


	GameEngineFontRenderer* Font;


	float FontA;
	bool Trigger;
};


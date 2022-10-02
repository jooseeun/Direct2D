#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class FalseKnightFireBall : public GameEngineActor
{
public:
	// constrcuter destructer
	FalseKnightFireBall();
	~FalseKnightFireBall();

	// delete Function
	FalseKnightFireBall(const FalseKnightFireBall& _Other) = delete;
	FalseKnightFireBall(FalseKnightFireBall&& _Other) noexcept = delete;
	FalseKnightFireBall& operator=(const FalseKnightFireBall& _Other) = delete;
	FalseKnightFireBall& operator=(FalseKnightFireBall&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}
private:
	void Gravity();

	GameEngineTextureRenderer* SkillRenderer;
	GameEngineCollision* SkillCol;
};


#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class FallenGround : public GameEngineActor
{
public:
	// constrcuter destructer
	FallenGround();
	~FallenGround();

	// delete Function
	FallenGround(const FallenGround& _Other) = delete;
	FallenGround(FallenGround&& _Other) noexcept = delete;
	FallenGround& operator=(const FallenGround& _Other) = delete;
	FallenGround& operator=(FallenGround&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End();
private:
	GameEngineTextureRenderer* GroundRenderer1;
	GameEngineTextureRenderer* GroundRenderer2;
	GameEngineCollision* GroundCol1;
	GameEngineCollision* GroundCol2;
	bool Trigger;

	CollisionReturn StartTrigger(GameEngineCollision* _This, GameEngineCollision* _Other);
};


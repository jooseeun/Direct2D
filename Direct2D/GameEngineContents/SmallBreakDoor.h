#pragma once

#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class SmallBreakDoor : public GameEngineActor
{
public:
	// constrcuter destructer
	SmallBreakDoor();
	~SmallBreakDoor();

	// delete Function
	SmallBreakDoor(const SmallBreakDoor& _Other) = delete;
	SmallBreakDoor(SmallBreakDoor&& _Other) noexcept = delete;
	SmallBreakDoor& operator=(const SmallBreakDoor& _Other) = delete;
	SmallBreakDoor& operator=(SmallBreakDoor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

private:
	GameEngineCollision* DoorCollision;
	GameEngineTextureRenderer* DoorRenderer;


	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);
};


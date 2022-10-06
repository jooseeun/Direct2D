#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class BigBreakDoor : public GameEngineActor
{
public:
	// constrcuter destructer
	BigBreakDoor();
	~BigBreakDoor();

	// delete Function
	BigBreakDoor(const BigBreakDoor& _Other) = delete;
	BigBreakDoor(BigBreakDoor&& _Other) noexcept = delete;
	BigBreakDoor& operator=(const BigBreakDoor& _Other) = delete;
	BigBreakDoor& operator=(BigBreakDoor&& _Other) noexcept = delete;
	GameEngineSoundPlayer SoundPlayer;
protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

private:
	GameEngineCollision* DoorCollision;
	GameEngineTextureRenderer* DoorRenderer;
	int HP;
	bool Check;
	float Time;

	CollisionReturn CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other);
};


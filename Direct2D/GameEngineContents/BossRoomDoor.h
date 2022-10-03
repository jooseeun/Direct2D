#pragma once
#include <GameEngineCore/CoreMinimal.h>
#include "ContentsEnum.h"

class BossRoomDoor : public GameEngineActor
{
public:
	// constrcuter destructer
	BossRoomDoor();
	~BossRoomDoor();

	// delete Function
	BossRoomDoor(const BossRoomDoor& _Other) = delete;
	BossRoomDoor(BossRoomDoor&& _Other) noexcept = delete;
	BossRoomDoor& operator=(const BossRoomDoor& _Other) = delete;
	BossRoomDoor& operator=(BossRoomDoor&& _Other) noexcept = delete;
	inline void DownAni()
	{
		if (DoorRenderer != nullptr)
		{
			DoorRenderer->ChangeFrameAnimation("Down");
			DoorRenderer->ScaleToCutTexture(0);
		}

	}
protected:
	void Start() override;
	void Update(float _DeltaTime);
	void End() {}

private:

	GameEngineTextureRenderer* DoorRenderer;
	GameEngineCollision* DoorCol;
};


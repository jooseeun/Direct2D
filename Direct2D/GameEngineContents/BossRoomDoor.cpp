#include "BossRoomDoor.h"

BossRoomDoor::BossRoomDoor() 
	:DoorRenderer(nullptr)
	,DoorCol(nullptr)
{
}

BossRoomDoor::~BossRoomDoor() 
{
}

void BossRoomDoor::Start()
{
	{
		DoorRenderer = CreateComponent< GameEngineTextureRenderer>();
		DoorRenderer->SetOrder((int)OBJECTORDER::FrontObject);
		DoorRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		DoorCol = CreateComponent<GameEngineCollision>();
		DoorCol->GetTransform().SetLocalScale({ 106,365,100 });
		DoorCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,15,0 });
		DoorCol->ChangeOrder((int)(OBJECTORDER::StopObject));
	}

	DoorRenderer->CreateFrameAnimationCutTexture("Up",
		FrameAnimation_DESC("Ruins Lift Cln_ornate_door0001-Sheet.png", 0, 6, 0.1f, false));
	DoorRenderer->CreateFrameAnimationCutTexture("Down",
		FrameAnimation_DESC("Ruins Lift Cln_ornate_door0000-Sheet.png", 0, 6, 0.1f, false));
	DoorRenderer->ChangeFrameAnimation("Up");
	DoorRenderer->ScaleToCutTexture(0);

	GameEngineSound::SoundPlayControl("bell_rise.wav");

}

void BossRoomDoor::Update(float _DeltaTime)
{
	DoorRenderer->AnimationBindEnd("Down", [=](const FrameAnimation_DESC& _Info)
	{
			GameEngineSound::SoundPlayControl("bell_rise.wav");
		Death();
	});
}

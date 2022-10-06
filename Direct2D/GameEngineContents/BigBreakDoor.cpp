#include "BigBreakDoor.h"
#include "PreCompile.h"
#include "Player.h"

BigBreakDoor::BigBreakDoor()
	:DoorRenderer(nullptr)
	, DoorCollision(nullptr)
	, HP(0)
	, Check(false)
	, Time(-1.0f)
{
}

BigBreakDoor::~BigBreakDoor() 
{
}

void BigBreakDoor::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		DoorRenderer = CreateComponent< GameEngineTextureRenderer>();
		DoorRenderer->SetTexture("Tute Great Door_door_v01-Sheet.png",0);
		DoorRenderer->ScaleToCutTexture(0);
		DoorRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		DoorCollision = CreateComponent<GameEngineCollision>();
		DoorCollision->GetTransform().SetLocalScale({ 300,1061,1000.0f });
		DoorCollision->GetTransform().SetLocalPosition({ 0,300,0 });
		DoorCollision->ChangeOrder((int)(OBJECTORDER::StopObject));
	}
}

void BigBreakDoor::Update(float _DeltaTime)
{

	DoorCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&BigBreakDoor::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);


	if (Time > 0.0f)
	{
		Time -= 1.0 * GameEngineTime::GetDeltaTime();

	}
	else
	{
		Check = false;
	}
}

CollisionReturn BigBreakDoor::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{

	if (Check == false)
	{
		Player::GetMainPlayer()->ShakeTimeReset();
		HP += 1;

		if (HP == 3)
		{
			SoundPlayer.Stop();
			SoundPlayer = GameEngineSound::SoundPlayControl("break_wall_after_tutorial_area.wav");
			Death();
			return CollisionReturn::Break;
		}
		SoundPlayer.Stop();
		SoundPlayer = GameEngineSound::SoundPlayControl("cave_in_rock_hit.wav");
		DoorRenderer->SetTexture("Tute Great Door_door_v01-Sheet.png", HP);
		DoorRenderer->ScaleToCutTexture(0);
		DoorRenderer->SetPivot(PIVOTMODE::BOT);
		Time = 1.0f;
		Check = true;
	}






	return CollisionReturn::ContinueCheck;
}
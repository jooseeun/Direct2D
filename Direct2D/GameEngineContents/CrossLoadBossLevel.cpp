#include "CrossLoadBossLevel.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "BasicZombie.h"
#include "HornZombie.h"
#include "FalseKnight.h"
#include "BossRoomDoor.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoadBossLevel::CrossLoadBossLevel()
	: Camera(nullptr)
	, MapSize()
	, RightDoor(nullptr)
	, LeftDoor(nullptr)
	, Zombie1(nullptr)
	, Zombie2(nullptr)
	, Zombie3(nullptr)
	, Boss(nullptr)
	, IsBoss(false)
	, IsClear(false)
{
}

CrossLoadBossLevel::~CrossLoadBossLevel() 
{
}

void CrossLoadBossLevel::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff")
		&& false == GameEngineInput::GetInst()->IsKey("CameraDebug"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
		GameEngineInput::GetInst()->CreateKey("CameraDebug", 'P');
	}

	{
		Camera = CreateActor<GameEngineCameraActor>();
		Camera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
		Camera->GetTransform().SetLocalPosition({ 0, 0, 0 });


		GetMainCamera()->SetProjectionSize(float4{ 1920 * 0.95 , 1080 * 0.95 });
		GetUICamera()->SetProjectionSize(float4{ 1920, 1080 });
	}


	{
		CreateMap("CrossBoss_BackGround.png",
			"CrossBoss_BackObject.png",
			"CrossBoss_Ground.png",
			"CrossBoss_FrontObject.png",
			"CrossBoss_ColMap.png");

		MapSize = { 4878, 1680, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 250,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 4831,-1390,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross6";
	}
	{
		Zombie1 = CreateActor<HornZombie>(OBJECTORDER::Monster);
		Zombie1->GetTransform().SetLocalPosition({ 2100, -1390, 0 });
		Zombie1->SetMonsterDir(MonsterDIR::Left);
	}
	{
		Zombie2 = CreateActor<BasicZombie>(OBJECTORDER::Monster);
		Zombie2->GetTransform().SetLocalPosition({ 1553, -1390, 0 });
		Zombie2->SetMonsterDir(MonsterDIR::Left);
	}
	{
		Zombie3 = CreateActor<BasicZombie>(OBJECTORDER::Monster);
		Zombie3->GetTransform().SetLocalPosition({ 1800, -1390, 0 });
		Zombie3->SetMonsterDir(MonsterDIR::Right);
	}


}
void CrossLoadBossLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
	if (GameEngineInput::GetInst()->IsDown("CameraDebug"))
	{
		SetMapONOFF();
	}
	if (IsClear == true)
	{
		return;
	}
	if (Zombie1->IsDeathReturn() == true &&
		Zombie2->IsDeathReturn() == true &&
		Zombie3->IsDeathReturn() == true)
	{

		if (IsBoss == false)
		{
			Player::GetMainBGM().Stop();
			Player::GetMainBGM() = GameEngineSound::SoundPlayControl("False Knight - Boss Battle 1.wav", 3);

			Boss = CreateActor<FalseKnight>(OBJECTORDER::Monster);
			Boss->GetTransform().SetLocalPosition({ 2457, -700, 0 });
			Boss->GetTransform().SetLocalScale({ 1.2, 1.2, 1.2 });

			RightDoor = CreateActor<BossRoomDoor>(OBJECTORDER::StopObject);
			RightDoor->GetTransform().SetLocalPosition({ 3294, -1600, 0 });

			LeftDoor = CreateActor<BossRoomDoor>(OBJECTORDER::StopObject);
			LeftDoor->GetTransform().SetLocalPosition({ 1138, -1600, 0 });

			IsBoss = true;
		}

		if (Boss->IsDeathReturn()&&IsBoss == true)
		{
			RightDoor->DownAni();
			LeftDoor->DownAni();
			IsClear = true;
			Player::GetMainBGM().Stop();
			Player::GetMainBGM() = GameEngineSound::SoundPlayControl("Forgotten Crossroads bass - S19 Crossroads Bass.wav", 1000);
		}
	}
}
void CrossLoadBossLevel::End()
{

}
void CrossLoadBossLevel::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 4371, -1555, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 4371, -1555, 0 });
		Player::GetMainPlayer()->SetCurLevelName(GetNameConstRef());
	}

	{
		if (nullptr == TopUI::GetMainTopUI())
		{
			TopUI* NewUI = CreateActor<TopUI>(OBJECTORDER::UI);
		}
		TopUI::GetMainTopUI()->SetLevelOverOn();
		TopUI::GetMainTopUI()->LevelStartUpdate();
	}
}
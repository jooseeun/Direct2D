#include "CrossLoad1.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "BasicZombie.h"
#include "Crawler.h"
#include "Climber.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad1::CrossLoad1() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad1::~CrossLoad1()
{
}

void CrossLoad1::Start()
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
		CreateMap("Cross1_BackGround.png",
			"Cross1_BackObject.png",
			"Cross1_Ground.png",
			"Cross1_FrontObject.png",
			"Cross1_ColMap.png");

		MapSize = { 6633, 1661, 100.0f };
	}
	{
		BasicZombie* Monster = CreateActor<BasicZombie>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 3150, -1551, 0 });
		Monster->SetMonsterDir(MonsterDIR::Left);
	}
	{
		BasicZombie* Monster = CreateActor<BasicZombie>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 5732, -1394, 0 });
		Monster->SetMonsterDir(MonsterDIR::Left);
	}
	{
		Crawler* Monster = CreateActor<Crawler>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 551, -1435, 0 });
	}
	{
		Climber* Monster = CreateActor<Climber>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 5045, -940, 0 });
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 6600,-818,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross2";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 0,-1200,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel2 = "Cross10";

		MapMoveCol->MoveCol3->GetTransform().SetLocalScale({ 200,50,1000.0f });
		MapMoveCol->MoveCol3->GetTransform().SetLocalPosition({ 3470,-600,100 });
		MapMoveCol->MoveCol3->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel3 = "Town3";
	}
}
void CrossLoad1::Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
	if (GameEngineInput::GetInst()->IsDown("CameraDebug"))
	{
		SetMapONOFF();
	}
}
void CrossLoad1::End()
{

}

void CrossLoad1::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}

		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS2")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 6400, -850, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 6400, -850, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS10")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 100, -1250, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 100, -1250, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 3469, -800, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 3469, -342, 0 });
		}

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
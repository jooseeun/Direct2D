#include "CrossLoad3.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "BasicZombie.h"
#include "HornZombie.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad3::CrossLoad3() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad3::~CrossLoad3()
{
}

void CrossLoad3::Start()
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
		CreateMap("Cross3_BackGround.png",
			"Cross3_BackObject.png",
			"Cross3_Ground.png",
			"Cross3_FrontObject.png",
			"Cross3_ColMap.png");

		MapSize = { 5856, 1080, 100.0f };
	}
	{
		BasicZombie* Monster = CreateActor<BasicZombie>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 2125, -931, 0 });
		Monster->SetMonsterDir(MonsterDIR::Left);
	}

	{
		HornZombie* Monster = CreateActor<HornZombie>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 4276, -929, 0 });
		Monster->SetMonsterDir(MonsterDIR::Left);
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 10,-928,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross2";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 5800,-928,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel2 = "Cross4";
	}
}
void CrossLoad3::Update(float _DeltaTime)
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
void CrossLoad3::End()
{

}

void CrossLoad3::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS2")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 250, -850, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 250, -850, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS4")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 5600, -850, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 5600, -850, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 250, -850, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 250, -850, 0 });
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
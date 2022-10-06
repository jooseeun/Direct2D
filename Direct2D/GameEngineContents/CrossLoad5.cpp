#include "CrossLoad5.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "Buzzer.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad5::CrossLoad5() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad5::~CrossLoad5()
{
}

void CrossLoad5::Start()
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
		CreateMap("Cross5_BackGround.png",
			"Cross5_BackObject.png",
			"Cross5_Ground.png",
			"Cross5_FrontObject.png",
			"Cross5_ColMap.png");

		MapSize = { 2836, 1798, 100.0f };
	}
	{
		Buzzer* Monster = CreateActor<Buzzer>(OBJECTORDER::Monster);
		Monster->GetTransform().SetLocalPosition({ 1873, -552, 0 });
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 2821,-945,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross4";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 200,100,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 1639,-1782,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel2 = "Cross6";
	}
}
void CrossLoad5::Update(float _DeltaTime)
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
void CrossLoad5::End()
{

}

void CrossLoad5::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS4")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 2651, -890, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 2651, -890, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS6")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 1881, -1623, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 1881, -1623, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 2651, -890, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 2651, -890, 0 });
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
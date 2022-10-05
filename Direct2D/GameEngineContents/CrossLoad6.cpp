#include "CrossLoad6.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad6::CrossLoad6() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad6::~CrossLoad6()
{
}

void CrossLoad6::Start()
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
		CreateMap("Cross6_BackGround.png",
			"Cross6_BackObject.png",
			"Cross6_Ground.png",
			"Cross6_FrontObject.png",
			"Cross6_ColMap.png");

		MapSize = { 1994, 4782, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 200,50,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 926,0,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross5";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 0,-4318,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol2);
		MapMoveCol->MoveLevel2 = "Cross7";

		MapMoveCol->MoveCol3->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol3->GetTransform().SetLocalPosition({ 50,-2792,100 });
		MapMoveCol->MoveCol3->ChangeOrder(OBJECTORDER::MoveCol3);
		MapMoveCol->MoveLevel3 = "CrossLoadBoss";
	}
}
void CrossLoad6::Update(float _DeltaTime)
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
void CrossLoad6::End()
{

}

void CrossLoad6::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS5")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 926, -300, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 926, -300, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS7")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 150, -4218, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 150, -2091, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSSLOADBOSS")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 170, -2692, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 170, -2692, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 926, -300, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 926, -300, 0 });
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
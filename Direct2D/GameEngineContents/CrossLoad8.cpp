#include "CrossLoad8.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad8::CrossLoad8() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad8::~CrossLoad8()
{
}

void CrossLoad8::Start()
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
		CreateMap("Cross8_BackGround.png",
			"Cross8_BackObject.png",
			"Cross8_Ground.png",
			"Cross8_FrontObject.png",
			"Cross8_ColMap.png");

		MapSize = { 3476, 2708, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 3430,-2392,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross7";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 0,-1522,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel2 = "Cross9";
	}
}
void CrossLoad8::Update(float _DeltaTime)
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
void CrossLoad8::End()
{

}

void CrossLoad8::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS7")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 3200, -2250, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 3200, -2250, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS9")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 150, -1400, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 150, -1400, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 3200, -2250, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 3200, -2250, 0 });
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
void CrossLoad8::LevelEndEvent()
{

}
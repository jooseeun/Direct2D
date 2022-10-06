#include "CrossLoad10.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

CrossLoad10::CrossLoad10() :
	Camera(nullptr),
	MapSize()
{
}

CrossLoad10::~CrossLoad10()
{
}

void CrossLoad10::Start()
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
		CreateMap("Cross10_BackGround.png",
			"Cross10_BackObject.png",
			"Cross10_Ground.png",
			"Cross10_FrontObject.png",
			"Cross10_ColMap.png");

		MapSize = { 2826, 4675, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 150,80,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 1284,-4660,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Cross9";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 50,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 2814,-965,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel2 = "Cross1";
	}
}
void CrossLoad10::Update(float _DeltaTime)
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
void CrossLoad10::End()
{

}

void CrossLoad10::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS9")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 1519, -4410, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 1519, -4410, 0 });
		}
		else if (Player::GetMainPlayer()->GetCurLevelName() == "CROSS1")
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 2654, -750, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 2654, -750, 0 });
		}
		else
		{
			Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 1519, -4410, 0 });
			Player::GetMainPlayer()->SetLevelOverOn();
			Player::GetMainPlayer()->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 1519, -4410, 0 });
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
void CrossLoad10::LevelEndEvent()
{

}
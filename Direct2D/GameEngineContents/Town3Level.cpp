#include "Town3Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "MapMoveCollision.h"
#include "PlayLevelManager.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Town3Level::Town3Level() :
	Camera(nullptr),
	MapSize()
{
}

Town3Level::~Town3Level()
{
}

void Town3Level::Start()
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


		GetMainCamera()->SetProjectionSize(float4{ 1920 * 0.85 , 1080 * 0.85 });
		GetUICamera()->SetProjectionSize(float4{ 1920 * 0.85 , 1080 * 0.85 });
	}

	{
		CreateMap("Dirtmouth_Background_3.png",
			"Dirtmouth_Background_Obj_3.png",
			"Dirtmouth_Ground_3.png",
			"Dirtmouth_FrontObject_3.png",
			"Dirtmouth_ColMap_3.png");

		MapSize = { 6468, 3600, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 25,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 10,-3021,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Town2";
	}
}
void Town3Level::Update(float _DeltaTime)
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
void Town3Level::End()
{

}

void Town3Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 101, -3021, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 101, -3021, 0 });
	}

	{
		if (nullptr == TopUI::GetMainTopUI())
		{
			TopUI* NewUI = CreateActor<TopUI>(OBJECTORDER::UI);
		}
		TopUI::GetMainTopUI()->SetLevelOverOn();
	}
}
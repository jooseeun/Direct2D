#include "Town2Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "TownFontUI.h"

#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Town2Level::Town2Level() :
	Camera(nullptr),
	MapSize()
{
}

Town2Level::~Town2Level()
{
}

void Town2Level::Start()
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
		CreateMap("Dirtmouth_Background_2.png",
			"Dirtmouth_Background_Obj_2.png",
			"Dirtmouth_Ground_2.png",
			"Dirtmouth_FrontObject_2.png",
			"Dirtmouth_ColMap_2.png");

		MapSize = { 6646, 3400, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 25,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 10,-3065,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Town1";

		MapMoveCol->MoveCol2->GetTransform().SetLocalScale({ 250,300,1000.0f });
		MapMoveCol->MoveCol2->GetTransform().SetLocalPosition({ 6505,-3065,100 });
		MapMoveCol->MoveCol2->ChangeOrder(OBJECTORDER::MoveCol2);
		MapMoveCol->MoveLevel2 = "Town3";
	}

	{
		TownFontUI* NewTownFontUI = CreateActor<TownFontUI>(OBJECTORDER::UI);
	}

}
void Town2Level::Update(float _DeltaTime)
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
void Town2Level::End()
{

}

void Town2Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 200,-3065,0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 200,-3065,0 });
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
#include "Tutorial2Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Tutorial2Level::Tutorial2Level() :
	Camera(nullptr),
	MapSize()
{
}

Tutorial2Level::~Tutorial2Level()
{
}

void Tutorial2Level::Start()
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

		CreateMap("King's-Pass_Background_2.png",
			"King's-Pass_Background_Object_2.png",
			"King's-Pass_Ground_2.png",
			"King's-Pass_FrontObject_2.png",
			"King's-Pass_ColMap_2.png");

		MapSize = { 3888.0f, 2546.0f, 100.0f };
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 250,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 124,-326,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Tutorial3";
	}
}
void Tutorial2Level::Update(float _DeltaTime)
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
void Tutorial2Level::End()
{

}

void Tutorial2Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 632, -1734, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 632, -1734, 0 });
	}

}
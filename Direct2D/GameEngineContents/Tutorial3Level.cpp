#include "Tutorial3Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "Buzzer.h"
#include "Crawler.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "ChageExplanation.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Tutorial3Level::Tutorial3Level() :
	Camera(nullptr),
	MapSize()
{
}

Tutorial3Level::~Tutorial3Level()
{
}

void Tutorial3Level::Start()
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
		GetUICamera()->SetProjectionSize(float4{ 1920, 1080 });
	}

	{

		CreateMap("King's-Pass_Background_3.png",
			"King's-Pass_Background_Object_3.png",
			"King's-Pass_Ground_3.png",
			"King's-Pass_FrontObject_3.png",
			"King's-Pass_ColMap_3.png");

		MapSize = { 5183, 3947, 100.0f };
	}
	{
		Buzzer* NewBuzzer = CreateActor<Buzzer>(OBJECTORDER::Monster);
		NewBuzzer->GetTransform().SetLocalPosition({2262, -3326, 0 });
	}
	{
		ChageExplanation* NewObject = CreateActor<ChageExplanation>(OBJECTORDER::BackGroundObject);
		NewObject->GetTransform().SetLocalPosition({4261, -3217, 50 });
	}
	{
		Crawler* NewCrawler = CreateActor<Crawler>(OBJECTORDER::Monster);
		NewCrawler->GetTransform().SetLocalPosition({2417 , -1140, 0 });
	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 250,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 3511,-1154,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Tutorial4";
	}
}
void Tutorial3Level::Update(float _DeltaTime)
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
void Tutorial3Level::End()
{

}

void Tutorial3Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 4948, -3080, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 4948, -3080, 0 });
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
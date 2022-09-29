#include "Tutorial4Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "TopUI.h"
#include "FallenGround.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "ThornTrap.h"
#include "BigBreakDoor.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Tutorial4Level::Tutorial4Level() :
	Camera(nullptr),
	MapSize()
{
}

Tutorial4Level::~Tutorial4Level()
{
}

void Tutorial4Level::Start()
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
		CreateMap("King's-Pass_Background_4.png",
			"King's-Pass_Background_Object_4.png",
			"King's-Pass_Ground_4.png",
			"King's-Pass_FrontObject_4.png",
			"King's-Pass_ColMap_4.png");

		MapSize = { 6210, 3271, 100.0f };
	}
	{
		FallenGround* fallG = CreateActor<FallenGround>(OBJECTORDER::FrontObject);
		fallG->GetTransform().SetLocalPosition({ 1645, -1620,  -250});
	}
	{
		BigBreakDoor* Door = CreateActor<BigBreakDoor>(OBJECTORDER::FrontObject);
		Door->GetTransform().SetLocalPosition({ 5605, -1310,  0 });
	}
	{
		for (int i = 0; i < 7; i++)
		{
			ThornTrap* ThornTrap_ = CreateActor<ThornTrap>(OBJECTORDER::FrontObject);
			ThornTrap_->GetTransform().SetLocalPosition({ 2711.0f + i * 140, -2926, 0 });
		}
		

	}
	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 250,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 6009,-1110,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Town1";
	}
}
void Tutorial4Level::Update(float _DeltaTime)
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
void Tutorial4Level::End()
{

}

void Tutorial4Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 290, -1080, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		//Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 2420, -2500, 0 });
		//Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 4586, -999, 0 });
		Player::GetMainPlayer()->SetTrapStunPos({ 2420, -2500, 0 });
		GetMainCameraActorTransform().SetLocalPosition({ 290, -1080, 0 });
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
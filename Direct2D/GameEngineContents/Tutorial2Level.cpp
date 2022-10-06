#include "Tutorial2Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "PlayLevelManager.h"
#include "MapMoveCollision.h"
#include "TopUI.h"
#include "Crawler.h"
#include "BugGlow.h"

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

		GetMainCamera()->SetProjectionSize(float4{ 1920 * 0.95 , 1080 * 0.95 });
		GetUICamera()->SetProjectionSize(float4{ 1920, 1080 });
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
		BugGlow* NewBugGlow = CreateActor<BugGlow>(OBJECTORDER::BackGroundObject);
		NewBugGlow->GetTransform().SetLocalPosition({ 1651, -1401, 50 });
	}
	{
		BugGlow* NewBugGlow = CreateActor<BugGlow>(OBJECTORDER::BackGroundObject);
		NewBugGlow->GetTransform().SetLocalPosition({ 2009, -1725, 50 });
	}
	{
		BugGlow* NewBugGlow = CreateActor<BugGlow>(OBJECTORDER::BackGroundObject);
		NewBugGlow->GetTransform().SetLocalPosition({ 1665, -1993, 50 });
	}
	{
		BugGlow* NewBugGlow = CreateActor<BugGlow>(OBJECTORDER::BackGroundObject);
		NewBugGlow->GetTransform().SetLocalPosition({ 1325, -1613, 50 });
	}
	{
		BugGlow* NewBugGlow = CreateActor<BugGlow>(OBJECTORDER::BackGroundObject);
		NewBugGlow->GetTransform().SetLocalPosition({ 1329, -1159, 50 });
	}
	{
		Crawler* NewCrawler = CreateActor<Crawler>(OBJECTORDER::Monster);
		NewCrawler->GetTransform().SetLocalPosition({ 2193, -2125, 0 });
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
	{
		if (nullptr == TopUI::GetMainTopUI())
		{
			TopUI* NewUI = CreateActor<TopUI>(OBJECTORDER::UI);
		}
		TopUI::GetMainTopUI()->SetLevelOverOn();
		TopUI::GetMainTopUI()->LevelStartUpdate();
	}
	BgmPlayer = GameEngineSound::SoundPlayControl("waterways_atmos_loop.wav", 1000);
}
void Tutorial2Level::LevelEndEvent()
{
	BgmPlayer.Stop();
}
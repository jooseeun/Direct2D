#include "Tutorial1Level.h"
#include "PreCompile.h"
#include "MapMoveCollision.h"
#include "SmallBreakDoor.h"
#include "MapSet.h"
#include "Player.h"
#include "Crawler.h"
#include "TopUI.h"

#include "PlayLevelManager.h"

#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>

Tutorial1Level::Tutorial1Level() :
	Camera(nullptr),
	MapSize()
{
}

Tutorial1Level::~Tutorial1Level() 
{
}

void Tutorial1Level::Start()
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
		Camera->GetTransform().SetLocalPosition( { 0, 0, 0 });

		GetMainCamera()->SetProjectionSize(float4{ 1920 * 0.95 , 1080 * 0.95 });
		GetUICamera()->SetProjectionSize(float4{ 1920, 1080});
	}
	

	{

		CreateMap("King's-Pass_Background_1.png",
			"King's-Pass_Background_Object_1.png",
			"King's-Pass_Ground_1.png",
			"King's-Pass_FrontObject_1.png",
			"King's-Pass_ColMap_1.png");

		MapSize = { 7098.0f, 5000.0f, 100.0f };
	}

	{
		Crawler* NewCrawler = CreateActor<Crawler>(OBJECTORDER::Monster);
		NewCrawler->GetTransform().SetLocalPosition({ 4709, -4653, 0 });
	}

	{
		Crawler* NewCrawler = CreateActor<Crawler>(OBJECTORDER::Monster);
		NewCrawler->GetTransform().SetLocalPosition({ 6700, -4653, 0 });
	}

	{
		MapMoveCollision* MapMoveCol = CreateActor<MapMoveCollision>(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveCol1->GetTransform().SetLocalScale({ 200,300,1000.0f });
		MapMoveCol->MoveCol1->GetTransform().SetLocalPosition({ 7000,-4527,100 });
		MapMoveCol->MoveCol1->ChangeOrder(OBJECTORDER::MoveCol1);
		MapMoveCol->MoveLevel1 = "Tutorial2";
	}

	{

		SmallBreakDoor * Door1 = CreateActor<SmallBreakDoor>(OBJECTORDER::FrontObject);
		Door1->GetTransform().SetLocalPosition({ 2420, -4660, 0 });
	}
	{

		SmallBreakDoor* Door2 = CreateActor<SmallBreakDoor>(OBJECTORDER::FrontObject);
		Door2->GetTransform().SetLocalPosition({ 3256, -4660, 0 });
	}
	{

		SmallBreakDoor* Door3 = CreateActor<SmallBreakDoor>(OBJECTORDER::FrontObject);
		Door3->GetTransform().SetLocalPosition({ 4076, -4660, 0 });
	}
	{

		SmallBreakDoor* Door4 = CreateActor<SmallBreakDoor>(OBJECTORDER::FrontObject);
		Door4->GetTransform().SetLocalPosition({ 5241, -4660, 0 });
	}


}
void Tutorial1Level::Update(float _DeltaTime)
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
void Tutorial1Level::End()
{

}

void Tutorial1Level::LevelStartEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
		}
		Player::GetMainPlayer()->GetTransform().SetLocalPosition({ 924.0f, -1640.0f, 0 });
		Player::GetMainPlayer()->SetLevelOverOn();
		Player::GetMainPlayer()->SetMapSize(MapSize);
		GetMainCameraActorTransform().SetLocalPosition({ 924.0f, -1640.0f, 0 });
	}
	if (nullptr == TopUI::GetMainTopUI())
	{
		TopUI* NewUI = CreateActor<TopUI>(OBJECTORDER::UI);
	}
	TopUI::GetMainTopUI()->SetLevelOverOn();
	BgmPlayer = GameEngineSound::SoundPlayControl("waterways_atmos_loop.wav", 1000);
}

void Tutorial1Level::LevelEndEvent()
{
	BgmPlayer.Stop();
}
#include "Tutorial1Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
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


		GetMainCamera()->SetProjectionSize(float4{ 1920*0.9 , 1080 * 0.9 });
	}

	{

		CreateMap("King's-Pass_Background_1.png",
			"King's-Pass_Background_Object_1.png",
			"King's-Pass_Ground_1.png",
			"King's-Pass_FrontObject_1.png",
			"King's-Pass_ColMap_1.png");

		MapSize = { 7098.0f, 4800.0f, 100.0f };
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

void Tutorial1Level::OnEvent()
{
	{
		if (nullptr == Player::GetMainPlayer())
		{
			Player* NewPlayer = CreateActor<Player>(OBJECTORDER::Player);
			NewPlayer->GetTransform().SetLocalPosition({ 924.0f, -1640.0f, 0 });
			NewPlayer->SetLevelOverOn();
			NewPlayer->SetMapSize(MapSize);
			GetMainCameraActorTransform().SetLocalPosition({ 924.0f, -1640.0f, 0 });
		}
	}

}
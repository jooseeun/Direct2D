#include "Tutorial3Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include "PlayLevelManager.h"
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
	}

	{

		CreateMap("King's-Pass_Background_3.png",
			"King's-Pass_Background_Object_3.png",
			"King's-Pass_Ground_3.png",
			"King's-Pass_FrontObject_3.png",
			"King's-Pass_ColMap_3.png");

		MapSize = { 5183, 3947, 100.0f };
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

void Tutorial3Level::OnEvent()
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

}
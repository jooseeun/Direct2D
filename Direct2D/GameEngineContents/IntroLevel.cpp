#include "IntroLevel.h"
#include "PreCompile.h"
#include "TitleObject.h"
#include "Mouse.h"
#include "ColorOverlay.h"
#include "TitleEffect.h"
#include <GameEngineCore/GameEngineBlur.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

IntroLevel::IntroLevel() 
	:Time(0.0f)
{
}

IntroLevel::~IntroLevel() 
{
}

void IntroLevel::Start()
{
	GameEngineDevice::GetBackBuffer()->AddEffect<GameEngineBlur>();
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<GameEngineBlur>();

	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff")
		&& false == GameEngineInput::GetInst()->IsKey("CameraDebug"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
		GameEngineInput::GetInst()->CreateKey("CameraDebug", 'P');
	}


	TitleCom = CreateActor<TitleObject>();

	{
		Camera = CreateActor<GameEngineCameraActor>();
		Camera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
		Camera->GetTransform().SetLocalPosition({ 960.0f, -900.0f, 100.0f });

		GetMainCamera()->SetProjectionSize(float4{ 1920 , 1080 });
		GetUICamera()->SetProjectionSize(float4{ 1920, 1080 });
	}
	for (int i = 0; i < 6; i++)
	{
		TitleEffect* Effect = CreateActor<TitleEffect>(OBJECTORDER::UI);
	}

}
void IntroLevel::Update(float _DeltaTime)
{
	
	if (GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}

	Time += 1.0f * _DeltaTime;
	if (Time > 1.0f)
	{
		TitleEffect* Effect1 = CreateActor<TitleEffect>(OBJECTORDER::UI);
		TitleEffect* Effect2 = CreateActor<TitleEffect>(OBJECTORDER::UI);
		Time = 0.0f;
	}

}
void IntroLevel::End()
{
}

void IntroLevel::LevelStartEvent()
{

	BgmPlayer = GameEngineSound::SoundPlayControl("Main menu theme - Title.wav");
	if (nullptr == Mouse::MainMouse)
	{
		Mouse* MainMouse = CreateActor<Mouse>(OBJECTORDER::UI);
	}

	GetMainCameraActorTransform().SetLocalPosition({ 960.0f, -900.0f, 0.0f });
}

void IntroLevel::LevelEndEvent()
{
	BgmPlayer.Stop();
}
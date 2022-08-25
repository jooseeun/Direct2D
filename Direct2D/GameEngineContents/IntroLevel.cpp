#include "IntroLevel.h"
#include "PreCompile.h"
#include "TitleObject.h"
#include "Mouse.h"
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineBase/GameEngineInput.h>

IntroLevel::IntroLevel() 
{
}

IntroLevel::~IntroLevel() 
{
}

void IntroLevel::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff")
		&& false == GameEngineInput::GetInst()->IsKey("CameraDebug"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
		GameEngineInput::GetInst()->CreateKey("CameraDebug", 'P');
		GameEngineInput::GetInst()->CreateKey("GameStart", VK_SPACE);
	}


	TitleCom = CreateActor<TitleObject>();

	{
		Camera = CreateActor<GameEngineCameraActor>();
		Camera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
		Camera->GetTransform().SetLocalPosition({ 1024.0f, -900.0f, 100.0f });

		GetMainCamera()->SetProjectionSize(float4{ 1920 , 1080 });
		GetUICamera()->SetProjectionSize(float4{ 1920* 0.7, 1080 * 0.7 });
	}


}
void IntroLevel::Update(float _DeltaTime)
{
	
	if (GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
	if (GameEngineInput::GetInst()->IsDown("GameStart"))
	{
		GEngine::ChangeLevel("Tutorial1");
	}

}
void IntroLevel::End()
{

}

void IntroLevel::LevelStartEvent()
{
	if (nullptr == Mouse::MainMouse)
	{
		Mouse* MainMouse = CreateActor<Mouse>(OBJECTORDER::UI);
	}

	GetMainCameraActorTransform().SetLocalPosition({ 1024.0f, -900.0f, 0.0f });
}
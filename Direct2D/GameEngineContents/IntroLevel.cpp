#include "IntroLevel.h"
#include "PreCompile.h"
#include "TitleObject.h"
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
	}


	TitleCom = CreateActor<TitleObject>();


	{
		Camera = CreateActor<GameEngineCameraActor>();
		Camera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		Camera->GetTransform().SetLocalPosition({ 0, 0, 0 });
	}


	Camera->GetCameraComponent()->SetProjectionSize(float4{ 1980,1080 }*0.6);

}
void IntroLevel::Update(float _DeltaTime)
{
	
	if (GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}


}
void IntroLevel::End()
{

}

void IntroLevel::OnEvent()
{

	GetMainCameraActorTransform().SetLocalPosition({ 1024.0f, -900.0f, 0.0f });
}
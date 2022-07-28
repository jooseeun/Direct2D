#include "Tutorial1Level.h"
#include "PreCompile.h"
#include "Player.h"
#include "MapSet.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineCameraActor.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>
Tutorial1Level::Tutorial1Level() 
{
}

Tutorial1Level::~Tutorial1Level() 
{
}

void Tutorial1Level::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("FreeCameaOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameaOnOff", 'O');
	}

	{
		Camera = CreateActor<GameEngineCameraActor>();
		Camera->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
		Camera->GetTransform().SetLocalPosition( {0.0f,0.0f,0.0f});
	
	}

	{
		MapSet* Map = CreateActor<MapSet>(OBJECTORDER::Ground);
		Map->BackGround->GetTransform().SetLocalScale({ 7098.0f, 5322.0f, 100.0f });
		Map->BackGround->SetPivot(PIVOTMODE::LEFTTOP);
		Map->BackGround->SetTexture("King's-Pass_Background_1.png");
		Map->BackGround->SetOrder((int)OBJECTORDER::BackGround);
		Map->BackObject->GetTransform().SetLocalScale({ 7098.0f, 5322.0f, 100.0f });
		Map->BackObject->SetPivot(PIVOTMODE::LEFTTOP);
		Map->BackObject->SetTexture("King's-Pass_Background_Object_1.png");
		Map->BackObject->SetOrder((int)OBJECTORDER::BackObject);
		Map->Ground->GetTransform().SetLocalScale({ 7098.0f, 5322.0f, 100.0f });
		Map->Ground->SetPivot(PIVOTMODE::LEFTTOP);
		Map->Ground->SetTexture("King's-Pass_Terrain_1.png");
		Map->Ground->SetOrder((int)OBJECTORDER::Ground);
		Map->FrontObject->GetTransform().SetLocalScale({ 7098.0f, 5322.0f, 100.0f });
		Map->FrontObject->GetTransform().SetLocalPosition({ 0, 0, -10 });
		Map->FrontObject->SetPivot(PIVOTMODE::LEFTTOP);
		Map->FrontObject->SetTexture("King's-Pass_FrontObject_1.png");
		Map->FrontObject->SetOrder((int)OBJECTORDER::FrontObject);
	}
}
void Tutorial1Level::Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst()->IsDown("FreeCameaOnOff"))
	{
		// ;
		GetMainCameraActor()->FreeCameraModeOnOff();
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
			NewPlayer->GetTransform().SetLocalPosition({ 924.0f, -4640.0f, 0 });
			NewPlayer->SetLevelOverOn();
		}
	}

}
#include "TownFontUI.h"
#include "PlayLevelManager.h"
#include "PreCompile.h"

TownFontUI::TownFontUI() 
	: Small(nullptr)
	, Big(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
	, Time(0.0f)
	, Alpha(1.0f)
{
}

TownFontUI::~TownFontUI() 
{
}

void TownFontUI::Start()
{

	{
		Top = CreateComponent<GameEngineUIRenderer>();
		Top->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Area Title Cln_fleur_top0000-Sheet.png", 2, 21, 0.1f, false));
		Top->ChangeFrameAnimation("Idle");
		Top->ScaleToCutTexture(0);
		Top->GetTransform().SetLocalPosition({ 0, 340, 0 });
		Top->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Bottom = CreateComponent<GameEngineUIRenderer>();
		Bottom->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Area Title Cln_fleur_bottom0002-Sheet.png", 0, 19, 0.1f, false));
		Bottom->ChangeFrameAnimation("Idle");
		Bottom->ScaleToCutTexture(0);
		Bottom->GetTransform().SetLocalPosition({ 0,20 ,0 });
		Bottom->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Big = CreateComponent<GameEngineFontRenderer>();
		Big->SetText("흙의 마을", "Noto Serif KR");
		Big->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Big->SetSize(60.0);
		Big->SetScreenPostion({ 1920*0.7/2 - 130.0f, 1080*0.7/2-220, 1 });
		Big->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Small = CreateComponent<GameEngineFontRenderer>();
		Small->SetText("사라져가는 땅의 입구", "Noto Serif KR");
		Small->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Small->SetSize(40.0);
		Small->SetScreenPostion({ 1920 * 0.7 / 2 - 190.0f, 1080 * 0.7 / 2-130, 1 });
		Small->ChangeCamera(CAMERAORDER::UICAMERA);
	}
}
void TownFontUI::Update(float _DeltaTime)
{
	Time += 1.0 * _DeltaTime;

	if (Time > 5.0f)
	{
		Top->GetPixelData().MulColor.a -= 0.2f * GameEngineTime::GetDeltaTime();
		Bottom->GetPixelData().MulColor.a -= 0.2f * GameEngineTime::GetDeltaTime();
		Alpha-= 0.2f * GameEngineTime::GetDeltaTime();
		Big->SetColor({ 1.0f, 1.0f, 1.0f, Alpha });
		Small->SetColor({ 1.0f, 1.0f, 1.0f, Alpha });
		if (Top->GetPixelData().MulColor.a <= 0.0f)
		{
			Top->GetPixelData().MulColor.a = 0.0f;
			Death();
		}
	}
}
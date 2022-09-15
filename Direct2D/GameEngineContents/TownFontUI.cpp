#include "TownFontUI.h"
#include "PlayLevelManager.h"
#include "PreCompile.h"

TownFontUI::TownFontUI() 
	: Small(nullptr)
	, Big(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
	, Time(0.0f)
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
		Big->SetText("���� ����", "Noto Serif KR");
		Big->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Big->SetSize(60.0);
		Big->SetScreenPostion({ 1920*0.7/2 - 130.0f, 1080*0.7/2-220, 1 });
		Big->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Small = CreateComponent<GameEngineFontRenderer>();
		Small->SetText("��������� ���� �Ա�", "Noto Serif KR");
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
		Death();
	}
}
#include "LorePromptFont.h"
#include "PlayLevelManager.h"
#include "PreCompile.h"

LorePromptFont* LorePromptFont::LorePrompt = nullptr;

LorePromptFont::LorePromptFont() 
	:BlackAlphaGround(nullptr)
	, LorePromptRenderer(nullptr)
	, TutorialRenderer(nullptr)
	, Font1(nullptr)
	, Font2(nullptr)
	, Font3(nullptr)
	, Font4(nullptr)
	, Font5(nullptr)
	, Font6(nullptr)
	, Font7(nullptr)
	, KeyRenderer(nullptr)
{
	LorePromptFont::LorePrompt = this;
}

LorePromptFont::~LorePromptFont() 
{
}

void LorePromptFont::Start()
{
	GameEngineSound::SoundPlayOneShot("Lore_Tablet_activate_temp.wav");
	SoundPlayer = GameEngineSound::SoundPlayControl("Lore_tablet_idle_audio_temp.wav",1000);
	if (false == GameEngineInput::GetInst()->IsKey("Exit"))
	{
		GameEngineInput::GetInst()->CreateKey("Exit", VK_UP);
	}

	{
		BlackAlphaGround = CreateComponent<GameEngineUIRenderer>();
		BlackAlphaGround->SetOrder((int)OBJECTORDER::Player);
		BlackAlphaGround->SetPivot(PIVOTMODE::BOT);
		BlackAlphaGround->SetTexture("AlphaBackGround.png");
		BlackAlphaGround->ScaleToTexture();
		BlackAlphaGround->GetTransform().SetLocalPosition({ 0, -1080 / 2, 0 });
		BlackAlphaGround->ChangeCamera(CAMERAORDER::UICAMERA);
		BlackAlphaGround->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		BlackAlphaGround->GetPixelData().MulColor.a = 0.8;
	}

	{
		LorePromptRenderer = CreateComponent<GameEngineUIRenderer>();
		LorePromptRenderer->SetOrder((int)OBJECTORDER::Player);
		LorePromptRenderer->SetPivot(PIVOTMODE::BOT);
		LorePromptRenderer->SetTexture("lore_prompt_new.png");
		LorePromptRenderer->GetTransform().SetLocalScale(LorePromptRenderer->GetCurTexture()->GetScale() * 1.5f);
		LorePromptRenderer->GetTransform().SetLocalPosition({ 0, -400.0f, 0 });
		LorePromptRenderer->ChangeCamera(CAMERAORDER::UICAMERA);
		LorePromptRenderer->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		LorePromptRenderer->GetPixelData().MulColor.a = 0.4f;
	}
	{
		Font1 = CreateComponent<GameEngineFontRenderer>();
		Font1->SetText("고귀한 존재들, 당신들을 위해 말한다.", "Noto Serif KR");
		Font1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font1->SetSize(33.0f);
		Font1->SetScreenPostion({ 1920 * 0.8 / 2- 240, 1080 * 0.8 / 2 - 330, 1 });
		Font1->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font2 = CreateComponent<GameEngineFontRenderer>();
		Font2->SetText("우리 일원이기에 위대한 힘이 있다. 영혼을 집중해 꿈만 같", "Noto Serif KR");
		Font2->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font2->SetSize(33.0f);
		Font2->SetScreenPostion({ 1920 * 0.8 / 2-380, 1080 * 0.8 / 2 - 230, 1 });
		Font2->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font3 = CreateComponent<GameEngineFontRenderer>();
		Font3->SetText("은 일을 이룰 수 있다.", "Noto Serif KR");
		Font3->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font3->SetSize(33.0f);
		Font3->SetScreenPostion({ 1920 * 0.8 / 2 - 120, 1080 * 0.8 / 2 - 180, 1 });
		Font3->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font4 = CreateComponent<GameEngineFontRenderer>();
		Font4->SetText("적을 공격하여 영혼을 모으십시오.", "Noto Serif KR");
		Font4->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font4->SetSize(25.0f);
		Font4->SetScreenPostion({ 1920 * 0.8 / 2 - 170, 1080 * 0.8 / 2 + 70, 1 });
		Font4->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font5 = CreateComponent<GameEngineFontRenderer>();
		Font5->SetText("충분한 영혼이 모이면.", "Noto Serif KR");
		Font5->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font5->SetSize(25.0f);
		Font5->SetScreenPostion({ 1920 * 0.8 / 2 - 110, 1080 * 0.8 / 2+110, 1 });
		Font5->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font6 = CreateComponent<GameEngineFontRenderer>();
		Font6->SetText("길게 누르기      A", "Noto Serif KR");
		Font6->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font6->SetSize(37.0f);
		Font6->SetScreenPostion({ 1920 * 0.8 / 2 - 165, 1080 * 0.8 / 2 + 156, 1 });
		Font6->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font7 = CreateComponent<GameEngineFontRenderer>();
		Font7->SetText("영혼을 집중해 치유할 수 있습니다.", "Noto Serif KR");
		Font7->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font7->SetSize(27.0f);
		Font7->SetScreenPostion({ 1920 * 0.8 / 2 - 180, 1080 * 0.8 / 2 + 235, 1 });
		Font7->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		KeyRenderer = CreateComponent<GameEngineUIRenderer>();
		KeyRenderer->SetOrder((int)OBJECTORDER::Player);
		KeyRenderer->SetPivot(PIVOTMODE::BOT);
		KeyRenderer->SetTexture("button_skin_0004_square_key.png");
		KeyRenderer->ScaleToTexture();
		KeyRenderer->GetTransform().SetLocalPosition({ 132, -285, 0 });
		KeyRenderer->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		TutorialRenderer = CreateComponent<GameEngineUIRenderer>();
		TutorialRenderer->SetOrder((int)OBJECTORDER::Player);
		TutorialRenderer->SetPivot(PIVOTMODE::BOT);
		TutorialRenderer->SetTexture("Focus_prompt_temp.png");
		TutorialRenderer->ScaleToTexture();
		TutorialRenderer->GetTransform().SetLocalPosition({ 0, -80, 0 });
		TutorialRenderer->ChangeCamera(CAMERAORDER::UICAMERA);
	}
}
void LorePromptFont::Update(float _DeltaTime)
{
	static float Times = 1.0f;
	if (Times > 0.0f)
	{
		Times -= 1.0f * _DeltaTime;
		return;
	}
	if (true == GameEngineInput::GetInst()->IsDown("Exit"))
	{
		SoundPlayer.Stop();
		Death();
	}
}
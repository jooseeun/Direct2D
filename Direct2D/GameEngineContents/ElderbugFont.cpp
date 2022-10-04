#include "ElderbugFont.h"
#include "PreCompile.h"


ElderbugFont* ElderbugFont::ElderFont = nullptr;

ElderbugFont::ElderbugFont() 
	: DiTop(nullptr)
	, DiBottom(nullptr)
	, DiBack(nullptr)
	, Font1(nullptr)
	, Font2(nullptr)
	, TextNum1(0)
	, TextTime(0.0f)
{
	ElderFont = this;
}

ElderbugFont::~ElderbugFont() 
{
}

void ElderbugFont::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("TextNext"))
	{
		GameEngineInput::GetInst()->CreateKey("TextNext", VK_SPACE);
	}
	{
		DiBack = CreateComponent<GameEngineTextureRenderer>();
		DiBack->SetOrder((int)OBJECTORDER::FrontObject);
		DiBack->SetTexture("Dialoguebackboard.png");
		DiBack->ScaleToTexture();
		DiBack->SetPivot(PIVOTMODE::BOT);
		DiBack->GetTransform().SetLocalPosition({ 0, 100, 0 });
		DiBack->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		DiBack->GetPixelData().MulColor.a = 0.5f;
		DiBack->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		DiTop = CreateComponent<GameEngineTextureRenderer>();
		DiTop->SetOrder((int)OBJECTORDER::FrontObject);
		DiTop->SetPivot(PIVOTMODE::BOT);
		DiTop->GetTransform().SetLocalPosition({ 0, 405, 0 });
		DiTop->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		DiBottom = CreateComponent<GameEngineTextureRenderer>();
		DiBottom->SetOrder((int)OBJECTORDER::FrontObject);
		DiBottom->SetPivot(PIVOTMODE::BOT);
		DiBottom->GetTransform().SetLocalPosition({ 0, 100, 0 });
		DiBottom->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		MainText[0] = "�ձ��� �ڽ��� ����� ������Ű�⸦ �ٶ�� ���� �̵��� �Ծ���.";
		MainText[1] = "�� ���� �ż�������� �ҷ���. �Ƹ� �װ��� ������ ��з� \ ������ ������ �ձ��� �־��� �ſ�.";
		MainText[2] = "��. ������ ������ ����� ������ ������ ���̿�.";
		MainText[3] = "�ᱹ. ������ ������°���.";
	}
	{
		Font1 = CreateComponent<GameEngineFontRenderer>();
		Font1->SetScreenPostion({ 1920 * 0.8 / 2 - 300, 1080 * 0.8 / 2 - 330, 1 });
		Font1->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		DiTop->CreateFrameAnimationCutTexture("Appear",
			FrameAnimation_DESC("Dialogue Cln_dialogue_fleur_top0000-Sheet.png", 0, 7, 0.1f, false));
		DiBottom->CreateFrameAnimationCutTexture("Appear",
			FrameAnimation_DESC("Dialogue Cln_dialogue_fleur_bottom0000-Sheet.png", 0, 7, 0.1f, false));
	}
	DiTop->ChangeFrameAnimation("Appear");
	DiTop->ScaleToCutTexture(0);

	DiBottom->ChangeFrameAnimation("Appear");
	DiBottom->ScaleToCutTexture(0);
}
void ElderbugFont::Update(float _DeltaTime)
{
	TextTime += 1.0f * _DeltaTime;
	if (true == GameEngineInput::GetInst()->IsDown("TextNext"))
	{
		TextNum1 += 1;
		TextTime = 0.0f;
		if (TextNum1 == 4)
		{
			Death();
		}
	}

	if( TextTime > 1.0f )
	{
		Font1->SetText(MainText[TextNum1], "Noto Serif KR");
		Font1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font1->SetSize(25.0f);
		TextTime = -500.0f;
	}

}
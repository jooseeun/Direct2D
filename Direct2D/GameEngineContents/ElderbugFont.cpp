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
	, TextNum2(0)
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
		GameEngineInput::GetInst()->CreateKey("TextNext", VK_UP);
	}
	{
		DiBack = CreateComponent<GameEngineTextureRenderer>();
		DiBack->SetOrder((int)OBJECTORDER::FrontObject);
		DiBack->SetTexture("Dialoguebackboard.png");
		DiBack->ScaleToTexture();
		DiBack->SetPivot(PIVOTMODE::BOT);
		DiBack->GetTransform().SetLocalPosition({ 0, 100, 0 });
		DiBack->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		DiBack->GetPixelData().MulColor.a = 0.65f;
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
		Text[0] = "왕국이 자신의 욕망을 충족시키기를 바라며 많은 이들이 왔었소.";
		Text[1] = "한 때는 신성둥지라고 불렸지 아마 그곳은 보물과.";
		Text[2] = "비밀로 가득찬 위대한 왕국이 있었을 거요.";
		Text[3] = "흠 이제는 괴물과 광기로 가득한 무덤일 뿐이오.";
		Text[4] = "결국 모든것은 사라지는거지.";
	}
	{
		Font1 = CreateComponent<GameEngineFontRenderer>();
		Font1->SetScreenPostion({ 1920 * 0.8 / 2 - 300, 1080 * 0.8 / 2 - 330, 1 });
		Font1->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		Font2 = CreateComponent<GameEngineFontRenderer>();
		Font2->SetScreenPostion({ 1920 * 0.8 / 2 - 300, 1080 * 0.8 / 2 - 290, 1 });
		Font2->ChangeCamera(CAMERAORDER::UICAMERA);
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
	TextNum2 = 0.0f;

	MainText = Text[TextNum1];
}
void ElderbugFont::Update(float _DeltaTime)
{
	TextTime += 1.0f * _DeltaTime;
	if (true == GameEngineInput::GetInst()->IsDown("TextNext"))
	{

		if (TextNum1 == 5)
		{
			Death();
			return;
		}

		MainText = Text[TextNum1];
		if (TextNum1 == 1)
		{
			GameEngineSound::SoundPlayControl("Elderbug_02.wav");
		}

		TextNum2 = 0;
		Temp = "";
		Temp2 = "";

		if (TextNum1 == 1)
		{
			TextNum1 += 1;
			MainText2 = Text[TextNum1];
			TextNum3 = 0;
			Font2->On();


		}

		TextNum1 += 1;

	}

	if( TextTime > 0.03f )
	{
		if (TextNum1 != 3)
		{
			Font2->Off();
		}
		if (MainText[TextNum2] == '.')
		{
			if (TextNum1 == 3)
			{
				if (MainText2[TextNum3] == '.')
				{
					return;
				}
				if (MainText2[TextNum3] != ' ')
				{
					Temp2 += MainText2[TextNum3];
					TextNum3 += 1;
				}
				Temp2 += MainText2[TextNum3];
				TextNum3 += 1;
				Font2->SetText(Temp2, "Noto Serif KR");
				Font2->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				Font2->SetSize(25.0f);

				TextTime = 0.0f;
			}
			return;
		}
		if (MainText[TextNum2] != ' ')
		{
			Temp += MainText[TextNum2];
			TextNum2 += 1;
		}
		Temp += MainText[TextNum2];
		TextNum2 += 1;
		Font1->SetText(Temp, "Noto Serif KR");
		Font1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Font1->SetSize(25.0f);

		TextTime = 0.0f;
	}

}
#include "Elderbug.h"
#include "ElderbugFont.h"
#include "PreCompile.h"

Elderbug::Elderbug() 
	:ElderbugRenderer(nullptr)
	, ElderbugCollision(nullptr)
	, PromptRenderer(nullptr)
	, Font(nullptr)
	, Trigger(nullptr)
	, FontA(0.0f)

{

}

Elderbug::~Elderbug() 
{
}

void Elderbug::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });
	if (false == GameEngineInput::GetInst()->IsKey("Next"))
	{
		GameEngineInput::GetInst()->CreateKey("Next", VK_UP);
	}
	{
		ElderbugRenderer = CreateComponent< GameEngineTextureRenderer>();
		ElderbugRenderer->SetPivot(PIVOTMODE::BOT);
		ElderbugRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Elderbug_idle_01-Sheet.png", 0, 5, 0.1f, true));
		ElderbugRenderer->ChangeFrameAnimation("Idle");
		ElderbugRenderer->ScaleToCutTexture(0);
	}
	{
		ElderbugCollision = CreateComponent<GameEngineCollision>();
		ElderbugCollision->ChangeOrder((int)(OBJECTORDER::Player));
		ElderbugCollision->GetTransform().SetLocalScale({ 500,500,1000.0f });
		ElderbugCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });
	}
	{
		PromptRenderer = CreateComponent<GameEngineTextureRenderer>();
		PromptRenderer->SetOrder((int)OBJECTORDER::FrontObject);
		PromptRenderer->SetPivot(PIVOTMODE::BOT);
		PromptRenderer->SetPivotToVector({ 0,150,-300 });
	}



	{
		Font = CreateComponent<GameEngineFontRenderer>();
		Font->SetText("µè±â", "Noto Serif KR");
		Font->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		Font->SetSize(35.0);
		Font->SetPositionMode(FontPositionMode::WORLD);
		Font->GetTransform().SetLocalPosition({ -40,290,-300 });
		Font->ChangeCamera(CAMERAORDER::MAINCAMERA);
	}

	{
		PromptRenderer->CreateFrameAnimationCutTexture("Appear",
			FrameAnimation_DESC("Prompts Cln_prompt_appear0000-Sheet.png", 0, 9, 0.1f, false));
		PromptRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Prompts Cln_prompt_idle0000-Sheet.png", 0, 4, 0.09f, true));
		std::vector<unsigned int> aniorder = { 9,8,7,6,5,4,3,2,1,0 };
		PromptRenderer->CreateFrameAnimationCutTexture("Disappear",
			FrameAnimation_DESC("Prompts Cln_prompt_appear0000-Sheet.png", aniorder, 0.1f, false));
		PromptRenderer->Off();
	}
}

void Elderbug::Update(float _DeltaTime)
{
	if (ElderbugCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&Elderbug::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2)
	) == false)
	{
		if (Trigger == true)
		{
			PromptRenderer->ChangeFrameAnimation("Disappear");
			PromptRenderer->ScaleToCutTexture(0);
			Trigger = false;
		}
		FontA -= 0.4f * GameEngineTime::GetDeltaTime();
		if (FontA <= 0.0f)
		{
			FontA = 0.0f;
		}

		Font->SetColor({ 1.0f, 1.0f, 1.0f, FontA });
	}

	ElderbugCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&Elderbug::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2));


	PromptRenderer->AnimationBindEnd("Appear", [=](const FrameAnimation_DESC& _Info)
	{
		PromptRenderer->ChangeFrameAnimation("Idle");
		PromptRenderer->ScaleToCutTexture(0);
	});
	PromptRenderer->AnimationBindEnd("Disappear", [=](const FrameAnimation_DESC& _Info)
	{
		PromptRenderer->Off();
	});
}

CollisionReturn Elderbug::PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	FontA += 0.4f * GameEngineTime::GetDeltaTime();
	if (FontA >= 1.0f)
	{
		FontA = 1.0f;
	}
	Font->SetColor({ 1.0f, 1.0f, 1.0f, FontA });

	if (Trigger == false)
	{
		PromptRenderer->On();
		PromptRenderer->ChangeFrameAnimation("Appear");
		PromptRenderer->ScaleToCutTexture(0);
		Trigger = true;
	}
	if (true == GameEngineInput::GetInst()->IsPress("Next"))
	{
		Font->Off();
		PromptRenderer->Off();
		StartTalk();
	}

	return CollisionReturn::ContinueCheck;

}

void Elderbug::StartTalk()
{
	if (ElderbugFont::GetElderFont() == nullptr)
	{
		ElderbugFont* NewFont = GetLevel()->CreateActor<ElderbugFont>();
	}

}
#include "ChageExplanation.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"
#include "Player.h"
ChageExplanation::ChageExplanation() 
	: LightRenderer(nullptr)
	, BackGroundRenderer(nullptr)
	, TriggerCol(nullptr)
	, PromptRenderer(nullptr)
	, Font(nullptr)
	, Trigger(false)
{

}

ChageExplanation::~ChageExplanation() 
{
}

void ChageExplanation::Start()
{
	BackGroundRenderer = CreateComponent<GameEngineTextureRenderer>();
	BackGroundRenderer->SetOrder((int)OBJECTORDER::Player);
	BackGroundRenderer->SetPivot(PIVOTMODE::BOT);
	BackGroundRenderer->SetTexture("Mines_Layered_0017_a.png");
	BackGroundRenderer->ScaleToTexture();

	LightRenderer = CreateComponent<GameEngineTextureRenderer>();
	LightRenderer->SetOrder((int)OBJECTORDER::Player);
	LightRenderer->SetPivot(PIVOTMODE::BOT);
	LightRenderer->SetTexture("lit_tablet.png");
	LightRenderer->ScaleToTexture();
	LightRenderer->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
	LightRenderer->GetPixelData().MulColor.a = 0.0f;

	Font = CreateComponent<GameEngineFontRenderer>();
	Font->SetText("Á¶»ç", "Noto Serif KR");
	Font->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Font->SetSize(40.0);
	Font->SetScreenPostion(GetTransform().GetLocalPosition() - GetLevel()->GetMainCamera()->GetTransform().GetLocalPosition());
	Font->ChangeCamera(CAMERAORDER::MAINCAMERA);

	PromptRenderer = CreateComponent<GameEngineTextureRenderer>();
	PromptRenderer->SetOrder((int)OBJECTORDER::FrontObject);
	PromptRenderer->SetPivot(PIVOTMODE::BOT);
	PromptRenderer->SetPivotToVector({ -5,280,-300 });

	GameEngineTexture::Cut("Prompts Cln_prompt_appear0000-Sheet.png", 10, 1);
	GameEngineTexture::Cut("Prompts Cln_prompt_idle0000-Sheet.png", 5, 1);

	TriggerCol = CreateComponent<GameEngineCollision>();
	TriggerCol->GetTransform().SetLocalScale({ 1000,1000,1000.0f });
	TriggerCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });


	PromptRenderer->CreateFrameAnimationCutTexture("Appear",
		FrameAnimation_DESC("Prompts Cln_prompt_appear0000-Sheet.png", 0, 9, 0.1f, false));
	PromptRenderer->CreateFrameAnimationCutTexture("Idle",
		FrameAnimation_DESC("Prompts Cln_prompt_idle0000-Sheet.png", 0, 4, 0.09f, true));
	std::vector<unsigned int> aniorder = { 9,8,7,6,5,4,3,2,1,0 };
	PromptRenderer->CreateFrameAnimationCutTexture("Disappear",
		FrameAnimation_DESC("Prompts Cln_prompt_appear0000-Sheet.png", aniorder, 0.1f, false));
	PromptRenderer->Off();

}
float4 ChageExplanation::GetWorldRePosition(float4 ReSize)
{
	float4 Pos = ReSize;

	float4x4 ViewPort;
	ViewPort.ViewPort(1920*0.7, 1080 * 0.7, 0, 0, 0, 1);
	ViewPort.Inverse();

	float4x4 ProjectionInvers = GetLevel()->GetUICamera()->GetProjectionMatrix().InverseReturn();

	Pos = Pos * ViewPort;
	Pos = Pos * ProjectionInvers;

	return Pos;
}
void ChageExplanation::Update(float _DeltaTime)
{

	if (TriggerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&ChageExplanation::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2)
	) == false)
	{
		if (Trigger == true)
		{
			PromptRenderer->ChangeFrameAnimation("Disappear");
			PromptRenderer->ScaleToCutTexture(0);
			Trigger = false;
		}
		LightRenderer->GetPixelData().MulColor.a -= 0.5f * GameEngineTime::GetDeltaTime();
		if (LightRenderer->GetPixelData().MulColor.a <= 0.0f)
		{
			LightRenderer->GetPixelData().MulColor.a = 0.0f;
		}
	}

	TriggerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&ChageExplanation::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2)
	);

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

CollisionReturn ChageExplanation::PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	LightRenderer->GetPixelData().MulColor.a += 0.2f * GameEngineTime::GetDeltaTime();
	if (LightRenderer->GetPixelData().MulColor.a >= 1.0f)
	{
		LightRenderer->GetPixelData().MulColor.a = 1.0f;
	}
	
	if (Trigger == false)
	{
		PromptRenderer->On();
		PromptRenderer->ChangeFrameAnimation("Appear");
		PromptRenderer->ScaleToCutTexture(0);
		Trigger = true;
	}

	return CollisionReturn::ContinueCheck;

}
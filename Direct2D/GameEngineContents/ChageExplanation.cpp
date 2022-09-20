#include "ChageExplanation.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"
#include "Player.h"
ChageExplanation::ChageExplanation() 
	: LightRenderer(nullptr)
	, BackGroundRenderer(nullptr)
	, TriggerCol(nullptr)
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


	TriggerCol = CreateComponent<GameEngineCollision>();
	TriggerCol->GetTransform().SetLocalScale({ 1000,1000,1000.0f });
	TriggerCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });

}
void ChageExplanation::Update(float _DeltaTime)
{
	if (TriggerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&ChageExplanation::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2)
	) == false)
	{
		LightRenderer->GetPixelData().MulColor.a -= 0.5f * GameEngineTime::GetDeltaTime();
		if (LightRenderer->GetPixelData().MulColor.a <= 0.0f)
		{
			LightRenderer->GetPixelData().MulColor.a = 0.0f;
		}
	}

	TriggerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&ChageExplanation::PlusAlpha, this, std::placeholders::_1, std::placeholders::_2)
	);
}

bool ChageExplanation::PlusAlpha(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	LightRenderer->GetPixelData().MulColor.a += 0.2f * GameEngineTime::GetDeltaTime();
	return true;
}
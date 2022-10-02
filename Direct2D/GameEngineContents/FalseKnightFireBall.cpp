#include "FalseKnightFireBall.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"


FalseKnightFireBall::FalseKnightFireBall() 
	:SkillRenderer(nullptr)
	, SkillCol(nullptr)
{
}

FalseKnightFireBall::~FalseKnightFireBall() 
{
}

void FalseKnightFireBall::Start()
{
	{
		SkillRenderer = CreateComponent< GameEngineTextureRenderer>();
		SkillRenderer->SetOrder((int)OBJECTORDER::Monster);
		SkillRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		SkillCol = CreateComponent<GameEngineCollision>();
		SkillCol->GetTransform().SetLocalScale({ 135,135,1000.0f });
		SkillCol->GetTransform().SetLocalPosition({ 0,60,0 });
		SkillCol->ChangeOrder((int)(OBJECTORDER::Monster));
	}

	SkillRenderer->CreateFrameAnimationCutTexture("Start",
		FrameAnimation_DESC("barrel_fall0000-Sheet.png", 0, 7, 0.1f, true));

	SkillRenderer->ChangeFrameAnimation("Start");
	SkillRenderer->ScaleToCutTexture(0);
}
void FalseKnightFireBall::Update(float _DeltaTime)
{
	Gravity();
}

void FalseKnightFireBall::Gravity()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}

	float4 Color = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 1);

	if (false == Color.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		Death();
		
	}
	else
	{
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 1000.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}
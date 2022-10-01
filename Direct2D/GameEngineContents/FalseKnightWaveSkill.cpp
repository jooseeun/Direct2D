#include "FalseKnightWaveSkill.h"
#include "PreCompile.h"

FalseKnightWaveSkill::FalseKnightWaveSkill() 
	:SkillRenderer(nullptr)
	, SkillCol(nullptr)
	, WaveScale()
	, CurDir(MonsterDIR::Left)
{

}

FalseKnightWaveSkill::~FalseKnightWaveSkill() 
{
}

void FalseKnightWaveSkill::Start()
{
	{
		SkillRenderer = CreateComponent< GameEngineTextureRenderer>();
		SkillRenderer->SetOrder((int)OBJECTORDER::Monster);
		SkillRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		SkillCol = CreateComponent<GameEngineCollision>();
		SkillCol->GetTransform().SetLocalScale({ 50,50,1000.0f });
		SkillCol->ChangeOrder((int)(OBJECTORDER::Monster));
	}
	WaveScale[0] = (float4{ 50,50,1000 }
		, float4{ 50,50,1000 }
		, float4{ 53,98,1000 }
		, float4{ 63,120,1000 }
		, float4{ 77,120,1000 }
		, float4{ 79,120,1000 }
		, float4{ 97,120,1000 }
		, float4{ 109,123,1000 }
		, float4{ 127,123,1000 }
		, float4{ 137,123,1000 }
		, float4{ 171,123,1000 }
		, float4{ 177,123,1000 }
		, float4{ 203,123,1000 }
		, float4{ 231,123,1000 }
		, float4{ 250,123,1000 }
		, float4{ 250,123,1000 }
	, float4{ 309,50,1000 });
	SkillRenderer->CreateFrameAnimationCutTexture("Start",
		FrameAnimation_DESC("Boss_WaveSkill_Sheet.png", 0, 15, 0.05f, false));

	SkillRenderer->ChangeFrameAnimation("Start");
	SkillRenderer->ScaleToCutTexture(0);
}

void FalseKnightWaveSkill::Update(float _DeltaTime)
{
	static float Time = 0.0f;
	Time += 20.0f * _DeltaTime;
	SkillCol->GetTransform().SetLocalScale((int)Time);
	if (CurDir == MonsterDIR::Left)
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * 500.0f * _DeltaTime);
		SkillRenderer->GetTransform().PixLocalNegativeX();

	}
	if (CurDir == MonsterDIR::Right)
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * 500.0f * _DeltaTime);
		SkillRenderer->GetTransform().PixLocalPositiveX();
	}

	SkillRenderer->AnimationBindEnd("Start", [=](const FrameAnimation_DESC& _Info)
	{
		Death();
	});
}
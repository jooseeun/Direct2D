#include "TitleParticle.h"
#include "PreCompile.h"
#include <GameEngineBase/GameEngineRandom.h>

TitleParticle::TitleParticle()
	:Ren(nullptr)
	, Time(5.0f)
{
}

TitleParticle::~TitleParticle()
{
}

void TitleParticle::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	float num = GameEngineRandom::MainRandom.RandomInt(0.2f, 1.5f);
	float X_ = GameEngineRandom::MainRandom.RandomFloat(300.0f, 1650.0f);
	float Y_ = GameEngineRandom::MainRandom.RandomFloat(-1500.0f, -1200.0f);
	Ren = CreateComponent< GameEngineTextureRenderer>();
	Ren->SetPivot(PIVOTMODE::BOT);
	Ren->SetTexture("particle_01.png");
	Ren->GetTransform().SetLocalScale({ Ren->GetCurTexture()->GetScale() * num });
	Ren->GetTransform().SetLocalPosition({ X_, Y_, 0 });
	Ren->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
	Ren->GetPixelData().MulColor.a = 1.0f;
}

void TitleParticle::Update(float _DeltaTime)
{
	Time -= 1.0f * _DeltaTime;
	if (Time >= 0.0f)
	{
		if (Ren->GetPixelData().MulColor.a < 1.0f)
		{
			Ren->GetPixelData().MulColor.a += 1.0f * _DeltaTime;
		}
		else
		{
			Ren->GetPixelData().MulColor.a = 1.0f;
		}
	}
	if (Time < 0.0f)
	{
		Ren->GetPixelData().MulColor.a -= 1.0f * _DeltaTime;
		if (Ren->GetPixelData().MulColor.a <= 0.0f)
		{
			Death();
		}
	}
	Ren->GetTransform().SetWorldUpMove(100.0f, _DeltaTime);

}
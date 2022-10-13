#include "TitleEffect.h"
#include "PreCompile.h"
#include <GameEngineBase/GameEngineRandom.h>

TitleEffect::TitleEffect() 
	:Ren(nullptr)
	, Time(4.0f)
{
}

TitleEffect::~TitleEffect() 
{
}

void TitleEffect::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	int num = GameEngineRandom::MainRandom.RandomInt(0, 4);
	float X_ = GameEngineRandom::MainRandom.RandomFloat(300.0f, 1650.0f);
	float Y_ = GameEngineRandom::MainRandom.RandomFloat(-1700.0f, -1500.0f);
	Ren = CreateComponent< GameEngineTextureRenderer>();
	Ren->SetPivot(PIVOTMODE::BOT);
	Ren->SetTexture("wispy_smoke_particle_abyss.png", num);
	Ren->ScaleToCutTexture(0);
	Ren->GetTransform().SetLocalScale({ Ren->GetCurTexture()->GetCutScale(0) * 5.0f });
	Ren->GetTransform().SetLocalPosition({ X_, Y_, 0 });
	Ren->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
	Ren->GetPixelData().MulColor.a = 0.0f;
}

void TitleEffect::Update(float _DeltaTime)
{
	Time -= 1.0f * _DeltaTime;
	if (Time >= 0.0f)
	{
		if (Ren->GetPixelData().MulColor.a < 0.7f)
		{
			Ren->GetPixelData().MulColor.a += 0.5f * _DeltaTime;
		}
		else
		{

			Ren->GetPixelData().MulColor.a = 0.7f;
		}
	}
	if (Time < 0.0f)
	{
		Ren->GetPixelData().MulColor.a -= 0.5f * _DeltaTime;
		if (Ren->GetPixelData().MulColor.a <= 0.0f)
		{
			Death();
		}
	}
	Ren->GetTransform().SetWorldUpMove(25.0f,_DeltaTime);

}
#include "SmallSmoke.h"
#include "PreCompile.h"

SmallSmoke::SmallSmoke() 
	:Smoke(nullptr)
{
}

SmallSmoke::~SmallSmoke() 
{
}

void SmallSmoke::Start()
{
	Smoke = CreateComponent<GameEngineTextureRenderer>();
	Smoke->SetOrder((int)OBJECTORDER::Player);
	Smoke->SetPivot(PIVOTMODE::CENTER);
	Smoke->SetTexture("hot_spring_smoke.png");
	Smoke->GetTransform().SetLocalScale(Smoke->GetCurTexture()->GetScale() * 4.0f);
	Smoke->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
	Smoke->GetPixelData().MulColor.a = 0.8;
}

void SmallSmoke::Update(float _DeltaTime)
{
	Smoke->GetPixelData().MulColor.a -= 0.5*_DeltaTime;
	Smoke->GetTransform().SetWorldUpMove(100.0f , _DeltaTime);
	if (Smoke->GetPixelData().MulColor.a <= 0.0f)
	{
		Death();
	}
}
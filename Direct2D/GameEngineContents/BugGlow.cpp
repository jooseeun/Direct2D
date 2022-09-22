#include "BugGlow.h"
#include "PlayLevelManager.h"
#include "PreCompile.h"

BugGlow::BugGlow() 
	: BugGlowRenderer(nullptr)
	, LightRenderer(nullptr)
{
}

BugGlow::~BugGlow() 
{
}

void BugGlow::Start()
{
	BugGlowRenderer = CreateComponent<GameEngineTextureRenderer>();
	BugGlowRenderer->SetOrder((int)OBJECTORDER::Player);
	BugGlowRenderer->SetPivot(PIVOTMODE::BOT);
	BugGlowRenderer->CreateFrameAnimationCutTexture("Idle",
		FrameAnimation_DESC("Ambient Life_cave_glow_bug0000-Sheet.png", 0, 3, 0.1f, true));
	BugGlowRenderer->ChangeFrameAnimation("Idle");
	BugGlowRenderer->ScaleToCutTexture(0);

	LightRenderer = CreateComponent<GameEngineTextureRenderer>();
	LightRenderer->SetOrder((int)OBJECTORDER::Player);
	LightRenderer->SetPivot(PIVOTMODE::BOT);
	LightRenderer->CreateFrameAnimationCutTexture("Idle",
		FrameAnimation_DESC("Ambient Life_bug_glow0000-Sheet.png", 0, 5, 0.1f, true));
	LightRenderer->ChangeFrameAnimation("Idle");
	LightRenderer->ScaleToCutTexture(0);
	LightRenderer->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
	LightRenderer->GetPixelData().MulColor.a = 0.9f;
}
void BugGlow::Update(float _DeltaTime)
{

}
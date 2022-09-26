#include "Elderbug.h"
#include "PreCompile.h"

Elderbug::Elderbug() 
	:ElderbugRenderer(nullptr)
	, ElderbugCollision(nullptr)
{

}

Elderbug::~Elderbug() 
{
}

void Elderbug::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

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
		ElderbugCollision->GetTransform().SetLocalScale(ElderbugRenderer->GetCurTexture()->GetCutScale(0));
		ElderbugCollision->ChangeOrder((int)(OBJECTORDER::Player));
	}
}

void Elderbug::Update(float _DeltaTime)
{

}
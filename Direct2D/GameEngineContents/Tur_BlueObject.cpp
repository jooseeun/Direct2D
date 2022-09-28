#include "Tur_BlueObject.h"
#include "PreCompile.h"

Tur_BlueObject::Tur_BlueObject() 
	:BlueObjectRenderer(nullptr)
	, BlueObjectCollision(nullptr)
	, BlueFlower1(nullptr)
	, BlueFlower2(nullptr)
	, BlueFlower3(nullptr)
	, BlueFlower4(nullptr)
	, AniTime(4.0f)
{
}

Tur_BlueObject::~Tur_BlueObject() 
{
}

void Tur_BlueObject::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		BlueObjectRenderer = CreateComponent< GameEngineTextureRenderer>();
		BlueObjectRenderer->SetPivot(PIVOTMODE::BOT);
		BlueObjectRenderer->GetTransform().SetLocalPosition({ 0,-30,0 });
		BlueObjectRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_cocoon_top0000-Sheet.png", 0, 22, 0.1f, true));
		BlueObjectRenderer->ChangeFrameAnimation("Idle");
		BlueObjectRenderer->ScaleToCutTexture(0);
	}
	{
		BlueObjectCollision = CreateComponent<GameEngineCollision>();
		BlueObjectCollision->GetTransform().SetLocalPosition({ 10,300,0 });
		BlueObjectCollision->GetTransform().SetLocalScale(BlueObjectRenderer->GetCurTexture()->GetCutScale(0)*0.7);
		BlueObjectCollision->ChangeOrder((int)(OBJECTORDER::FrontObject));
	}
	{
		BlueFlower1 = CreateComponent< GameEngineTextureRenderer>();
		BlueFlower1->SetPivot(PIVOTMODE::BOT);
		BlueFlower1->GetTransform().SetLocalPosition({-150,-120,0 });
		BlueFlower1->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_plant_010013-Sheet.png", 0, 4, 0.12f, false));
		BlueFlower1->ChangeFrameAnimation("Idle");
		BlueFlower1->ScaleToCutTexture(0);
	
	}
	{
		BlueFlower2 = CreateComponent< GameEngineTextureRenderer>();
		BlueFlower2->SetPivot(PIVOTMODE::BOT);
		BlueFlower2->GetTransform().SetLocalPosition({ 0,-130,0 });
		BlueFlower2->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_plant_020007-Sheet.png", 0, 15, 0.1f, false));
		BlueFlower2->ChangeFrameAnimation("Idle");
		BlueFlower2->ScaleToCutTexture(0);
	}
	{
		BlueFlower3 = CreateComponent< GameEngineTextureRenderer>();
		BlueFlower3->SetPivot(PIVOTMODE::BOT);
		BlueFlower3->GetTransform().SetLocalPosition({ 150,-150,0 });
		BlueFlower3->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_plant_020007-Sheet.png", 0, 15, 0.1f, false));
		BlueFlower3->ChangeFrameAnimation("Idle");
		BlueFlower3->ScaleToCutTexture(0);
	}
	{
		BlueFlower4 = CreateComponent< GameEngineTextureRenderer>();
		BlueFlower4->SetPivot(PIVOTMODE::BOT);
		BlueFlower4->GetTransform().SetLocalPosition({ 300,-130,0 });
		BlueFlower4->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_plant_020007-Sheet.png", 0, 15, 0.1f, false));
		BlueFlower4->ChangeFrameAnimation("Idle");
		BlueFlower4->ScaleToCutTexture(0);
	}
}

void Tur_BlueObject::Update(float _DeltaTime)
{
	AniTime -= 1.0f * GameEngineTime::GetDeltaTime();
	if (AniTime < 0.0f)
	{
		BlueFlower1->CurAnimationReset();
		BlueFlower2->CurAnimationReset();
		BlueFlower3->CurAnimationReset();
		BlueFlower4->CurAnimationReset();
		AniTime = 5.0f;
	}

	BlueObjectCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&Tur_BlueObject::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);

}

CollisionReturn Tur_BlueObject::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return CollisionReturn::ContinueCheck;
}
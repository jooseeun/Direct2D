#include "Tur_BlueObject.h"
#include "PreCompile.h"

Tur_BlueObject::Tur_BlueObject() 
	:BlueObjectRenderer(nullptr)
	, BlueObjectCollision(nullptr)
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
		BlueObjectRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Health Bugs_health_cocoon_top0000-Sheet.png", 0, 22, 0.08f, true));
		BlueObjectRenderer->ChangeFrameAnimation("Idle");
		BlueObjectRenderer->ScaleToCutTexture(0);
	}
	{
		BlueObjectCollision = CreateComponent<GameEngineCollision>();
		BlueObjectCollision->GetTransform().SetLocalScale(BlueObjectRenderer->GetCurTexture()->GetCutScale(0));
		BlueObjectCollision->ChangeOrder((int)(OBJECTORDER::StopObject));
	}
}

void Tur_BlueObject::Update(float _DeltaTime)
{
	BlueObjectCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&Tur_BlueObject::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);
}

CollisionReturn Tur_BlueObject::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	Death();

	return CollisionReturn::Break;
}
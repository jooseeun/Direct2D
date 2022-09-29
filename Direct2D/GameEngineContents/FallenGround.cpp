#include "FallenGround.h"
#include "Player.h"
#include "PreCompile.h"
#include "SmallSmoke.h"

FallenGround::FallenGround() 
	: GroundRenderer1(nullptr)
	, GroundRenderer2(nullptr)
	, GroundCol1(nullptr)
	, GroundCol2(nullptr)
	, Trigger(false)
{
}

FallenGround::~FallenGround() 
{
}

void FallenGround::Start()
{
	GroundRenderer1 = CreateComponent<GameEngineTextureRenderer>();
	GroundRenderer1->SetOrder((int)OBJECTORDER::Player);
	GroundRenderer1->SetPivot(PIVOTMODE::BOT);
	GroundRenderer1->GetTransform().SetLocalPosition({ 300,0,0 });
	GroundRenderer1->SetTexture("FallenGround.png");
	GroundRenderer1->ScaleToTexture();
	GroundRenderer1->GetTransform().PixLocalNegativeX();

	GroundRenderer2 = CreateComponent<GameEngineTextureRenderer>();
	GroundRenderer2->SetOrder((int)OBJECTORDER::Player);
	GroundRenderer2->SetPivot(PIVOTMODE::BOT);
	GroundRenderer2->GetTransform().SetLocalPosition({ -300,0,0 });
	GroundRenderer2->SetTexture("FallenGround.png");
	GroundRenderer2->ScaleToTexture();

	GroundCol1 = CreateComponent<GameEngineCollision>();
	GroundCol1->GetTransform().SetLocalScale({ 1400,400,1000.0f });
	GroundCol1->GetTransform().SetLocalPosition({ 0,300,0 });
	GroundCol1->ChangeOrder((int)(OBJECTORDER::Ground));
	GroundCol1->SetDebugSetting(CollisionType::CT_AABB, { 1.0,0,0,1 });

	GroundCol2 = CreateComponent<GameEngineCollision>();
	GroundCol2->GetTransform().SetLocalScale({ 1000,400,1000.0f });
	GroundCol2->GetTransform().SetLocalPosition({ 0,300,0 });
	GroundCol2->ChangeOrder((int)(OBJECTORDER::Ground));
	GroundCol2->SetDebugSetting(CollisionType::CT_AABB, { 1.0,0,0,1 });
}
void FallenGround::End()
{
	for (int i = 0; i < 4; i++)
	{
		SmallSmoke* Smoke = GetLevel()->CreateActor<SmallSmoke>();
		Smoke->GetTransform().SetLocalPosition({ 1200.0f + i * 200.0f, -1200,  -250 });
	}
}
void FallenGround::Update(float _DeltaTime)
{
	if (Trigger == false)
	{
		GroundCol2->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
			std::bind(&FallenGround::StartTrigger, this, std::placeholders::_1, std::placeholders::_2));
	}


}

CollisionReturn FallenGround::StartTrigger(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	Trigger = true;
	Player::GetMainPlayer()->ShakeTimeReset();
	GroundRenderer1->GetTransform().SetLocalRotation({ 0,0,3 });
	GroundRenderer2->GetTransform().SetLocalRotation({ 0,0,-3 });
	Death(0.5f);
	return CollisionReturn::Break;
}
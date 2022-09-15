#include "GeoCoin.h"
#include "PlayLevelManager.h"
#include "PreCompile.h"

GeoCoin::GeoCoin() 
	:GeoCoinCol(nullptr)
	,GeoCoinRenderer(nullptr)
	, UpTime(0.4f)
	, SpringNum(3)
{
}

GeoCoin::~GeoCoin() 
{
}

void GeoCoin::Start()
{

	{
		GeoCoinRenderer = CreateComponent< GameEngineTextureRenderer>();
		GeoCoinRenderer->GetTransform().SetLocalScale({ 40,40,100 });
		GeoCoinRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		GeoCoinCol = CreateComponent<GameEngineCollision>();
		GeoCoinCol->GetTransform().SetLocalScale({ 40,40,100 });
		GeoCoinCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,15,0 });
		GeoCoinCol->ChangeOrder((int)(OBJECTORDER::Coin));
	}

	GeoCoinRenderer->CreateFrameAnimationCutTexture("Idle",
		FrameAnimation_DESC("Geo_coin0000-Sheet.png", 0, 7, 0.1f, true));
	GeoCoinRenderer->CreateFrameAnimationCutTexture("Air",
		FrameAnimation_DESC("Geo_coin_air000-Sheet.png", 0, 8, 0.1f, false));
	GeoCoinRenderer->ChangeFrameAnimation("Air");
	GeoCoinRenderer->ScaleToCutTexture(0);
}
void GeoCoin::Update(float _DeltaTime)
{
	if (UpTime > 0.0f)
	{
		Up();
	}

	Gravity();
	DeathCheck();
}

void GeoCoin::Gravity()
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
		GeoCoinRenderer->ChangeFrameAnimation("Idle");
		GeoCoinRenderer->ScaleToCutTexture(0);
		return;
	}
	else
	{
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 500.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

void GeoCoin::Up()
{
	UpTime -= 1.0f * GameEngineTime::GetDeltaTime();

	GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
GetTransform().GetWorldPosition().y + 1000.0f * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
}
void GeoCoin::DeathCheck()
{

	GeoCoinCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&GeoCoin::DeathCoin, this, std::placeholders::_1, std::placeholders::_2)
	);
}
bool GeoCoin::DeathCoin(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	Death();
	return true;
}
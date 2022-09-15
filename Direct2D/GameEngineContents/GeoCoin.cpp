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
		GeoCoinRenderer->GetTransform().SetLocalScale({ 30,30,100 });
		GeoCoinRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		GeoCoinCol = CreateComponent<GameEngineCollision>();
		GeoCoinCol->GetTransform().SetLocalScale({ 30,30,100 });
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
	GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

void GeoCoin::Up()
{
	UpTime -= 0.1f * GameEngineTime::GetDeltaTime();

	GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
GetTransform().GetWorldPosition().y + 1000.0f * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
}
bool GeoCoin::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 15,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() - 15,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorUp = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 30);

	if (false == ColorUp.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}


	if (false == ColorL.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;

	}


	if (false == ColorR.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}


	return false;
}
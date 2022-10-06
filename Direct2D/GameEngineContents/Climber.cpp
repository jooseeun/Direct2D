#include "Climber.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"
#include "Player.h"
#include "GeoCoin.h"
#include <iostream>

Climber::Climber()
	: MonsterRenderer(nullptr)
	, MonsterCollision(nullptr)
	, TriggerCollision(nullptr)
	, StateManager()
	, Speed(150)
	, Health(3)
	, OnGround(false)
	, HPEffect1(nullptr)
	, HPEffect2(nullptr)
	, HPEffect3(nullptr)
	, IsDeath_(false)
{
}

Climber::~Climber()
{
}

void Climber::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		MonsterRenderer = CreateComponent< GameEngineTextureRenderer>();
		MonsterRenderer->GetTransform().SetLocalScale({ 108, 80, 10.0f });
		MonsterRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		HPEffect1 = CreateComponent< GameEngineTextureRenderer>();
		HPEffect1->SetTexture("hit_orange.png");
		HPEffect1->GetTransform().SetLocalScale(HPEffect1->GetCurTexture()->GetScale() * 6.0f);
		HPEffect1->SetPivot(PIVOTMODE::CENTER);
		HPEffect1->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		HPEffect1->GetPixelData().MulColor.a = 0.0f;
	}
	{
		HPEffect2 = CreateComponent< GameEngineTextureRenderer>();
		HPEffect2->CreateFrameAnimationCutTexture("Start",
			FrameAnimation_DESC("Parasite Balloon Cln_Parasite_Blob_Flyer0012-Sheet.png", 0, 3, 0.1f, false));
		HPEffect2->ChangeFrameAnimation("Start");
		HPEffect2->SetPivot(PIVOTMODE::CENTER);
		HPEffect2->GetTransform().SetLocalScale(HPEffect2->GetCurTexture()->GetCutScale(0) * 1.5f);
		HPEffect2->GetTransform().SetLocalPosition({ 20,50,0 });
		HPEffect2->Off();
	}
	{

		HPEffect3 = CreateComponent< GameEngineTextureRenderer>();
		HPEffect3->CreateFrameAnimationCutTexture("Start",
			FrameAnimation_DESC("orange_puff_animated.png", 0, 8, 0.1f, false));
		HPEffect3->ChangeFrameAnimation("Start");
		HPEffect3->SetPivot(PIVOTMODE::CENTER);
		HPEffect3->GetTransform().SetLocalScale(HPEffect3->GetCurTexture()->GetCutScale(0) * 5.0f);
		HPEffect3->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		HPEffect3->GetPixelData().MulColor.a = 0.0f;
		HPEffect3->Off();


	}
	{
		MonsterCollision = CreateComponent<GameEngineCollision>();
		MonsterCollision->GetTransform().SetLocalScale({ 108,120,1000.0f });
		MonsterCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });
		MonsterCollision->ChangeOrder((int)(OBJECTORDER::Monster));
	}
	{
		TriggerCollision = CreateComponent<GameEngineCollision>();
		TriggerCollision->GetTransform().SetLocalScale({ 1000,80,1000.0f });
		TriggerCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });
	}
	{
		MonsterRenderer->CreateFrameAnimationCutTexture("Turn1",
			FrameAnimation_DESC("Climber_deathroll_01-Sheet.png", 0, 2, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Turn2",
			FrameAnimation_DESC("Climber_deathroll_01-Sheet.png", 2, 4, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Turn3",
			FrameAnimation_DESC("Climber_deathroll_01-Sheet.png", 4, 6, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Turn4",
			FrameAnimation_DESC("Climber_deathroll_01-Sheet.png", 6, 7, 0.1f, false));

		MonsterRenderer->CreateFrameAnimationCutTexture("Move",
			FrameAnimation_DESC("Climber_walk0000-Sheet.png", 0, 3, 0.1f, true));

		MonsterRenderer->CreateFrameAnimationCutTexture("Death",
			FrameAnimation_DESC("Climber_climber_death_v02000-Sheet.png", 0, 5, 0.1f, false));
	}


	{
		StateManager.CreateStateMember("Trun"
			, std::bind(&Climber::TrunUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::TrunStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Move"
			, std::bind(&Climber::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::MoveStart, this, std::placeholders::_1)
		);

		StateManager.CreateStateMember("Death"
			, std::bind(&Climber::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::DeathStart, this, std::placeholders::_1)
		);


		StateManager.ChangeState("Idle");
	}
}
CollisionReturn Climber::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (Health == 0)
	{
		{
			HPEffect1->GetPixelData().MulColor.a = 0.8f;

			HPEffect2->On();
			HPEffect2->CurAnimationReset();

			HPEffect3->On();
			HPEffect3->CurAnimationReset();
			HPEffect3->GetPixelData().MulColor.a = 1.0f;

		}

		StateManager.ChangeState("Death");
		return CollisionReturn::Break;
	}
	if (StateManager.GetCurStateStateName() == "Move")
	{
		Health -= 1;

		{
			HPEffect1->GetPixelData().MulColor.a = 0.8f;

			HPEffect2->On();
			HPEffect2->CurAnimationReset();

		}


		StateManager.ChangeState("Back");
	}

	return CollisionReturn::ContinueCheck;
}

void Climber::EffectUpdate()
{
	if (HPEffect1->GetPixelData().MulColor.a != 0.0f)
	{
		HPEffect1->GetPixelData().MulColor.a -= 0.6f * GameEngineTime::GetDeltaTime();
		if (HPEffect1->GetPixelData().MulColor.a < 0.0f)
		{
			HPEffect1->GetPixelData().MulColor.a = 0.0f;
		}
	}
	if (HPEffect3->GetPixelData().MulColor.a != 0.0f)
	{
		HPEffect3->GetPixelData().MulColor.a -= 0.6f * GameEngineTime::GetDeltaTime();
		if (HPEffect3->GetPixelData().MulColor.a < 0.0f)
		{
			HPEffect3->GetPixelData().MulColor.a = 0.0f;
		}
	}
	HPEffect2->AnimationBindEnd("Start", [=](const FrameAnimation_DESC& _Info)
		{
			HPEffect2->Off();
		});
	HPEffect3->AnimationBindEnd("Start", [=](const FrameAnimation_DESC& _Info)
		{
			HPEffect3->Off();
		});

}
void Climber::Update(float _DeltaTime)
{

	MonsterCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&Climber::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);
	if (IsDeath_ == true)
	{
		Gravity();
	}
	EffectUpdate();
	StateManager.Update(_DeltaTime);
}


/////////픽셀충돌 중력 함수/////////////////////////////
void Climber::Gravity()
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
		OnGround = true;
		return;
	}
	else
	{
		OnGround = false;
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

bool Climber::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorRDown = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 54,
		-GetTransform().GetWorldPosition().iy() - 1);

	if (false == ColorRDown.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}

	return false;

}


///////////State 함수//////////////////////////////////////

void Climber::TrunStart(const StateInfo& _Info)
{

}
void Climber::TrunUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Climber::MoveStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Move");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Climber::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{

	GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
	MonsterRenderer->GetTransform().PixLocalPositiveX();


}

void Climber::DeathStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Death");
	MonsterRenderer->ScaleToCutTexture(0);
	MonsterCollision->Off();

	GeoCoin* Coin = GetLevel()->CreateActor<GeoCoin>();
	Coin->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ 10,-6,0 });
	GeoCoin* Coin1 = GetLevel()->CreateActor<GeoCoin>();
	Coin1->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ -10,-10,0 });
	GeoCoin* Coin2 = GetLevel()->CreateActor<GeoCoin>();
	Coin2->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ 28,-23,0 });
	GeoCoin* Coin3 = GetLevel()->CreateActor<GeoCoin>();
	Coin3->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ -20,0,0 });
	GeoCoin* Coin4 = GetLevel()->CreateActor<GeoCoin>();
	Coin4->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ -34,0,0 });
	GeoCoin* Coin5 = GetLevel()->CreateActor<GeoCoin>();
	Coin5->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ 17,0,0 });
	GeoCoin* Coin6 = GetLevel()->CreateActor<GeoCoin>();
	Coin6->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ 40,0,0 });


}
void Climber::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

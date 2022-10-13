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
	, TurnNum(1)
	, MoveTime(0.0f)
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
		MonsterCollision->GetTransform().SetLocalScale({ 80,80,1000.0f });
		MonsterCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,40.0f,0 });
		MonsterCollision->ChangeOrder((int)(OBJECTORDER::Monster));
	}

	{
		MonsterRenderer->CreateFrameAnimationCutTexture("Turn1",
			FrameAnimation_DESC("Climber_deathroll_01-Sheet.png", 0, 1, 0.2f, false));


		MonsterRenderer->CreateFrameAnimationCutTexture("Move",
			FrameAnimation_DESC("Climber_walk0000-Sheet.png", 0, 3, 0.1f, true));

		MonsterRenderer->CreateFrameAnimationCutTexture("Death",
			FrameAnimation_DESC("Climber_climber_death_v02000-Sheet.png", 0, 4, 0.1f, false));
	}


	{
		StateManager.CreateStateMember("Move"
			, std::bind(&Climber::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::MoveStart, this, std::placeholders::_1)
		);

		StateManager.CreateStateMember("Turn"
			, std::bind(&Climber::TurnUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::TurnStart, this, std::placeholders::_1)
		);

		StateManager.CreateStateMember("Death"
			, std::bind(&Climber::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Climber::DeathStart, this, std::placeholders::_1)
		);


		StateManager.ChangeState("Move");
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
		SoundPlayer.Stop();
		SoundPlayer = GameEngineSound::SoundPlayControl("spikes_arm_1.wav");
		{
			HPEffect1->GetPixelData().MulColor.a = 0.8f;

			HPEffect2->On();
			HPEffect2->CurAnimationReset();

		}

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
		MonsterRenderer->GetTransform().SetLocalRotation({ 0,0,0 });
		Gravity();
	}
	EffectUpdate();
	StateManager.Update(_DeltaTime);
}


/////////픽셀충돌 중력 함수/////////////////////////////
void Climber::Gravity()
{
	GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
		GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
		GetTransform().GetWorldPosition().z, });

}


///////////State 함수//////////////////////////////////////

void Climber::MoveStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Move");
	MonsterRenderer->ScaleToCutTexture(0);
	MoveTime = 0.0f;
}
void Climber::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MoveTime += 1.0f * _DeltaTime;
	if (TurnNum == 1)
	{
		if (MoveTime > 2.0f)
		{
			StateManager.ChangeState("Turn");
			return;
		}
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * 110 * _DeltaTime);
		MonsterRenderer->GetTransform().SetLocalRotation({ 0,0,0 });
		MonsterCollision->GetTransform().SetLocalPosition({0,40.0f,0});
	}
	else if (TurnNum == 2)
	{
		if (MoveTime > 1.0f)
		{
			StateManager.ChangeState("Turn");
			return;
		}
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * 30 * _DeltaTime);
		MonsterRenderer->GetTransform().SetLocalRotation({ 0,0,-90 });
		MonsterCollision->GetTransform().SetLocalPosition({ 50,0,0 });
	}
	else if (TurnNum == 3)
	{
		if (MoveTime > 2.0f)
		{
			StateManager.ChangeState("Turn");
			return;
		}
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * 110 * _DeltaTime);
		MonsterRenderer->GetTransform().SetLocalRotation({ 0,0,-180 });
		MonsterCollision->GetTransform().SetLocalPosition({ 0,-40,0 });
	}
	else if (TurnNum == 4)
	{
		if (MoveTime > 1.0f)
		{
			StateManager.ChangeState("Turn");
			return;
		}
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * 30 * _DeltaTime);
		MonsterRenderer->GetTransform().SetLocalRotation({ 0,0,-270 });
		MonsterCollision->GetTransform().SetLocalPosition({ -50,0,0 });
	}


}

void Climber::TurnStart(const StateInfo& _Info)
{
	TurnNum += 1;
	if (TurnNum > 4)
	{
		TurnNum = 1;
	}

	MonsterRenderer->ChangeFrameAnimation("Turn1");
	MonsterRenderer->ScaleToCutTexture(0);

}
void Climber::TurnUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (TurnNum == 1)
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * 10 * _DeltaTime);
	}
	else if (TurnNum == 2)
	{
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * 10 * _DeltaTime);
	}
	else if (TurnNum == 3)
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * 10 * _DeltaTime);
	}
	else if (TurnNum == 4)
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * 10 * _DeltaTime);
	}

	MonsterRenderer->GetTransform().SetAddWorldRotation({ 0,0,-225.0f*_DeltaTime });
	MonsterRenderer->AnimationBindEnd("Turn1", [=](const FrameAnimation_DESC& _Info)
		{
			StateManager.ChangeState("Move");
		});




}
void Climber::DeathStart(const StateInfo& _Info)
{
	SoundPlayer.Stop();
	SoundPlayer = GameEngineSound::SoundPlayControl("spikes_arm_3.wav");
	MonsterRenderer->ChangeFrameAnimation("Death");
	MonsterRenderer->ScaleToCutTexture(0);
	MonsterCollision->Off();
	IsDeath_ = true;
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

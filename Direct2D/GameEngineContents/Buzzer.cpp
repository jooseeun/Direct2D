#include "Buzzer.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"
#include "Player.h"
#include "GeoCoin.h"
#include <iostream>

Buzzer::Buzzer()
	: MonsterRenderer(nullptr)
	, MonsterCollision(nullptr)
	, TriggerCollision(nullptr)
	, StateManager()
	, Speed(150)
	, Health(3)
	, FallTime(0)
	, OnGround(false)
	, HPEffect1(nullptr)
	, HPEffect2(nullptr)
	, HPEffect3(nullptr)
{
}

Buzzer::~Buzzer()
{
}

void Buzzer::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		MonsterRenderer = CreateComponent< GameEngineTextureRenderer>();
		MonsterRenderer->GetTransform().SetLocalScale({ 86, 136, 10.0f });
		MonsterRenderer->SetPivot(PIVOTMODE::CUSTOM);
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
		MonsterCollision->GetTransform().SetLocalScale({ 100,130,1000.0f });
		MonsterCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,50.0f,0 });
		MonsterCollision->ChangeOrder((int)(OBJECTORDER::Monster));
	}
	{
		TriggerCollision = CreateComponent<GameEngineCollision>();
		TriggerCollision->GetTransform().SetLocalScale({ 1300,1300,1000.0f });
		TriggerCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,50.0f,0 });
	}
	{
		MonsterRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Buzzer_idle_01-Sheet.png", 0, 4, 0.1f, true));
		MonsterRenderer->CreateFrameAnimationCutTexture("Move",
			FrameAnimation_DESC("Buzzer_fly0000-Sheet.png", 0, 3, 0.1f, true));
		MonsterRenderer->CreateFrameAnimationCutTexture("Death",
			FrameAnimation_DESC("Buzzer_death0000-Sheet.png", 0, 2, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Startle",
			FrameAnimation_DESC("Buzzer_startle_01-Sheet.png", 0, 3, 0.1f, false));

	}


	{
		StateManager.CreateStateMember("Idle"
			, std::bind(&Buzzer::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Buzzer::IdleStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Startle"
			, std::bind(&Buzzer::StartleUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Buzzer::StartleStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Move"
			, std::bind(&Buzzer::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Buzzer::MoveStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Back"
			, std::bind(&Buzzer::BackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Buzzer::BackStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Death"
			, std::bind(&Buzzer::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Buzzer::DeathStart, this, std::placeholders::_1)
		);


		StateManager.ChangeState("Idle");
	}
}
CollisionReturn Buzzer::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
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
CollisionReturn Buzzer::CheckTrigger(GameEngineCollision* _This, GameEngineCollision* _Other)
{

	StateManager.ChangeState("Startle");
	return CollisionReturn::ContinueCheck;
}
void Buzzer::EffectUpdate()
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

void Buzzer::Update(float _DeltaTime)
{
	if (StateManager.GetCurStateStateName() == "Idle")
	{
		TriggerCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
			std::bind(&Buzzer::CheckTrigger, this, std::placeholders::_1, std::placeholders::_2));

	}
	EffectUpdate();
	MonsterCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&Buzzer::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);
	if (StateManager.GetCurStateStateName() == "Death")
	{
		Gravity();
	}
	StateManager.Update(_DeltaTime);
}


/////////픽셀충돌 중력 함수/////////////////////////////
void Buzzer::Gravity()
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

bool Buzzer::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 40,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() - 40,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorUp = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 136);

	if (false == ColorUp.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}

	if (CurDir == MonsterDIR::Left)
	{
		if (false == ColorL.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}

	if (CurDir == MonsterDIR::Right)
	{
		if (false == ColorR.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}


	return false;

}


///////////State 함수//////////////////////////////////////

void Buzzer::IdleStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Idle");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Buzzer::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Buzzer::MoveStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Move");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Buzzer::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 MovePos = Player::GetMainPlayer()->GetTransform().GetLocalPosition() + float4{0,60,0} - GetTransform().GetLocalPosition();
	float MoveLen = MovePos.Length();
	if (MovePos.x < 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
		MonsterRenderer->GetTransform().PixLocalPositiveX();

	}
	if (MovePos.x >= 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
		MonsterRenderer->GetTransform().PixLocalNegativeX();

	}

	if (MovePos.y < 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed * _DeltaTime);

	}
	if (MovePos.y >= 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);

	}


}
void Buzzer::StartleStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Startle");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Buzzer::StartleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MonsterRenderer->AnimationBindEnd("Startle", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Move");
	});
}
void Buzzer::BackStart(const StateInfo& _Info)
{
	FallTime = 0.4f;
}
void Buzzer::BackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (FallTime < 0.0f)
	{
		StateManager.ChangeState("Move");
	}

	float4 MovePos = Player::GetMainPlayer()->GetTransform().GetLocalPosition() - GetTransform().GetLocalPosition();
	float MoveLen = MovePos.Length();
	if (FallTime > 0.0f)
	{
		FallTime -= 1.0f * _DeltaTime;
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y + 300.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });

	}

	if (MovePos.x < 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed  * _DeltaTime);
		MonsterRenderer->GetTransform().PixLocalPositiveX();

	}
	if (MovePos.x >= 0.0f)
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed  * _DeltaTime);
		MonsterRenderer->GetTransform().PixLocalNegativeX();

	}
}

void Buzzer::DeathStart(const StateInfo& _Info)
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
void Buzzer::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}
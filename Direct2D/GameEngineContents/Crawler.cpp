#include "Crawler.h"
#include "PreCompile.h"
#include "PlayLevelManager.h"
#include "Player.h"
#include <iostream>

Crawler::Crawler() 
	: MonsterRenderer(nullptr)
	, MonsterCollision(nullptr)
	, TriggerCollision(nullptr)
	, StateManager()
	, Speed(100)
{
}

Crawler::~Crawler() 
{
}

void Crawler::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		MonsterRenderer = CreateComponent< GameEngineTextureRenderer>();
		MonsterRenderer->GetTransform().SetLocalScale({ 108, 80, 10.0f });
		MonsterRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		MonsterCollision = CreateComponent<GameEngineCollision>();
		MonsterCollision->GetTransform().SetLocalScale({ 108,80,1000.0f });
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
		MonsterRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Crawler_walk.png", 0, 0, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Move",
			FrameAnimation_DESC("Crawler_walk.png", 0, 3, 0.1f, true));
	}


	{
		StateManager.CreateStateMember("Idle"
			, std::bind(&Crawler::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Crawler::IdleStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Move"
			, std::bind(&Crawler::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Crawler::MoveStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Attack"
			, std::bind(&Crawler::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&Crawler::AttackStart, this, std::placeholders::_1)
		);


		StateManager.ChangeState("Idle");
	}
}
bool Crawler::CheckTrigger(GameEngineCollision* _This, GameEngineCollision* _Other)
{

	StateManager.ChangeState("Move");
	return true;
}
void Crawler::Update(float _DeltaTime)
{

	TriggerCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&Crawler::CheckTrigger, this, std::placeholders::_1, std::placeholders::_2)
	);

	Gravity();
	StateManager.Update(_DeltaTime);
}

void Crawler::IdleStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Idle");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Crawler::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Crawler::MoveStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Move");
	MonsterRenderer->ScaleToCutTexture(0);
}
void Crawler::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float4 MovePos = Player::GetMainPlayer()->GetTransform().GetLocalPosition() - GetTransform().GetLocalPosition();
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


}

void Crawler::AttackStart(const StateInfo& _Info)
{

}
void Crawler::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}
void Crawler::DeathStart(const StateInfo& _Info)
{

}
void Crawler::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Crawler::Gravity()
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

		return;
	}
	else
	{
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

bool Crawler::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 54,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() - 54,
		-GetTransform().GetWorldPosition().iy() - 5);
	float4 ColorUp = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 80);

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
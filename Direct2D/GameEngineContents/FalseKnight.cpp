#include "FalseKnight.h"
#include "PlayLevelManager.h"
#include "Player.h"
#include "PreCompile.h"

FalseKnight::FalseKnight()
	: StateManager()
	, MonsterRenderer(nullptr)
	, HPEffect1(nullptr)
	, HPEffect2(nullptr)
	, HPEffect3(nullptr)
	, MonsterCollision(nullptr)
	, CurDir(MonsterDIR::Left)
	, Speed(200.0f)
	, JumpTime(2.0f)
	, Health(15)
	, OnGround(false)
	, StopTime(0.0f)
{
}

FalseKnight::~FalseKnight() 
{
}

void FalseKnight::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		MonsterRenderer = CreateComponent< GameEngineTextureRenderer>();
		MonsterRenderer->SetOrder((int)OBJECTORDER::Monster);
		MonsterRenderer->SetPivot(PIVOTMODE::BOT);
		MonsterRenderer->GetTransform().SetLocalPosition({ 0,-40,0 });
	}

	{
		MonsterCollision = CreateComponent<GameEngineCollision>();
		MonsterCollision->GetTransform().SetLocalScale({ 330,300,1000.0f });
		MonsterCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,110,0 });
		MonsterCollision->ChangeOrder((int)(OBJECTORDER::Monster));
	}

	{
		MonsterRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("False Knight_idle0000-Sheet.png", 0, 4, 0.1f, true));
		MonsterRenderer->CreateFrameAnimationCutTexture("Fall",
			FrameAnimation_DESC("False Knight_jump0002-Sheet.png", 0, 2, 0.1f, true));
		MonsterRenderer->CreateFrameAnimationCutTexture("Land",
			FrameAnimation_DESC("False Knight_jump_antic0000-Sheet.png", 0, 2, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("Jump",
			FrameAnimation_DESC("False Knight_jump0002-Sheet.png", 0, 2, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("BackJump",
			FrameAnimation_DESC("False Knight_jump0002-Sheet.png", 0, 2, 0.1f, false));
	}


	{
		StateManager.CreateStateMember("Idle"
			, std::bind(&FalseKnight::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::IdleStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Jump"
			, std::bind(&FalseKnight::JumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::JumpStart, this, std::placeholders::_1)
		);		
		StateManager.CreateStateMember("BackJump"
			, std::bind(&FalseKnight::JumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::JumpStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Fall"
			, std::bind(&FalseKnight::FallUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::FallStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("JumpAttack"
			, std::bind(&FalseKnight::JumpAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::JumpAttackStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("GroundAttack"
			, std::bind(&FalseKnight::GroundAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::GroundAttackStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("HitAttack"
			, std::bind(&FalseKnight::HitAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::HitAttackStart, this, std::placeholders::_1)
		);
	
		StateManager.CreateStateMember("DropAttack"
			, std::bind(&FalseKnight::DropAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::DropAttackStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Land"
			, std::bind(&FalseKnight::LandUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::LandStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Stun"
			, std::bind(&FalseKnight::StunUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::StunStart, this, std::placeholders::_1)
		);
		StateManager.CreateStateMember("Death"
			, std::bind(&FalseKnight::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::DeathStart, this, std::placeholders::_1)
		);
	}

	StateManager.ChangeState("Idle");
}

void FalseKnight::Update(float _DeltaTime)
{
	Gravity();
	StateManager.Update(_DeltaTime);
}

void FalseKnight::Gravity()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("�浹�� ���� ���õ��� �ʾҽ��ϴ�");
	}
	float4 Color = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 1);

	if (false == Color.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		OnGround = true;
		if (StateManager.GetCurStateStateName() == "Fall")
		{
			MonsterRenderer->ChangeFrameAnimation("Land");
			MonsterRenderer->ScaleToCutTexture(0);
			StateManager.ChangeState("Land");
		}
		return;
	}
	else
	{
		if (StateManager.GetCurStateStateName() == "Idle")
		{
			StateManager.ChangeState("Fall");
		}
		OnGround = false;
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}
}
bool FalseKnight::MapPixelCheck()
{
	return true;
}

CollisionReturn FalseKnight::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{

	return CollisionReturn::ContinueCheck;
}

void FalseKnight::CheckMonsterDir()
{
	float4 MovePos = Player::GetMainPlayer()->GetTransform().GetLocalPosition() -GetTransform().GetLocalPosition();
	float MoveLen = MovePos.Length();
	if (MovePos.x < 0.0f)
	{
		CurDir = MonsterDIR::Left;

	}
	if (MovePos.x > 0.0f)
	{
		CurDir = MonsterDIR::Right;
	}
}
///////////////////////State ���� �Լ�/////////////////////////////////////////////////////

void FalseKnight::IdleStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Idle");
	MonsterRenderer->ScaleToCutTexture(0);
}
void FalseKnight::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	//StateManager.ChangeState("Jump");
}

void FalseKnight::JumpStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Jump");
	MonsterRenderer->ScaleToCutTexture(0);
	JumpTime = 1.3f;
	CheckMonsterDir();
}
void FalseKnight::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	JumpTime -= 1.0f * _DeltaTime;
	if (JumpTime <= 0.0f)
	{
		StateManager.ChangeState("Fall");
	} 
	else if(JumpTime > 0.3f)
	{

		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
GetTransform().GetWorldPosition().y + 1200 * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
	}
	else
	{
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
	}
}
void FalseKnight::BackJumpStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("BackJump");
	MonsterRenderer->ScaleToCutTexture(0);
	JumpTime = 1.0f;
	CheckMonsterDir();
}
void FalseKnight::BackJumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	JumpTime -= 1.0f * _DeltaTime;
	if (JumpTime <= 0.0f)
	{
		StateManager.ChangeState("Fall");
	}
	else if (JumpTime > 0.3f)
	{

		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
GetTransform().GetWorldPosition().y + 1200 * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
	}
	else
	{
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
	}
}
void FalseKnight::LandStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Land");
	MonsterRenderer->ScaleToCutTexture(0);
}

void FalseKnight::LandUpdate(float _DeltaTime, const StateInfo& _Info)
{

	MonsterRenderer->AnimationBindEnd("Land", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});

}


void FalseKnight::FallStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Fall");
	MonsterRenderer->ScaleToCutTexture(0);
	CheckMonsterDir();
}
void FalseKnight::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.PrevState == "BackJump")
	{
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
	}
	else
	{
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
	}

}

void FalseKnight::JumpAttackStart(const StateInfo& _Info)
{

}
void FalseKnight::JumpAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void FalseKnight::GroundAttackStart(const StateInfo& _Info)
{

}
void FalseKnight::GroundAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void FalseKnight::HitAttackStart(const StateInfo& _Info)
{

}
void FalseKnight::HitAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void FalseKnight::DropAttackStart(const StateInfo& _Info)
{

}
void FalseKnight::DropAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void FalseKnight::StunStart(const StateInfo& _Info)
{

}
void FalseKnight::StunUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void FalseKnight::DeathStart(const StateInfo& _Info)
{

}
void FalseKnight::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}
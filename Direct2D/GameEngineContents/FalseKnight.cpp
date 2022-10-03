#include "FalseKnight.h"
#include "PlayLevelManager.h"
#include "Player.h"
#include "PreCompile.h"
#include "FalseKnightWaveSkill.h"
#include "FalseKnightFireBall.h"
#include <GameEngineBase/GameEngineRandom.h>

FalseKnight::FalseKnight()
	: StateManager()
	, MonsterRenderer(nullptr)
	, HPEffect1(nullptr)
	, HPEffect2(nullptr)
	, HPEffect3(nullptr)
	, MonsterCollision(nullptr)
	, MonsterHeadRenderer(nullptr)
	, LeftSkillCol(nullptr)
	, RightSkillCol(nullptr)
	, MonsterHeadCollision(nullptr)
	, CurDir(MonsterDIR::Left)
	, Speed(200.0f)
	, JumpTime(2.0f)
	, Health(10)
	, OnGround(false)
	, StopTime(0.0f)
	, SkillTime(0.0f)
	, AttackTime(0.0f)
	, DeathNum(3)
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
		MonsterHeadRenderer = CreateComponent< GameEngineTextureRenderer>();
		MonsterHeadRenderer->SetOrder((int)OBJECTORDER::Monster);
		MonsterHeadRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		MonsterCollision = CreateComponent<GameEngineCollision>();
		MonsterCollision->GetTransform().SetLocalScale({ 290,300,1000.0f });
		MonsterCollision->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,160,0 });
		MonsterCollision->ChangeOrder((int)(OBJECTORDER::Monster));
	}
	{
		MonsterHeadCollision = CreateComponent<GameEngineCollision>();
		MonsterHeadCollision->GetTransform().SetLocalScale({ 150,150,1000.0f });
		MonsterHeadCollision->ChangeOrder((int)(OBJECTORDER::Monster));			
	}
	{
		LeftSkillCol = CreateComponent<GameEngineCollision>();
		LeftSkillCol->GetTransform().SetLocalScale({ 330,290,1000.0f });
		LeftSkillCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ -300,0,0 });
		LeftSkillCol->ChangeOrder((int)(OBJECTORDER::Monster));
	}
	{
		RightSkillCol = CreateComponent<GameEngineCollision>();
		RightSkillCol->GetTransform().SetLocalScale({ 330,290,1000.0f });
		RightSkillCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ +300,0,0 });
		RightSkillCol->ChangeOrder((int)(OBJECTORDER::Monster));
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

		MonsterRenderer->CreateFrameAnimationCutTexture("JumpAttack1",
			FrameAnimation_DESC("False Knight_jump_attack0001-Sheet.png", 0, 4, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("JumpAttack2",
			FrameAnimation_DESC("False Knight_jump_attack0001-Sheet.png", 5, 10, 0.15f, false));


		MonsterRenderer->CreateFrameAnimationCutTexture("GroundAttack1",
			FrameAnimation_DESC("False Knight_attack0000-Sheet.png", 0, 3, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("GroundAttack2",
			FrameAnimation_DESC("False Knight_attack0005-Sheet.png", 0, 7, 0.1f, false));

		MonsterRenderer->CreateFrameAnimationCutTexture("DropAttack1",
			FrameAnimation_DESC("False Knight_attack0005-Sheet.png", 0, 4, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("DropAttack2",
			FrameAnimation_DESC("False Knight_attack0005-Sheet.png", 0, 4, 0.1f, false));


		MonsterRenderer->CreateFrameAnimationCutTexture("HitAttack",
			FrameAnimation_DESC("False Knight_attack0005-Sheet.png", 0, 7, 0.1f, false));

		MonsterRenderer->CreateFrameAnimationCutTexture("StunStart",
			FrameAnimation_DESC("False Knight_stun0000-Sheet.png", 0, 8, 0.1f, false));
		MonsterRenderer->CreateFrameAnimationCutTexture("StunIdle",
			FrameAnimation_DESC("False Knight_stun_damage0000-Sheet.png", 0, 2, 0.1f, false));
		MonsterHeadRenderer->CreateFrameAnimationCutTexture("StunIdle",
			FrameAnimation_DESC("False Knight_stun_crop0000-Sheet.png", 0, 2, 0.1f, false));
		MonsterHeadRenderer->CreateFrameAnimationCutTexture("StunStart",
			FrameAnimation_DESC("False Knight_stun_head_idle0000-Sheet.png", 0, 4, 0.1f, false));
		MonsterHeadRenderer->Off();
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
			, std::bind(&FalseKnight::BackJumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
			, std::bind(&FalseKnight::BackJumpStart, this, std::placeholders::_1)
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
	LeftSkillCol->Off();
	RightSkillCol->Off();
}

void FalseKnight::Update(float _DeltaTime)
{
	Gravity();
	UpdateDamage();
	EffectUpdate();
	StateManager.Update(_DeltaTime);
}

void FalseKnight::Gravity()
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
void FalseKnight::UpdateDamage()
{
 	AttackTime -= 1.0f * GameEngineTime::GetDeltaTime();


	if (AttackTime <= 0)
	{
	MonsterCollision->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::Skill, CollisionType::CT_SPHERE,
		std::bind(&FalseKnight::CheckDemage, this, std::placeholders::_1, std::placeholders::_2));


	if (true == MonsterHeadCollision->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::Skill, CollisionType::CT_SPHERE,
			std::bind(&FalseKnight::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)))
	{
		MonsterHeadRenderer->ChangeFrameAnimation("StunIdle");
		MonsterHeadRenderer->CurAnimationReset();
		MonsterHeadRenderer->ScaleToCutTexture(0);
		{
			HPEffect3->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
			HPEffect1->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
			HPEffect2->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
			HPEffect1->GetPixelData().MulColor.a = 0.8f;
			HPEffect2->On();
			HPEffect2->CurAnimationReset();

		}
	}
	}
}



CollisionReturn FalseKnight::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	AttackTime = 0.5f;
	Health -= 1;

	if (Health == 0)
	{
		if (StateManager.GetCurStateStateName() != "Stun")
		{
			StateManager.ChangeState("Stun");
			Health = 8;
		}

		else
		{
			StateManager.ChangeState("Idle");
			Health = 8;
		}
		HPEffect3->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
		HPEffect1->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
		HPEffect2->GetTransform().SetLocalPosition({ MonsterHeadCollision->GetTransform().GetLocalPosition() });
		HPEffect1->GetPixelData().MulColor.a = 0.8f;

		HPEffect2->On();
		HPEffect2->CurAnimationReset();

		HPEffect3->On();
		HPEffect3->CurAnimationReset();
		HPEffect3->GetPixelData().MulColor.a = 1.0f;
	}
	return CollisionReturn::ContinueCheck;
}

void FalseKnight::EffectUpdate()
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

void FalseKnight::MakeWave()
{
	FalseKnightWaveSkill* Skill = GetLevel()->CreateActor<FalseKnightWaveSkill>();
	CheckMonsterDir();
	Skill->CurDir = CurDir;
	if (CurDir == MonsterDIR::Left)
	{
		Skill->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ -300,0,0 });
	}
	else if (CurDir == MonsterDIR::Right)
	{
		Skill->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4{ 300,0,0 });
	}
}

void FalseKnight::DropBall()
{
	for (int i = 0; i < 6; i++)
	{
		FalseKnightFireBall* Skill = GetLevel()->CreateActor<FalseKnightFireBall>();
		Skill->GetTransform().SetLocalPosition(float4{ GameEngineRandom::MainRandom.RandomFloat(1600.0f, 2900.0f),-GameEngineRandom::MainRandom.RandomFloat(400.0f, 900.0f),0 });
	}

}
///////////////////////State 관련 함수/////////////////////////////////////////////////////

void FalseKnight::IdleStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("Idle");
	MonsterRenderer->ScaleToCutTexture(0);
	AttackNum=GameEngineRandom::MainRandom.RandomInt(1, 6);
	SkillTime = 2.0f;
}

void FalseKnight::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	
	{
	StateManager.ChangeState("Stun");
	return;
	}
	float4 MovePos = Player::GetMainPlayer()->GetTransform().GetLocalPosition() - GetTransform().GetLocalPosition();
	float MoveLen = MovePos.Length();

	//if (MovePos.x < 200.0f && MovePos.x > -200.0f) // 가까우면
	//{
	//	StateManager.ChangeState("BackJump");

	//}

	SkillTime -= 1.0f * _DeltaTime;
	if (SkillTime > 0.0f)
	{
		return;
	}

	switch (AttackNum)
	{
	case 1:
	{
		StateManager.ChangeState("Jump");
		break;

	case 2:
	{
		StateManager.ChangeState("BackJump");
		break;
	}
	case 3:
	{
		StateManager.ChangeState("JumpAttack");
		break;
	}
	case 4:
	{
		StateManager.ChangeState("GroundAttack");
		break;
	}
	case 5:
	{
		StateManager.ChangeState("HitAttack");
		break;
	}
	case 6:
	{
		StateManager.ChangeState("DropAttack");
		break;
	}

	}

	}

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
			MonsterRenderer->GetTransform().PixLocalPositiveX();

		}
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();
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
			MonsterRenderer->GetTransform().PixLocalPositiveX();

		}
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();
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
void FalseKnight::GroundAttackStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("GroundAttack1");
	MonsterRenderer->ScaleToCutTexture(0);
	CheckMonsterDir();
	IsMakeWave = true;
	SkillTime = 5.0f;
}
void FalseKnight::GroundAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MonsterRenderer->AnimationBindEnd("GroundAttack1", [=](const FrameAnimation_DESC& _Info)
	{
		MonsterRenderer->ChangeFrameAnimation("GroundAttack2");
		MonsterRenderer->ScaleToCutTexture(0);
		SkillTime = 0.3f;
	});
	MonsterRenderer->AnimationBindEnd("GroundAttack2", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});

	SkillTime -= 1.0f * _DeltaTime;
	if (SkillTime < 0.0f && IsMakeWave==true)
	{
		MakeWave();
		IsMakeWave = false;
	}
}

void FalseKnight::JumpAttackStart(const StateInfo& _Info)
{ 
	MonsterRenderer->ChangeFrameAnimation("JumpAttack1");
	MonsterRenderer->ScaleToCutTexture(0);
	CheckMonsterDir();
	JumpTime = 0.5f;
}
void FalseKnight::JumpAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MonsterRenderer->AnimationBindEnd("JumpAttack1", [=](const FrameAnimation_DESC& _Info)
	{
		MonsterRenderer->ChangeFrameAnimation("JumpAttack2");
		SkillTime = 0.3f;
	});
	MonsterRenderer->AnimationBindEnd("JumpAttack2", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});
	
	SkillTime -= 1.0f * _DeltaTime;
	JumpTime -= 1.0f * _DeltaTime;

	if (SkillTime > 0.0f && SkillTime < 0.15f)
	{
		if (CurDir == MonsterDIR::Left)
		{
			LeftSkillCol->On();
		}
		else
		{
			RightSkillCol->On();
		}
	}
	else
	{
		LeftSkillCol->Off();
		RightSkillCol->Off();

	}

	if (JumpTime > 0.0f)
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
GetTransform().GetWorldPosition().y + 1200.0f * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
	}

	else
	{
		if (CurDir == MonsterDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed*0.5f * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalNegativeX();

		}
		if (CurDir == MonsterDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed* 0.5f * _DeltaTime);
			MonsterRenderer->GetTransform().PixLocalPositiveX();
		}
	}
}
void FalseKnight::DropAttackStart(const StateInfo& _Info)
{
	MonsterRenderer->ChangeFrameAnimation("DropAttack1");
	MonsterRenderer->GetTransform().PixLocalPositiveX(); 
	MonsterRenderer->ScaleToCutTexture(0);
	SkillNum = 10;
}
void FalseKnight::DropAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (SkillNum == 5 || SkillNum == 8)
	{
		DropBall();		
		SkillNum -= 1;
	}

	MonsterRenderer->AnimationBindEnd("DropAttack1", [=](const FrameAnimation_DESC& _Info)
	{
		SkillNum -= 1;
		MonsterRenderer->ChangeFrameAnimation("DropAttack2");
		MonsterRenderer->GetTransform().PixLocalNegativeX();
		LeftSkillCol->On();
		RightSkillCol->Off();
	});

	MonsterRenderer->AnimationBindEnd("DropAttack2", [=](const FrameAnimation_DESC& _Info)
	{
		SkillNum -= 1;
		MonsterRenderer->ChangeFrameAnimation("DropAttack1");
		MonsterRenderer->GetTransform().PixLocalPositiveX();
		RightSkillCol->On();
		LeftSkillCol->Off();
		if (SkillNum < 0)
		{
			StateManager.ChangeState("Idle");
			LeftSkillCol->Off();
			RightSkillCol->Off();

		}
	});

}
void FalseKnight::HitAttackStart(const StateInfo& _Info)
{
	CheckMonsterDir();
	MonsterRenderer->ChangeFrameAnimation("HitAttack");
	if (CurDir == MonsterDIR::Left)
	{
		MonsterRenderer->GetTransform().PixLocalNegativeX();
	}
	else
	{
		MonsterRenderer->GetTransform().PixLocalPositiveX();
	}
	MonsterRenderer->ScaleToCutTexture(0);

	SkillTime = 0.3f;
}
void FalseKnight::HitAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MonsterRenderer->AnimationBindEnd("HitAttack", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});

	SkillTime -= 1.0f * _DeltaTime;
	JumpTime -= 1.0f * _DeltaTime;

	if (SkillTime > 0.0f && SkillTime < 0.2f)
	{
		if (CurDir == MonsterDIR::Left)
		{
			LeftSkillCol->On();
		}
		else
		{
			RightSkillCol->On();
		}
	}
	else
	{
		LeftSkillCol->Off();
		RightSkillCol->Off();

	}
}



void FalseKnight::StunStart(const StateInfo& _Info)
{
	CheckMonsterDir();
	MonsterRenderer->ChangeFrameAnimation("StunStart");
	if (CurDir == MonsterDIR::Left)
	{
		MonsterRenderer->GetTransform().PixLocalNegativeX();
	}
	else
	{
		MonsterRenderer->GetTransform().PixLocalPositiveX();
	}
	MonsterRenderer->ScaleToCutTexture(0);
}
void FalseKnight::StunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	MonsterRenderer->AnimationBindEnd("StunStart", [=](const FrameAnimation_DESC& _Info)
	{
		MonsterHeadRenderer->On();
		MonsterHeadCollision->On();

		MonsterHeadRenderer->ChangeFrameAnimation("StunStart");
		MonsterHeadRenderer->ScaleToCutTexture(0);

		MonsterRenderer->ChangeFrameAnimation("StunIdle");
		MonsterRenderer->ScaleToCutTexture(0);

		if (CurDir == MonsterDIR::Left)
		{
			MonsterRenderer->GetTransform().PixLocalNegativeX();
			MonsterHeadRenderer->GetTransform().PixLocalNegativeX();
			MonsterHeadRenderer->GetTransform().SetLocalPosition({ -140,30,0 });
			MonsterHeadCollision->GetTransform().SetLocalPosition({ -140,30,0 });


		}
		else
		{
			MonsterRenderer->GetTransform().PixLocalPositiveX();
			MonsterHeadRenderer->GetTransform().PixLocalPositiveX();
			MonsterHeadRenderer->GetTransform().SetLocalPosition({ 140,30,0 });
			MonsterHeadCollision-> GetTransform().SetLocalPosition({ 140,30,0 });
		}
	});

}

void FalseKnight::DeathStart(const StateInfo& _Info)
{

}
void FalseKnight::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}
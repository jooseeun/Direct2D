#include "PreCompile.h"
#include "Player.h"
#include "PlayLevelManager.h"
#include "ColorOverlay.h"
#include <iostream>
#include <GameEngineCore/GameEngineBlur.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineDefaultRenderer.h>
#include <GameEngineCore/GameEngineRenderingPipeLine.h>
#include <GameEngineCore/GameEngineVertexShader.h>
#include <GameEngineCore/GameEngineConstantBuffer.h>
#include <GameEngineCore/GameEngineDevice.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>


Player* Player::MainPlayer = nullptr;

GameEngineSoundPlayer Player::MainBGM;
Player::Player()
	: Speed(700.0f)
	, PlayerRenderer(nullptr)
	, CurDir(PLAYERDIR::Right)
	, AttackNum(1)
	, Color(0)
	, MapSize(0)
	, StateManager()
	, JumpTime(0)
	, SkillRenderer(nullptr)
	, FallTime(0)
	, LeftSkilCol(nullptr)
	, RightSkilCol(nullptr)
	, PlayerCol(nullptr)
	, PlayerHealth(5)
	, PlayerFullHealth(5)
	, PlayerEnergyGage(0.1f)
	, GlobalTimeScale(1.0f)
	, StunEffect1Renderer(nullptr)
	, StunEffect2Renderer(nullptr)
	, PlayerLightRenderer(nullptr)
	, ShakeRight(false)
	, ShakeTime(0.0f)
	, PlayerGeoCoin(0)
	, CameraShake(false)
	, HitRenderer1(nullptr)
	, HitRenderer2(nullptr)
	, CoinEffectRenderer(nullptr)
	, ChargeTime(0.0f)
	, ObjectShakeCamera(false)
	, IsTrapStun(false)
	, SkillTime(0.0f)
	, ChargeEffect1(nullptr)
	, CurLevelName()
	, OneShakeCheck(0.0f)
	, IsSuperMode(false)
{
	MainPlayer = this;
}

Player::~Player()
{
}


void Player::Start()
{
	GameEngineDevice::GetBackBuffer()->AddEffect<GameEngineBlur>();
	GetLevel()->GetMainCamera()->GetCameraRenderTarget()->AddEffect<GameEngineBlur>();

	if (false == GameEngineInput::GetInst()->IsKey("PlayerLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("PlayerLeft", VK_LEFT);
		GameEngineInput::GetInst()->CreateKey("PlayerRight", VK_RIGHT);
		GameEngineInput::GetInst()->CreateKey("PlayerUp", VK_UP);
		GameEngineInput::GetInst()->CreateKey("PlayerDown", VK_DOWN);
		GameEngineInput::GetInst()->CreateKey("PlayerJump", 'Z');
		GameEngineInput::GetInst()->CreateKey("PlayerAttack", 'X');
		GameEngineInput::GetInst()->CreateKey("PlayerCharge", 'A');
		GameEngineInput::GetInst()->CreateKey("PlayerSuper", 'C');
	}
	
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		PlayerLightRenderer = CreateComponent<GameEngineTextureRenderer>();
		PlayerLightRenderer->SetOrder((int)OBJECTORDER::LightEffect);
		PlayerLightRenderer->SetPivot(PIVOTMODE::CENTER);
		PlayerLightRenderer->SetTexture("light_effect_v02.png");
		PlayerLightRenderer->GetTransform().SetLocalScale({ 256.0f * 5.0f,216.0f * 5.0f,100.0f });
		PlayerLightRenderer->GetTransform().SetLocalPosition({ 0.0f ,50.0f,50.0f });
		PlayerLightRenderer->GetPipeLine()->SetOutputMergerBlend("AlphaBlend");
		PlayerLightRenderer->GetPixelData().MulColor.b += 0.03f;
		PlayerLightRenderer->GetPixelData().MulColor.a = 0.6f;
	}
	{
		PlayerRenderer = CreateComponent<GameEngineTextureRenderer>();
		PlayerRenderer->SetOrder((int)OBJECTORDER::Player);
		PlayerRenderer->SetPivot(PIVOTMODE::BOT);
	}

	{
		SkillRenderer = CreateComponent<GameEngineTextureRenderer>();
		SkillRenderer->SetOrder((int)OBJECTORDER::Player);
		SkillRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		HitRenderer1 = CreateComponent<GameEngineTextureRenderer>();
		HitRenderer1->SetOrder((int)OBJECTORDER::Player);
		HitRenderer1->SetPivot(PIVOTMODE::BOT);
	}
	{
		HitRenderer2 = CreateComponent<GameEngineTextureRenderer>();
		HitRenderer2->SetOrder((int)OBJECTORDER::Player);
		HitRenderer2->SetPivot(PIVOTMODE::BOT);
		HitRenderer2->SetPivotToVector({ 0,30,0 });
	}
	{
		StunEffect1Renderer = CreateComponent<GameEngineTextureRenderer>();
		StunEffect1Renderer->SetOrder((int)OBJECTORDER::Player);
		StunEffect1Renderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		StunEffect2Renderer = CreateComponent<GameEngineTextureRenderer>();
		StunEffect2Renderer->SetOrder((int)OBJECTORDER::Player);
		StunEffect2Renderer->SetPivot(PIVOTMODE::CENTER);
	}
	{
		ChargeEffect1 = CreateComponent<GameEngineTextureRenderer>();
		ChargeEffect1->SetOrder((int)OBJECTORDER::Player);
		ChargeEffect1->SetPivot(PIVOTMODE::CENTER);
		ChargeEffect1->SetPivotToVector({ 0,100,0 });
	}
	{
		CoinEffectRenderer = CreateComponent<GameEngineTextureRenderer>();
		CoinEffectRenderer->SetOrder((int)OBJECTORDER::Player);
		CoinEffectRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		PlayerCol = CreateComponent<GameEngineCollision>();
		PlayerCol->GetTransform().SetLocalScale({ 66.0f,125.0f,1000.0f });
		PlayerCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,62.5f,0 });
		PlayerCol->ChangeOrder((int)OBJECTORDER::Player);
	}
	{
		LeftSkilCol = CreateComponent<GameEngineCollision>();
		LeftSkilCol->GetTransform().SetLocalScale({ 175.0f , 92.f, 1000.0f });
		LeftSkilCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ -50,62.5f,0 });
		LeftSkilCol->ChangeOrder((int)OBJECTORDER::Skill);
		LeftSkilCol->Off();

	}
	{
		RightSkilCol = CreateComponent<GameEngineCollision>();
		RightSkilCol->GetTransform().SetLocalScale({ 175.0f , 92.f, 1000.0f });
		RightSkilCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 50,62.5f,0 });
		RightSkilCol->ChangeOrder((int)OBJECTORDER::Skill);
		RightSkilCol->Off();
	}

	{
		PlayerRenderer->CreateFrameAnimationCutTexture("IdleHigh",
			FrameAnimation_DESC("PlayerIdleHighHealth.png", 0, 8,0.1f, true));
		PlayerRenderer->CreateFrameAnimationCutTexture("PlayerWalk",
			FrameAnimation_DESC("Player_run.png", 0, 7, 0.1f, true));
		PlayerRenderer->CreateFrameAnimationCutTexture("Player_idle_to_run",
			FrameAnimation_DESC("Player_idle_to_run.png", 0, 4, 0.1f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Player_run_to_idle",
			FrameAnimation_DESC("Player_run_to_idle.png", 0, 5, 0.1f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Roar",
			FrameAnimation_DESC("Player_roar.png", 0, 7, 0.1f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Fall",
			FrameAnimation_DESC("Player_fall.png", 3, 5, 0.1f, true));		
		PlayerRenderer->CreateFrameAnimationCutTexture("Jump",
				FrameAnimation_DESC("Player_jump.png", 0, 5, 0.1f, true));
		PlayerRenderer->CreateFrameAnimationCutTexture("Attack1",
			FrameAnimation_DESC("Player_slash_longer.png", 0, 5, 0.08f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Attack2",
			FrameAnimation_DESC("Player_slash_longer.png", 5, 10, 0.08f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("UpAttack",
			FrameAnimation_DESC("Player_slash_up.png", 0, 4, 0.08f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("DownAttack",
			FrameAnimation_DESC("Player_slash_down.png", 0, 4, 0.08f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Land",
			FrameAnimation_DESC("Player_land.png", 0, 2, 0.08f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("HardLand",
			FrameAnimation_DESC("Player_land_hard.png", 0, 4, 0.12f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Stun",
			FrameAnimation_DESC("Player_stun.png", 0, 4, 0.03f, false));
		PlayerRenderer->CreateFrameAnimationCutTexture("Charge",
			FrameAnimation_DESC("Player_Charge.png", 0, 11, 0.1f, false));
	}
	{
		SkillRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Player_slash_effect.png", 0, 0, 0.1f, false));
		SkillRenderer->CreateFrameAnimationCutTexture("Attack1",
			FrameAnimation_DESC("Player_slash_effect.png", 0, 3, 0.1f, false));
		SkillRenderer->CreateFrameAnimationCutTexture("Attack2",
			FrameAnimation_DESC("Player_slash_effect.png", 4, 7, 0.1f, false));
		SkillRenderer->CreateFrameAnimationCutTexture("UpAttack",
			FrameAnimation_DESC("Player_Upslash_effect.png", 0, 2, 0.1f, false));
		SkillRenderer->CreateFrameAnimationCutTexture("DownAttack",
				FrameAnimation_DESC("Player_Downslash_effect.png", 0, 3, 0.1f, false));
	}
	{
		StunEffect1Renderer->CreateFrameAnimationCutTexture("StunEffect",
			FrameAnimation_DESC("Hit_crack_simple.png", 0, 2, 0.03f, false));
		StunEffect2Renderer->CreateFrameAnimationCutTexture("StunEffect",
			FrameAnimation_DESC("Stun_impact_effect.png", 0, 6, 0.01f, false));
	}
	{
		ChargeEffect1->CreateFrameAnimationCutTexture("Charge1",
			FrameAnimation_DESC("Spell Effects_focus_appear0000-Sheet.png", 0, 6, 0.2f, false));
		ChargeEffect1->CreateFrameAnimationCutTexture("Charge2",
			FrameAnimation_DESC("Spell Effects_burst_effect0000-Sheet.png", 0, 7, 0.1f, false));

		ChargeEffect1->Off();
	}
	{

		HitRenderer1->CreateFrameAnimationCutTexture("HitEffect",
			FrameAnimation_DESC("EnemyHitEffects_1.png", 0, 2, 0.08f, false));
		HitRenderer1->CreateFrameAnimationCutTexture("ObjectHitEffect",
			FrameAnimation_DESC("nail_terrain_hit_effect.png", 1, 2, 0.08f, false));
		HitRenderer2->CreateFrameAnimationCutTexture("HitEffect1",
			FrameAnimation_DESC("EnemyHitEffects_2.png", 0, 4, 0.08f, false));
		HitRenderer2->CreateFrameAnimationCutTexture("HitEffect2",
			FrameAnimation_DESC("EnemyHitEffects_3.png", 0, 4, 0.08f, false));
	}
	{
		CoinEffectRenderer->CreateFrameAnimationCutTexture("Effect",
			FrameAnimation_DESC("Geo_coinget_01-Sheet.png", 0, 3, 0.08f, false));
	}

	StateManager.CreateStateMember("Idle"
		, std::bind(&Player::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::IdleStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Move"
		, std::bind(&Player::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::MoveStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("MoveToIdle"
		, std::bind(&Player::MoveToIdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::MoveToIdleStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Jump"
		, std::bind(&Player::JumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::JumpStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Attack"
		, std::bind(&Player::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::AttackStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("UpAttack"
		, std::bind(&Player::UpAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::UpAttackStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("DownAttack"
		, std::bind(&Player::DownAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::DownAttackStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Fall"
		, std::bind(&Player::FallUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::FallStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("HardLand"
		, std::bind(&Player::HardLandUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::HardLandStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Stun"
		, std::bind(&Player::StunUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::StunStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Charge"
		, std::bind(&Player::ChargeUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::ChargeStart, this, std::placeholders::_1)
	);
	StateManager.ChangeState("Idle");

	{
		StunEffect1Renderer->ChangeFrameAnimation("StunEffect");
		StunEffect1Renderer->ScaleToCutTexture(0);
		StunEffect2Renderer->ChangeFrameAnimation("StunEffect");
		StunEffect2Renderer->ScaleToCutTexture(0);
		StunEffect1Renderer->Off();
		StunEffect2Renderer->Off();
		HitRenderer1->ChangeFrameAnimation("HitEffect");
		HitRenderer1->ScaleToCutTexture(0);
		HitRenderer1->Off();
		HitRenderer2->ChangeFrameAnimation("HitEffect1");
		HitRenderer2->ScaleToCutTexture(0);
		HitRenderer2->Off();
		CoinEffectRenderer->ChangeFrameAnimation("Effect");
		CoinEffectRenderer->ScaleToCutTexture(0);
		CoinEffectRenderer->Off();
	}


	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 500.0f);
}


void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("PlayerSuper"))
	{
		if (IsSuperMode == true)
		{
			IsSuperMode = false;
		}
		else
		{
			IsSuperMode = true;
		}
	}

	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	if (ObjectShakeCamera == true)
	{
		ShakeTime = 0.0f;
		CameraShake = true;
		ObjectShakeCamera = false;
	}

	StateManager.Update(_DeltaTime);
	Gravity();
	CameraCheck();
	MonsterColCheck();
	CoinColCheck();
	
	EffectOffCheck();
}
void Player::EffectOffCheck()
{
	CoinEffectRenderer->AnimationBindEnd("Effect", [=](const FrameAnimation_DESC& _Info)
	{
		CoinEffectRenderer->Off();
	});
	HitRenderer1->AnimationBindEnd("HitEffect", [=](const FrameAnimation_DESC& _Info)
	{
		HitRenderer1->Off();
	});
	HitRenderer1->AnimationBindEnd("ObjectHitEffect", [=](const FrameAnimation_DESC& _Info)
	{
		HitRenderer1->Off();
	});
	HitRenderer2->AnimationBindEnd("HitEffect1", [=](const FrameAnimation_DESC& _Info)
	{
		HitRenderer2->Off();
	});
	HitRenderer2->AnimationBindEnd("HitEffect2", [=](const FrameAnimation_DESC& _Info)
	{
		HitRenderer2->Off();
	});
	ChargeEffect1->AnimationBindEnd("Charge2", [=](const FrameAnimation_DESC& _Info)
		{
			ChargeEffect1->Off();
		});
}
void Player::CameraCheck()
{

	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 500.0f + float4::UP*100.0f );

	float CameraRectX = 1920;
	float CameraRectY = 1080;

	float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();

	if (0 >= CurCameraPos.x- CameraRectX/2)
	{
		CurCameraPos.x = CameraRectX / 2;

		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}

	else if (MapSize.x <= CurCameraPos.x + CameraRectX / 2)
	{
		CurCameraPos.x = static_cast<float>(CurCameraPos.x - (GetTransform().GetLocalPosition().ix() + CameraRectX/2 - MapSize.x));
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}


	if (0 <= CurCameraPos.y + CameraRectY / 2)
	{
		CurCameraPos.y = -CameraRectY / 2;
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}



	else if (-MapSize.y >= CurCameraPos.y - CameraRectY / 2)
	{
		CurCameraPos.y = -static_cast<float>(CurCameraPos.y - (GetTransform().GetLocalPosition().iy() + CameraRectY/2 - MapSize.y + 100));
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
		
	}

	if(CameraShake==true)
	{
		ShakeCamera();
	}
}

void Player::ShakeCamera()
{
	if (IsTrapStun == true)
	{
		ShakeTime += 1.0f * GameEngineTime::GetDeltaTime()*10.0f;
		OneShakeCheck += 1.0f * GameEngineTime::GetDeltaTime() * 10.0f;
	}
	else
	{
		ShakeTime += 1.0f * GameEngineTime::GetDeltaTime();
		OneShakeCheck += 1.0f * GameEngineTime::GetDeltaTime();
	}
	if (ShakeTime >= 0.6f)
	{
		CameraShake = false;
	}
	if (ShakeRight == false)
	{
		
		GetLevel()->GetMainCameraActorTransform().SetWorldLeftMove(2000.0f, GameEngineTime::GetDeltaTime());
		if (OneShakeCheck > 0.03f)
		{
			ShakeRight = true;
			OneShakeCheck = 0.0f;
		}
		return;
	}
	else
	{
		GetLevel()->GetMainCameraActorTransform().SetWorldRightMove(2000.0f, GameEngineTime::GetDeltaTime());
		if (OneShakeCheck > 0.03f)
		{
			ShakeRight = false;
			OneShakeCheck = 0.0f;
		}
		return;
	}
}
void Player::Gravity()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}

	float4 Color = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy()-1);

	if (false == Color.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f))|| true == PlayerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Ground, CollisionType::CT_OBB2D,
		std::bind(&Player::GroundColCheck, this, std::placeholders::_1, std::placeholders::_2)))
	{
		if (StateManager.GetCurStateStateName() == "Fall")
		{
			if (FallTime > 1.0f)
			{
				StateManager.ChangeState("HardLand");
				return;
			}
			PlayerRenderer->ChangeFrameAnimation("Land");
			PlayerRenderer->ScaleToCutTexture(0);
			StateSound.Stop();
			StateSound = GameEngineSound::SoundPlayControl("hero_land_soft.wav");
			StateManager.ChangeState("Idle");
		}

		return;
	}
	else if(false == PlayerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Ground, CollisionType::CT_OBB2D,
		std::bind(&Player::GroundColCheck, this, std::placeholders::_1, std::placeholders::_2)))
	{
		if (StateManager.GetCurStateStateName() == "Idle")
		{
			StateManager.ChangeState("Fall");
		}
		if (StateManager.GetCurStateStateName() == "Move")
		{
			StateManager.ChangeState("Fall");
		}

		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 1000.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

void Player::MonsterColCheck()
{
	StunReadyTime -= 1.0f * GameEngineTime::GetDeltaTime();

	if (StateManager.GetCurStateStateName() != "Stun"&& StunReadyTime < 0)
	{
		PlayerCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::Monster, CollisionType::CT_SPHERE,
			std::bind(&Player::PlayerStun, this, std::placeholders::_1, std::placeholders::_2)
		);
		if (true == PlayerCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::TrapObject, CollisionType::CT_SPHERE,
			std::bind(&Player::PlayerStun, this, std::placeholders::_1, std::placeholders::_2)))
		{
			IsTrapStun = true;
		}
	}
}
void Player::CoinColCheck()
{

	PlayerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Coin, CollisionType::CT_OBB2D,
		std::bind(&Player::CoinPlus, this, std::placeholders::_1, std::placeholders::_2)
	);
}

CollisionReturn Player::GroundColCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	return::CollisionReturn::ContinueCheck;
}
CollisionReturn Player::CoinPlus(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	CoinEffectRenderer->On();
	if (CurDir == PLAYERDIR::Left)
	{

		CoinEffectRenderer->CurAnimationReset();
		CoinEffectRenderer->GetTransform().PixLocalNegativeX();
	}
	else
	{
		CoinEffectRenderer->CurAnimationReset();
		CoinEffectRenderer->GetTransform().PixLocalPositiveX();
	}
	PlayerGeoCoin += 1;
	return CollisionReturn::ContinueCheck;
}
CollisionReturn Player::HitEffectCreate(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (IsStartEffect == true)
	{
		if (CurDir == PLAYERDIR::Right)
		{
			HitRenderer1->On();
			HitRenderer1->GetTransform().SetLocalPosition({ 50,30.0f,0 });
			HitRenderer1->ChangeFrameAnimation("ObjectHitEffect");
			HitRenderer1->CurAnimationReset();
			HitRenderer1->ScaleToCutTexture(0);
			HitRenderer1->GetTransform().PixLocalPositiveX();
		}
		else
		{
			HitRenderer1->On();
			HitRenderer1->GetTransform().SetLocalPosition({ -50,30.0f,0 });
			HitRenderer1->ChangeFrameAnimation("ObjectHitEffect");
			HitRenderer1->CurAnimationReset();
			HitRenderer1->ScaleToCutTexture(0);
			HitRenderer1->GetTransform().PixLocalNegativeX();
		}
		IsStartEffect = false;
	}


	return CollisionReturn::ContinueCheck;
}
CollisionReturn Player::MonsterHit(GameEngineCollision* _This, GameEngineCollision* _Other)
{


	if (IsStartEffect == true)
	{
		PlayerEnergyGage += 0.07f;
		if (PlayerEnergyGage >= 1.0f)
		{
			PlayerEnergyGage = 1.0f;
		}
		if (AttackNum == 1)
		{
			HitRenderer1->On();
			HitRenderer1->ChangeFrameAnimation("HitEffect");
			HitRenderer1->CurAnimationReset();
			HitRenderer1->ScaleToCutTexture(0);
			HitRenderer1->GetTransform().PixLocalNegativeX();

			if (CurDir == PLAYERDIR::Right)
			{
				HitRenderer2->On();
				HitRenderer2->ChangeFrameAnimation("HitEffect1");
				HitRenderer2->CurAnimationReset();
				HitRenderer2->ScaleToCutTexture(0);
				HitRenderer2->GetTransform().PixLocalPositiveX();
			}
			else
			{
				HitRenderer2->On();
				HitRenderer2->ChangeFrameAnimation("HitEffect1");
				HitRenderer2->CurAnimationReset();
				HitRenderer2->ScaleToCutTexture(0);
				HitRenderer2->GetTransform().PixLocalNegativeX();
			}
		}
		else if (AttackNum == 2)
		{
			HitRenderer1->On();
			HitRenderer1->ChangeFrameAnimation("HitEffect");
			HitRenderer1->CurAnimationReset();
			HitRenderer1->ScaleToCutTexture(0);
			HitRenderer1->GetTransform().PixLocalPositiveX();

			if (CurDir == PLAYERDIR::Right)
			{
				HitRenderer2->On();
				HitRenderer2->ChangeFrameAnimation("HitEffect2");
				HitRenderer2->CurAnimationReset();
				HitRenderer2->ScaleToCutTexture(0);
				HitRenderer2->GetTransform().PixLocalPositiveX();

			}
			else
			{
				HitRenderer2->On();
				HitRenderer2->ChangeFrameAnimation("HitEffect2");
				HitRenderer2->CurAnimationReset();
				HitRenderer2->ScaleToCutTexture(0);
				HitRenderer2->GetTransform().PixLocalNegativeX();

			}
		}
		IsStartEffect = false;
	}

	return CollisionReturn::ContinueCheck;
}
CollisionReturn Player::DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (CurDir == PLAYERDIR::Left)
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * GameEngineTime::GetDeltaTime());
	}
	if (CurDir == PLAYERDIR::Right)
	{
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * GameEngineTime::GetDeltaTime());
	}

	return CollisionReturn::ContinueCheck;
}

bool Player::MapPixelCheck()
{
	if (true == PlayerCol->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::StopObject, CollisionType::CT_OBB2D,
		std::bind(&Player::DoorCheck, this, std::placeholders::_1, std::placeholders::_2)))
	{
		return true;
	}


	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 34,
		-GetTransform().GetWorldPosition().iy() - 40);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() - 34,
		-GetTransform().GetWorldPosition().iy() - 40);


	if (CurDir == PLAYERDIR::Left)
	{
		if (false == ColorL.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}

	if (CurDir == PLAYERDIR::Right)
	{
		if (false == ColorR.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}


	return false;

}

bool Player::MapStunPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() + 34,
		-GetTransform().GetWorldPosition().iy() - 40);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix() - 34,
		-GetTransform().GetWorldPosition().iy() - 40);


	if (CurDir == PLAYERDIR::Right)
	{
		if (false == ColorL.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}

	if (CurDir == PLAYERDIR::Left)
	{
		if (false == ColorR.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
		{
			return true;
		}
	}


	return false;

}
bool Player::MapPixelJumpCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}

	float4 ColorUp = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 60);
	if (false == ColorUp.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}


	return false;

}

CollisionReturn Player::PlayerStun(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (IsSuperMode == true)
	{
		return CollisionReturn::ContinueCheck;

	}
	else
	{
		if (PlayerHealth == 0)
		{
			//Death
			return CollisionReturn::ContinueCheck;
		}
		StateManager.ChangeState("Stun");
		StunReadyTime = 1.5f;
	}
	return CollisionReturn::ContinueCheck;
}




/////////////////////////////////////////////////////////////////////////////////
//////////////////플레이어 StateManager 함수들/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Player::IdleStart(const StateInfo& _Info)
{
	FallTime = 0.0f;
	SkillRenderer->ChangeFrameAnimation("Idle");
	SkillRenderer->ScaleToCutTexture(0);
}
void Player::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{


	if (_Info.PrevState == "Fall")
	{
		PlayerRenderer->AnimationBindEnd("Land", [=](const FrameAnimation_DESC& _Info)
		{
			PlayerRenderer->ChangeFrameAnimation("IdleHigh");
			PlayerRenderer->ScaleToCutTexture(0);
		});
	}
	else
	{
		PlayerRenderer->ChangeFrameAnimation("IdleHigh");
		PlayerRenderer->ScaleToCutTexture(0);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerCharge"))
	{
		if (PlayerHealth != PlayerFullHealth && PlayerEnergyGage >= 0.3f)
		{
			StateManager.ChangeState("Charge");
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		PlayerRenderer->ChangeFrameAnimation("Player_idle_to_run");
		PlayerRenderer->ScaleToCutTexture(0);
		StateManager.ChangeState("Move");
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump")) 
	{
		StateManager.ChangeState("Jump");
	}

	if (true == GameEngineInput::GetInst()->IsDown("PlayerAttack"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			StateManager.ChangeState("UpAttack");
		}
		else if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
		{

			StateManager.ChangeState("DownAttack");
		}
		else
		{
			StateManager.ChangeState("Attack");
		}
	}

}

void Player::MoveStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("hero_run_footsteps_stone.wav",10000);
}

void Player::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.PrevState == "Idle")
	{
		PlayerRenderer->AnimationBindEnd("Player_idle_to_run", [=](const FrameAnimation_DESC& _Info)
		{
			PlayerRenderer->ChangeFrameAnimation("PlayerWalk");
			PlayerRenderer->ScaleToCutTexture(0);
		});
	}
	else
	{
		PlayerRenderer->ChangeFrameAnimation("PlayerWalk");
		PlayerRenderer->ScaleToCutTexture(0);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump")) // 점프와 동시에 이동할 수 있어야 한다.
	{
		StateManager.ChangeState("Jump");
		StateSound.Stop();
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerAttack")) // 점프와 동시에 이동할 수 있어야 한다.
	{
		StateManager.ChangeState("Attack");
		StateSound.Stop();
	}

	if (false == GameEngineInput::GetInst()->IsPress("PlayerLeft") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{

		StateManager.ChangeState("MoveToIdle");
		StateSound.Stop();
		return;
	}
	
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
		}
		
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
		}
	
	}


}
void Player::MoveToIdleStart(const StateInfo& _Info)
{
	PlayerRenderer->ChangeFrameAnimation("Player_run_to_idle");
	PlayerRenderer->ScaleToCutTexture(0);

}
void Player::MoveToIdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	PlayerRenderer->AnimationBindEnd("Player_run_to_idle", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		PlayerRenderer->ChangeFrameAnimation("Player_idle_to_run");
		PlayerRenderer->ScaleToCutTexture(0);
		StateManager.ChangeState("Move");
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump"))
	{
		StateManager.ChangeState("Jump");
	}

	if (true == GameEngineInput::GetInst()->IsDown("PlayerAttack"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			StateManager.ChangeState("UpAttack");
		}
		else if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
		{

			StateManager.ChangeState("DownAttack");
		}
		else
		{
			StateManager.ChangeState("Attack");
		}
	}
}

void Player::FallStart(const StateInfo& _Info)
{
	if (IsTrapStun == true)
	{
		GetTransform().SetLocalPosition(TrapStunPos);
		IsTrapStun = false;
	}

	PlayerRenderer->ChangeFrameAnimation("Fall");
	PlayerRenderer->ScaleToCutTexture(0);
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
	}
	FallTime = 0.0f;
}
void Player::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == GameEngineInput::GetInst()->IsDown("PlayerAttack"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			StateManager.ChangeState("UpAttack");
		}
		else if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
		{

			StateManager.ChangeState("DownAttack");
		}
		else
		{
			StateManager.ChangeState("Attack");
		}
	}

	FallTime += 1.0f * _DeltaTime;

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
		}

	}

	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
	}
}	
void Player::HardLandStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("hero_land_hard.wav");
	ShakeTime = 0.0f;
	CameraShake = true;
	PlayerRenderer->ChangeFrameAnimation("HardLand");
	PlayerRenderer->ScaleToCutTexture(0);
}
void Player::HardLandUpdate(float _DeltaTime, const StateInfo& _Info)
{

	PlayerRenderer->AnimationBindEnd("HardLand", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
	}

}
void Player::JumpStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("hero_jump.wav");
	JumpTime = 0.0f;
	PlayerRenderer->ChangeFrameAnimation("Jump");
	PlayerRenderer->ScaleToCutTexture(0);
}

void Player::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == GameEngineInput::GetInst()->IsDown("PlayerAttack"))
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
		{
			StateManager.ChangeState("UpAttack");
		}
		else if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
		{

			StateManager.ChangeState("DownAttack");
		}
		else
		{
			StateManager.ChangeState("Attack");
		}
	}

	if (false == GameEngineInput::GetInst()->IsPress("PlayerJump"))
	{
		StateManager.ChangeState("Idle");
		return;
	}

	JumpTime += 1.0f * _DeltaTime;

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump"))
	{
		if (JumpTime > 0.5f)
		{
			StateManager.ChangeState("Idle");
		}

		if (CurDir == PLAYERDIR::Left)
		{
			PlayerRenderer->GetTransform().PixLocalPositiveX();
		}		
		if (CurDir == PLAYERDIR::Right)
		{
			PlayerRenderer->GetTransform().PixLocalNegativeX();
		}

		if (false == MapPixelJumpCheck())
		{
			GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
GetTransform().GetWorldPosition().y + 1800.0f * GameEngineTime::GetDeltaTime(),
GetTransform().GetWorldPosition().z, });
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
		}

	}


}

void Player::AttackStart(const StateInfo& _Info)
{

	if (CurDir == PLAYERDIR::Left)
	{
		LeftSkilCol->On();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		RightSkilCol->On();
	}
	if (AttackNum == 1)
	{
		AttackSound.Stop();
		AttackSound = GameEngineSound::SoundPlayControl("sword_1.wav");
		PlayerRenderer->ChangeFrameAnimation("Attack1");
		PlayerRenderer->ScaleToCutTexture(0);
		SkillRenderer->ChangeFrameAnimation("Attack1");
		SkillRenderer->ScaleToCutTexture(0);
		AttackNum = 2;
	}
	else if (AttackNum == 2)
	{
		AttackSound.Stop();
		AttackSound = GameEngineSound::SoundPlayControl("sword_3.wav");
		PlayerRenderer->ChangeFrameAnimation("Attack2");
		PlayerRenderer->ScaleToCutTexture(0);
		SkillRenderer->ChangeFrameAnimation("Attack2");
		SkillRenderer->ScaleToCutTexture(0);
		AttackNum = 1;
	}

	IsStartEffect = true;
} 
void Player::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (true == RightSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::Monster, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		AttackSound.Stop();
	}
	else if (true == LeftSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::Monster, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		AttackSound.Stop();
	}

	if (true == RightSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::FrontObject, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		ShakeTimeReset();
		AttackSound.Stop();
	}
	else if (true == LeftSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::FrontObject, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		ShakeTimeReset();
		AttackSound.Stop();
	}

	if (true == RightSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::StopObject, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		AttackSound.Stop();
	}
	else if (true == LeftSkilCol->IsCollision(CollisionType::CT_SPHERE, OBJECTORDER::StopObject, CollisionType::CT_SPHERE,
		std::bind(&Player::MonsterHit, this, std::placeholders::_1, std::placeholders::_2)))
	{
		AttackSound.Stop();
	}



	PlayerRenderer->AnimationBindEnd("Attack1", [=](const FrameAnimation_DESC& _Info)
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft")
			|| true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
		{

			LeftSkilCol->Off();
			RightSkilCol->Off();
			StateManager.ChangeState("Move");
		}
		else
		{

			LeftSkilCol->Off();
			RightSkilCol->Off();
			StateManager.ChangeState("Idle");
		}
	});

	PlayerRenderer->AnimationBindEnd("Attack2", [=](const FrameAnimation_DESC& _Info)
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft")
			|| true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
		{

			LeftSkilCol->Off();
			RightSkilCol->Off();
			StateManager.ChangeState("Move");
		}
		else
		{
			LeftSkilCol->Off();
			RightSkilCol->Off();
			StateManager.ChangeState("Idle");
		}
	});

	if (CurDir == PLAYERDIR::Left)
	{
		SkillRenderer->GetTransform().PixLocalPositiveX();
		PlayerRenderer->GetTransform().PixLocalPositiveX();

	}
	if (CurDir == PLAYERDIR::Right)
	{
		SkillRenderer->GetTransform().PixLocalNegativeX();
		PlayerRenderer->GetTransform().PixLocalNegativeX();

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
		}
	}

}
void Player::UpAttackStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("sword_2.wav");
	PlayerRenderer->ChangeFrameAnimation("UpAttack");
	PlayerRenderer->ScaleToCutTexture(0);	
	SkillRenderer->ChangeFrameAnimation("UpAttack");
	SkillRenderer->ScaleToCutTexture(0);
}
void Player::UpAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
		SkillRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
		SkillRenderer->GetTransform().PixLocalNegativeX();
	}
	PlayerRenderer->AnimationBindEnd("UpAttack", [=](const FrameAnimation_DESC& _Info)
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft")
			|| true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
		{
			StateManager.ChangeState("Move");
		}
		else
		{
			StateManager.ChangeState("Idle");
		}
	});

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
			SkillRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
			SkillRenderer->GetTransform().PixLocalNegativeX();
		}
	}
}

void Player::DownAttackStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("sword_3.wav");
	PlayerRenderer->ChangeFrameAnimation("DownAttack");
	PlayerRenderer->ScaleToCutTexture(0);
	SkillRenderer->ChangeFrameAnimation("DownAttack");
	SkillRenderer->ScaleToCutTexture(0);
}
void Player::DownAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	PlayerRenderer->AnimationBindEnd("DownAttack", [=](const FrameAnimation_DESC& _Info)
	{
		if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft")
			|| true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
		{
			StateManager.ChangeState("Move");
		}
		else
		{
			StateManager.ChangeState("Idle");
		}
	});

	if (CurDir == PLAYERDIR::Left)
	{

		PlayerRenderer->GetTransform().PixLocalPositiveX();
		SkillRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
		SkillRenderer->GetTransform().PixLocalNegativeX();
	}


	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
			SkillRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
			SkillRenderer->GetTransform().PixLocalNegativeX();
		}
	}
}

void Player::StunStart(const StateInfo& _Info)
{
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("hero_damage.wav");
	ShakeTimeReset();
	PlayerRenderer->ChangeFrameAnimation("Stun");
	PlayerRenderer->ScaleToCutTexture(0);
	PlayerHealth -= 1;
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
	}
	FallTime = 0.05f;
	GlobalTimeScale = 0.1f;
	{
		StunEffect1Renderer->On();
		StunEffect1Renderer->CurAnimationReset();
		StunEffect1Renderer->ScaleToCutTexture(0);
	}
}
void Player::StunUpdate(float _DeltaTime, const StateInfo& _Info)
{
	GameEngineTime::GetInst()->SetGlobalScale(GlobalTimeScale);

	StunEffect1Renderer->AnimationBindEnd("StunEffect", [=](const FrameAnimation_DESC& _Info)
	{
		StunEffect1Renderer->Off();
	});

	PlayerRenderer->AnimationBindEnd("Stun", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Fall");
		GameEngineTime::GetInst()->SetGlobalScale(1.0f);

	});

	if (FallTime > 0.0f)
	{
		FallTime -= 1.0f * _DeltaTime;
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y + 500.0f * _DeltaTime,
	GetTransform().GetWorldPosition().z, });
	}
	else
	{
		GlobalTimeScale += 0.5f * _DeltaTime * 10;
		if (GlobalTimeScale >= 1.0f)
		{
			GlobalTimeScale = 1.0f;
		}
		GameEngineTime::GetInst()->SetGlobalScale(1.0f);
	}
	if (MapStunPixelCheck() == false)
	{
		if (CurDir == PLAYERDIR::Left)
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * 1800.0f * _DeltaTime);
		}
		if (CurDir == PLAYERDIR::Right)
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * 1800.0f * _DeltaTime);
		}
	}

}


void Player::ChargeStart(const StateInfo& _Info)
{

	PlayerRenderer->ChangeFrameAnimation("Charge");
	PlayerRenderer->ScaleToCutTexture(0);
	ChargeTime = 0.0f;
	ChargeEffect1->On();
	ChargeEffect1->ChangeFrameAnimation("Charge1");
	ChargeEffect1->ScaleToCutTexture(0);
	StateSound.Stop();
	StateSound = GameEngineSound::SoundPlayControl("hero_nail_art_charge_initiate.wav");
}

void Player::ChargeUpdate(float _DeltaTime, const StateInfo& _Info)
{
	ChargeEffect1->AnimationBindEnd("Charge1", [=](const FrameAnimation_DESC& _Info)
	{
		StateSound.Stop();
		StateSound = GameEngineSound::SoundPlayControl("hero_nail_art_charge_complete.wav");
		ChargeEffect1->ChangeFrameAnimation("Charge2");
		ChargeEffect1->ScaleToCutTexture(0);
	});
	ChargeTime += 1.0f * _DeltaTime;
	PlayerEnergyGage -= 0.15f * _DeltaTime;
	if (false == GameEngineInput::GetInst()->IsPress("PlayerCharge"))
	{
		StateManager.ChangeState("Idle");
	}
	if (ChargeTime > 1.2f)
	{
		if (PlayerHealth != PlayerFullHealth)
		{
			PlayerHealth += 1;
		}
		ChargeTime = 0.0f;
	}
	PlayerRenderer->AnimationBindEnd("Charge", [=](const FrameAnimation_DESC& _Info)
	{
		StateManager.ChangeState("Idle");
	});

}

////////////////////////////////////////////////////////////////////////////////////////////////////


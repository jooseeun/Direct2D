#include "PreCompile.h"
#include "Player.h"
#include "PlayLevelManager.h"
#include <iostream>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineDefaultRenderer.h>
#include <GameEngineCore/GameEngineRenderingPipeLine.h>
#include <GameEngineCore/GameEngineVertexShader.h>
#include <GameEngineCore/GameEngineConstantBuffer.h>
#include <GameEngineCore/GameEngineDevice.h>
#include <GameEngineCore/GameEngineTextureRenderer.h>


Player* Player::MainPlayer = nullptr;

Player::Player()
	: Speed(1000.0f)
	, PlayerRenderer(nullptr)
	, CurDir(PLAYERDIR::Right)
	, AttackNum(1)
	, Color(0)
	, MapSize(0)
	, StateManager()
	, JumpTime(0)
	, SkilRenderer(nullptr)
	, FallTime(0)
	, LeftSkilCol(nullptr)
	, RightSkilCol(nullptr)
	, PlayerCol(nullptr)
{
	MainPlayer = this;
}

Player::~Player()
{
}


void Player::Start()
{
	if (false == GameEngineInput::GetInst()->IsKey("PlayerLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("PlayerLeft", VK_LEFT);
		GameEngineInput::GetInst()->CreateKey("PlayerRight", VK_RIGHT);
		GameEngineInput::GetInst()->CreateKey("PlayerUp", VK_UP);
		GameEngineInput::GetInst()->CreateKey("PlayerDown", VK_DOWN);
		GameEngineInput::GetInst()->CreateKey("PlayerJump", 'Z');
		GameEngineInput::GetInst()->CreateKey("PlayerAttack", 'X');
	}

	GetTransform().SetLocalScale({ 1, 1, 1 });

	
	{
		PlayerRenderer = CreateComponent<GameEngineTextureRenderer>();
		PlayerRenderer->GetTransform().SetLocalScale({ 350, 186, 10.0f });
		PlayerRenderer->SetTexture("PlayerIdle.png");
		PlayerRenderer->SetOrder((int)OBJECTORDER::Player);
		PlayerRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		SkilRenderer = CreateComponent<GameEngineTextureRenderer>();
		SkilRenderer->GetTransform().SetLocalScale({ 350, 186, 10.0f });
		SkilRenderer->SetTexture("PlayerIdle.png");
		SkilRenderer->SetOrder((int)OBJECTORDER::Player);
		SkilRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		PlayerCol = CreateComponent<GameEngineCollision>();
		PlayerCol->GetTransform().SetLocalScale({ 66.0f,125.0f,100.0f });
		PlayerCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 0,62.5f,0 });
	}
	{
		LeftSkilCol = CreateComponent<GameEngineCollision>();
		LeftSkilCol->GetTransform().SetLocalScale({ 175.0f , 92.f, 100.0f });
		LeftSkilCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ -50,62.5f,0 });
		LeftSkilCol->Off();
	}
	{
		RightSkilCol = CreateComponent<GameEngineCollision>();
		RightSkilCol->GetTransform().SetLocalScale({ 175.0f , 92.f, 100.0f });
		RightSkilCol->GetTransform().SetLocalPosition(GetTransform().GetLocalPosition() +
			float4{ 50,62.5f,0 });
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
			FrameAnimation_DESC("Player_land_hard.png", 0, 4, 0.08f, false));
	}
	{
		SkilRenderer->CreateFrameAnimationCutTexture("Idle",
			FrameAnimation_DESC("Player_slash_effect.png", 0, 0, 0.1f, false));
		SkilRenderer->CreateFrameAnimationCutTexture("Attack1",
			FrameAnimation_DESC("Player_slash_effect.png", 0, 3, 0.1f, false));
		SkilRenderer->CreateFrameAnimationCutTexture("Attack2",
			FrameAnimation_DESC("Player_slash_effect.png", 4, 7, 0.1f, false));
		SkilRenderer->CreateFrameAnimationCutTexture("UpAttack",
			FrameAnimation_DESC("Player_Upslash_effect.png", 0, 2, 0.1f, false));
		SkilRenderer->CreateFrameAnimationCutTexture("DownAttack",
				FrameAnimation_DESC("Player_Downslash_effect.png", 0, 3, 0.1f, false));
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
	StateManager.ChangeState("Fall");

	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 500.0f);
}


void Player::Update(float _DeltaTime)
{


	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	StateManager.Update(_DeltaTime);
	Gravity();
	CameraCheck();
}

void Player::CameraCheck()
{

	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 500.0f);

	float CameraRectX = 1920;
	float CameraRectY = 1080;

	float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();

	if (0 >= CurCameraPos.x- CameraRectX/2)
	{
		float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();
		CurCameraPos.x = CameraRectX / 2;

		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}

	else if (MapSize.x <= CurCameraPos.x + CameraRectX / 2)
	{
		float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();
		CurCameraPos.x = static_cast<int>(GetLevel()->GetMainCameraActorTransform().GetLocalPosition().ix() - (GetTransform().GetLocalPosition().ix() + CameraRectX/2 - MapSize.x));
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}


	if (0 <= CurCameraPos.y + CameraRectY / 2)
	{
		float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();
		CurCameraPos.y = -CameraRectY / 2;
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
	}



	else if (-MapSize.y >= CurCameraPos.y - CameraRectY / 2)
	{
		float4 CurCameraPos = GetLevel()->GetMainCameraActorTransform().GetLocalPosition();
		CurCameraPos.y = -static_cast<int>(GetLevel()->GetMainCameraActorTransform().GetLocalPosition().iy() - (GetTransform().GetLocalPosition().iy() + CameraRectY/2 - MapSize.y));
		GetLevel()->GetMainCameraActorTransform().SetLocalPosition(CurCameraPos);
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
		-GetTransform().GetWorldPosition().iy() - 1);

	if (false == Color.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		if (StateManager.GetCurStateStateName() == "Fall")
		{
			if (FallTime > 1.5f)
			{
				StateManager.ChangeState("HardLand");
				return;
			}
			PlayerRenderer->ChangeFrameAnimation("Land");
			PlayerRenderer->ScaleToCutTexture(0);
			StateManager.ChangeState("Idle");
		}

		return;
	}
	else
	{
		if (StateManager.GetCurStateStateName() == "Idle")
		{
			StateManager.ChangeState("Fall");
		}
		GetTransform().SetLocalPosition({ GetTransform().GetWorldPosition().x,
	GetTransform().GetWorldPosition().y - 800.0f * GameEngineTime::GetDeltaTime(),
	GetTransform().GetWorldPosition().z, });
	}

}

bool Player::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}


	float4 ColorR = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix()+34,
		-GetTransform().GetWorldPosition().iy()-5);
	float4 ColorL = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix()-34,
		-GetTransform().GetWorldPosition().iy()-5);
	float4 ColorUp = ColMapTexture->GetPixelToFloat4(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 125);
	if (false == ColorUp.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}

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

void Player::IdleStart(const StateInfo& _Info)
{
	FallTime = 0.0f;
	SkilRenderer->ChangeFrameAnimation("Idle");
	SkilRenderer->ScaleToCutTexture(0);
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
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerAttack")) // 점프와 동시에 이동할 수 있어야 한다.
	{
		StateManager.ChangeState("Attack");
	}

	if (false == GameEngineInput::GetInst()->IsPress("PlayerLeft") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{

		StateManager.ChangeState("MoveToIdle");
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
	JumpTime = 0.0f;
	PlayerRenderer->ChangeFrameAnimation("Jump");
	PlayerRenderer->ScaleToCutTexture(0);
}

void Player::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
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

		if (false == MapPixelCheck())
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
		PlayerRenderer->ChangeFrameAnimation("Attack1");
		PlayerRenderer->ScaleToCutTexture(0);
		SkilRenderer->ChangeFrameAnimation("Attack1");
		SkilRenderer->ScaleToCutTexture(0);
		AttackNum = 2;
	}
	else if (AttackNum == 2)
	{
		PlayerRenderer->ChangeFrameAnimation("Attack2");
		PlayerRenderer->ScaleToCutTexture(0);
		SkilRenderer->ChangeFrameAnimation("Attack2");
		SkilRenderer->ScaleToCutTexture(0);
		AttackNum = 1;
	}
} 
void Player::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
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
		SkilRenderer->GetTransform().PixLocalPositiveX();
		PlayerRenderer->GetTransform().PixLocalPositiveX();

	}
	if (CurDir == PLAYERDIR::Right)
	{
		SkilRenderer->GetTransform().PixLocalNegativeX();
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
	PlayerRenderer->ChangeFrameAnimation("UpAttack");
	PlayerRenderer->ScaleToCutTexture(0);	
	SkilRenderer->ChangeFrameAnimation("UpAttack");
	SkilRenderer->ScaleToCutTexture(0);
}
void Player::UpAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (CurDir == PLAYERDIR::Left)
	{
		PlayerRenderer->GetTransform().PixLocalPositiveX();
		SkilRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
		SkilRenderer->GetTransform().PixLocalNegativeX();
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
			SkilRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
			SkilRenderer->GetTransform().PixLocalNegativeX();
		}
	}
}

void Player::DownAttackStart(const StateInfo& _Info)
{
	PlayerRenderer->ChangeFrameAnimation("DownAttack");
	PlayerRenderer->ScaleToCutTexture(0);
	SkilRenderer->ChangeFrameAnimation("DownAttack");
	SkilRenderer->ScaleToCutTexture(0);
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
		SkilRenderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		PlayerRenderer->GetTransform().PixLocalNegativeX();
		SkilRenderer->GetTransform().PixLocalNegativeX();
	}


	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalPositiveX();
			SkilRenderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			PlayerRenderer->GetTransform().PixLocalNegativeX();
			SkilRenderer->GetTransform().PixLocalNegativeX();
		}
	}
}


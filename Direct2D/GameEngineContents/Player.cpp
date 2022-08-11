#include "PreCompile.h"
#include "Player.h"
#include "PlayLevelManager.h"
#include <GameEngineBase/GameEngineInput.h>
#include <iostream>
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
	, Renderer(nullptr)
	, CurDir(PLAYERDIR::Right)
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
		GameEngineInput::GetInst()->CreateKey("PlayerJump", 'Z');
		GameEngineInput::GetInst()->CreateKey("PlayerAttack", 'X');
	}

	GetTransform().SetLocalScale({ 1., 1, 1 });

	
	{
		Renderer = CreateComponent<GameEngineTextureRenderer>();
		Renderer->GetTransform().SetLocalScale({ 350, 186, 10.0f });
		Renderer->SetTexture("PlayerIdle.png");
		Renderer->SetOrder((int)OBJECTORDER::Player);
		Renderer->SetPivot(PIVOTMODE::BOT);
	}

	{
		Renderer->CreateFrameAnimationCutTexture("IdleHigh", 
			FrameAnimation_DESC("PlayerIdleHighHealth.png", 0, 8,0.1f, true));
		Renderer->CreateFrameAnimationCutTexture("PlayerWalk",
			FrameAnimation_DESC("Player_run.png", 0, 7, 0.1f, true));
		Renderer->CreateFrameAnimationCutTexture("Player_idle_to_run",
			FrameAnimation_DESC("Player_idle_to_run.png", 0, 4, 0.1f, false));
		Renderer->CreateFrameAnimationCutTexture("Player_run_to_idle",
			FrameAnimation_DESC("Player_run_to_idle.png", 0, 5, 0.1f, false));
		Renderer->CreateFrameAnimationCutTexture("Roar",
			FrameAnimation_DESC("Player_roar.png", 0, 7, 0.1f, false));
		Renderer->CreateFrameAnimationCutTexture("Fall",
			FrameAnimation_DESC("Player_fall.png", 3, 5, 0.1f, true));		
		Renderer->CreateFrameAnimationCutTexture("Jump",
				FrameAnimation_DESC("Player_jump.png", 0, 5, 0.1f, true));
	}


	StateManager.CreateStateMember("Idle"
		, std::bind(&Player::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::IdleStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Move"
		, std::bind(&Player::MoveUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::MoveStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Jump"
		, std::bind(&Player::JumpUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::JumpStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Attack"
		, std::bind(&Player::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::AttackStart, this, std::placeholders::_1)
	);
	StateManager.CreateStateMember("Fall"
		, std::bind(&Player::FallUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Player::FallStart, this, std::placeholders::_1)
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
	float4 Color = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 1);

	if (false == Color.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		if (StateManager.GetCurStateStateName() == "Fall")
		{
			Renderer->ChangeFrameAnimation("Roar");
			Renderer->ScaleToCutTexture(0);
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


	float4 ColorR = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix()+34,
		-GetTransform().GetWorldPosition().iy()-5);
	float4 ColorL = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix()-34,
		-GetTransform().GetWorldPosition().iy()-5);
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

}
void Player::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

	Renderer->AnimationBindEnd("IdleHigh", std::bind(&Player::Renderer,this));
	Renderer->ScaleToCutTexture(0);

	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		Renderer->ChangeFrameAnimation("Player_idle_to_run");
		Renderer->ScaleToCutTexture(0);
		StateManager.ChangeState("Move");
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump")) 
	{
		StateManager.ChangeState("Jump");
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerAttack"))
	{
		StateManager.ChangeState("Attack");
	}
}

void Player::MoveStart(const StateInfo& _Info)
{

}

void Player::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.5f)
	{
		Renderer->ChangeFrameAnimation("PlayerWalk");
		Renderer->ScaleToCutTexture(0);
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump")) // 점프와 동시에 이동할 수 있어야 한다.
	{
		StateManager.ChangeState("Jump");
	}


	if (false == GameEngineInput::GetInst()->IsPress("PlayerLeft") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		Renderer->ChangeFrameAnimation("Player_run_to_idle");
		Renderer->ScaleToCutTexture(0);
		StateManager.ChangeState("Idle");
		return;
	}
	
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalPositiveX();
		}
		
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalNegativeX();
		}
	
	}


}


void Player::FallStart(const StateInfo& _Info)
{
	Renderer->ChangeFrameAnimation("Fall");
	Renderer->ScaleToCutTexture(0);
	Renderer->GetTransform().PixLocalNegativeX();
}
void Player::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{



	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		CurDir = PLAYERDIR::Left;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalNegativeX();
		}

	}

	if (CurDir == PLAYERDIR::Left)
	{
		Renderer->GetTransform().PixLocalPositiveX();
	}
	if (CurDir == PLAYERDIR::Right)
	{
		Renderer->GetTransform().PixLocalNegativeX();
	}
}
void Player::JumpStart(const StateInfo& _Info)
{
	JumpTime = 0.0f;
	Renderer->ChangeFrameAnimation("Jump");
	Renderer->ScaleToCutTexture(0);
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
			Renderer->GetTransform().PixLocalPositiveX();
		}		
		if (CurDir == PLAYERDIR::Right)
		{
			Renderer->GetTransform().PixLocalNegativeX();
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
			Renderer->GetTransform().PixLocalPositiveX();
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		CurDir = PLAYERDIR::Right;
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalNegativeX();
		}

	}


}

void Player::AttackStart(const StateInfo& _Info)
{

}
void Player::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}



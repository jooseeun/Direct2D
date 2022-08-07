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
		Renderer = CreateComponent<GameEngineTextureRenderer>();
		Renderer->GetTransform().SetLocalScale({ 100, 100, 10.0f });
		Renderer->SetTexture("PlayerIdle.png");
		Renderer->SetOrder((int)OBJECTORDER::Player);

		GameEngineDirectory Dir;
		Dir.MoveParentToExitsChildDirectory("ContentsResources");
		Dir.Move("ContentsResources");
		Dir.Move("Texture");
		Dir.Move("Player");

		GameEngineFolderTexture::Load(Dir.GetFullPath());

		Renderer->ScaleToTexture();
		Renderer->SetPivot(PIVOTMODE::BOT);
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
	StateManager.ChangeState("Idle");

	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 500.0f);
}


void Player::Update(float _DeltaTime)
{


	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	StateManager.Update(_DeltaTime);
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

bool Player::MapPixelCheck()
{
	GameEngineTexture* ColMapTexture = GetLevel<PlayLevelManager>()->GetColMap()->GetCurTexture();
	if (nullptr == ColMapTexture)
	{
		MsgBoxAssert("충돌용 맵이 세팅되지 않았습니다");
	}
	float4 ColorR = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix()+32,
		-GetTransform().GetWorldPosition().iy());
	float4 ColorL = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix()-32,
		-GetTransform().GetWorldPosition().iy());
	float4 ColorU = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() + 62);
	float4 ColorD = ColMapTexture->GetPixel(GetTransform().GetWorldPosition().ix(),
		-GetTransform().GetWorldPosition().iy() - 62);

	if (false == ColorR.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)) ||
		false == ColorL.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)) ||
		false == ColorU.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)) ||
		false == ColorD.CompareInt4D(float4(1.0f, 1.0f, 1.0f, 0.0f)))
	{
		return true;
	}
	return false;

}

void Player::IdleStart(const StateInfo& _Info)
{

}
void Player::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerRight") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerUp") ||
		true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		StateManager.ChangeState("Move");
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerJump")) // 점프와 동시에 이동할 수 있어야 한다.
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


	if (false == GameEngineInput::GetInst()->IsPress("PlayerLeft") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerRight") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerUp") &&
		false == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		StateManager.ChangeState("Idle");
		return;
	}
	
	if (true == GameEngineInput::GetInst()->IsPress("PlayerLeft"))
	{
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalNegativeX();
		}
		
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		if (false == MapPixelCheck())
		{
			GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
			Renderer->GetTransform().PixLocalPositiveX();
		}
	
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
	{
		if (false == MapPixelCheck())
		{

			GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		if (false == MapPixelCheck())
		{

			GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed * _DeltaTime);
		}
	}

}
void Player::JumpStart(const StateInfo& _Info)
{

}

void Player::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Player::AttackStart(const StateInfo& _Info)
{

}
void Player::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{

}



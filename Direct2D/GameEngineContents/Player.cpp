#include "PreCompile.h"
#include "Player.h"
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
		GameEngineInput::GetInst()->CreateKey("PlayerLeft", 'A');
		GameEngineInput::GetInst()->CreateKey("PlayerRight", 'D');
		GameEngineInput::GetInst()->CreateKey("PlayerUp", 'W');
		GameEngineInput::GetInst()->CreateKey("PlayerDown", 'S');
	}

	GetTransform().SetLocalScale({ 1, 1, 1 });

	
	{
		Renderer = CreateComponent<GameEngineTextureRenderer>();
		Renderer->GetTransform().SetLocalScale({ 100, 100, 1 });
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


	StateManager.CreateStateMember("Idle", this, &Player::IdleUpdate, &Player::IdleStart);
	StateManager.CreateStateMember("Move", this, &Player::MoveUpdate, &Player::MoveStart);
	StateManager.ChangeState("Idle");

	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 100.0f);
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
		GetTransform().SetWorldMove(GetTransform().GetLeftVector() * Speed * _DeltaTime);
		Renderer->GetTransform().PixLocalNegativeX();
	}

	if (true == GameEngineInput::GetInst()->IsPress("PlayerRight"))
	{
		GetTransform().SetWorldMove(GetTransform().GetRightVector() * Speed * _DeltaTime);
		Renderer->GetTransform().PixLocalPositiveX();
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerUp"))
	{
		GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::GetInst()->IsPress("PlayerDown"))
	{
		GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed * _DeltaTime);
	}
	GetLevel()->GetMainCameraActorTransform().SetLocalPosition(GetTransform().GetLocalPosition() + float4::BACK * 100.0f);
}

void Player::Update(float _DeltaTime)
{


	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	StateManager.Update(_DeltaTime);

}
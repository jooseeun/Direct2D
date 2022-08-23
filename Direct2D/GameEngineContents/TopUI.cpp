#include "TopUI.h"
#include "PreCompile.h"
#include "Player.h"

TopUI* TopUI::MainTopUI = nullptr;

TopUI::TopUI() 
	:EnergyUI(nullptr)
	, Health()
	, EmpthyHealth()
	, CurHealth(5)
{
}

TopUI::~TopUI() 
{
}

void TopUI::Start()
{
	{
		EnergyUI = CreateComponent<GameEngineUIRenderer>();
		EnergyUI->SetTexture("EnergyUI.png");
		EnergyUI->GetTransform().SetLocalScale({ 257, 164, 1 });
		EnergyUI->SetFrame(5);
		EnergyUI->GetTransform().SetLocalPosition({ -650, 350.0f, 1 });
		EnergyUI->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		for (int i = 0; i < 5 ; i++)
		{
			Health[i] = CreateComponent<GameEngineUIRenderer>();
			Health[i]->CreateFrameAnimationCutTexture("FullHealth",
				FrameAnimation_DESC("FullHealthUI.png", 0, 0, 0.1f, false));
			Health[i]->ChangeFrameAnimation("FullHealth");
			Health[i]->ScaleToCutTexture(0);
			Health[i]->GetTransform().SetLocalPosition({ -590.0f + i * 74.0f, 350.0f, 1 });
			Health[i]->ChangeCamera(CAMERAORDER::UICAMERA);
		}
	}
	{
		for (int i = 0; i < 5; i++)
		{
			EmpthyHealth[i] = CreateComponent<GameEngineUIRenderer>();
			EmpthyHealth[i]->SetTexture("EmptyHealthUI.png");
			EmpthyHealth[i]->ScaleToTexture();
			EmpthyHealth[i]->GetTransform().SetLocalPosition({ -590.0f + i * 74.0f, 350.0f, 1 });
			EmpthyHealth[i]->ChangeCamera(CAMERAORDER::UICAMERA);
			EmpthyHealth[i]->Off();
		}
	}
}

void TopUI::Update(float _DeltaTime)
{
	HealthUpdate();
}
void TopUI::HealthUpdate()
{
	if (CurHealth != Player::GetMainPlayer()->PlayerHealth)
	{
		for (int i = 0; i < Player::GetMainPlayer()->PlayerHealth; i++)
		{
			Health[i]->On();
			EmpthyHealth[i]->Off();
		}
		for (int i = Player::GetMainPlayer()->PlayerHealth; i < Player::GetMainPlayer()->PlayerFullHealth; i++)
		{
			Health[i]->Off();
			EmpthyHealth[i]->On();
		}

		CurHealth = Player::GetMainPlayer()->PlayerHealth;
	}


}

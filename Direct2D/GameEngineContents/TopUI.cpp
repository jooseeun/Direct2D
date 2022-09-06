#include "TopUI.h"
#include "PreCompile.h"
#include "Player.h"
#include "GameEngineMyRenderer.h"
TopUI* TopUI::MainTopUI = nullptr;

TopUI::TopUI() 
	:EnergyUI(nullptr)
	, Health()
	, EmpthyHealth()
	, CurHealth(5)
	, MoneyUI(nullptr)
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
		MoneyUI = CreateComponent<GameEngineUIRenderer>();
		MoneyUI->SetTexture("TopUICoin.png");
		MoneyUI->ScaleToCutTexture(0);
		MoneyUI->SetFrame(1);
		MoneyUI->GetTransform().SetLocalPosition({ -590, 300.0f, 1 });
		MoneyUI->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		for (int i = 0; i < 5 ; i++)
		{
			Health[i] = CreateComponent<GameEngineUIRenderer>();
			Health[i]->CreateFrameAnimationCutTexture("FullHealth",
				FrameAnimation_DESC("FullHealthUI.png", 0, 0, 0.1f, false));
			Health[i]->CreateFrameAnimationCutTexture("BreakHealth",
				FrameAnimation_DESC("BreakHealthUI.png", 0, 5, 0.1f, false));
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
	{
		Energy = CreateComponent<GameEngineMyRenderer>();
		Energy->SetMaskingTexture("HUD_Soulorb_fills_soul_idle0000-Sheet.png","HUD Cln_soul_orb_shape.png",0);
		Energy->GetTransform().SetLocalScale({ 257, 164, 1 });
		Energy->GetTransform().SetLocalPosition({ -650, 350.0f, 1 });
		Energy->ChangeCamera(CAMERAORDER::UICAMERA);
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
			Health[i]->ChangeFrameAnimation("BreakHealth");
			Health[i]->ScaleToCutTexture(0);
		}

		CurHealth = Player::GetMainPlayer()->PlayerHealth;
	}

	for (int i = Player::GetMainPlayer()->PlayerHealth; i < Player::GetMainPlayer()->PlayerFullHealth; i++)
	{
		Health[i]->AnimationBindEnd("BreakHealth", [=](const FrameAnimation_DESC& _Info)
		{
			Health[i]->Off();
			EmpthyHealth[i]->On();
		});
	}


}

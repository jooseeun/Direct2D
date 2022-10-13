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
	, AniTime(4.0f)
	, GeoCoinFont(nullptr)
	, CurGeoCoin("0")
	, CurCoin(0)
	, EnergyUIEffect(nullptr)
{
	MainTopUI = this;
}

TopUI::~TopUI() 
{
}

void TopUI::Start()
{

	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		EnergyUI = CreateComponent<GameEngineUIRenderer>();
		EnergyUI->SetTexture("EnergyUI.png");
		EnergyUI->GetTransform().SetLocalScale({ 257, 164, 1 });
		EnergyUI->SetFrame(5);
		EnergyUI->GetTransform().SetLocalPosition({ -780, 420.0f, 1 });
		EnergyUI->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		MoneyUI = CreateComponent<GameEngineUIRenderer>();
		MoneyUI->SetTexture("TopUICoin.png");
		MoneyUI->ScaleToCutTexture(0);
		MoneyUI->SetFrame(1);
		MoneyUI->GetTransform().SetLocalPosition({ -720, 360.0f, 1 });
		MoneyUI->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{

		GeoCoinFont = CreateComponent<GameEngineFontRenderer>();
		GeoCoinFont->SetText(CurGeoCoin, "Noto Serif KR Black");
		GeoCoinFont->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		GeoCoinFont->SetSize(40.0f);
		GeoCoinFont->SetScreenPostion({ 220, 115, 1 });
		GeoCoinFont->ChangeCamera(CAMERAORDER::UICAMERA);
	}
	{
		for (int i = 0; i < 5 ; i++)
		{
			Health[i] = CreateComponent<GameEngineUIRenderer>();
			Health[i]->CreateFrameAnimationCutTexture("FullHealth",
				FrameAnimation_DESC("FullHealthUI.png", 0, 5, 0.1f, false));
			Health[i]->CreateFrameAnimationCutTexture("IdleHealth",
				FrameAnimation_DESC("FullHealthUI.png", 0, 0, 0.1f, false));
			Health[i]->CreateFrameAnimationCutTexture("BreakHealth",
				FrameAnimation_DESC("BreakHealthUI.png", 0, 5, 0.1f, false));
			Health[i]->CreateFrameAnimationCutTexture("FillHealth",
				FrameAnimation_DESC("HUD Cln_appear_v020000-Sheet.png", 0, 4, 0.1f, false));

			Health[i]->ChangeFrameAnimation("FullHealth");
			Health[i]->ScaleToCutTexture(0);
			Health[i]->GetTransform().SetLocalPosition({ -720.0f + i * 74.0f, 420.0f, 1 });
			Health[i]->ChangeCamera(CAMERAORDER::UICAMERA);
		}
	}
	{
		for (int i = 0; i < 5; i++)
		{
			EmpthyHealth[i] = CreateComponent<GameEngineUIRenderer>();
			EmpthyHealth[i]->SetTexture("EmptyHealthUI.png");
			EmpthyHealth[i]->ScaleToTexture();
			EmpthyHealth[i]->GetTransform().SetLocalPosition({ -720.0f + i * 74.0f, 420.0f, 1 });
			EmpthyHealth[i]->ChangeCamera(CAMERAORDER::UICAMERA);
			EmpthyHealth[i]->Off();
		}
	}

	{
		Energy = CreateComponent<GameEngineMyRenderer>();
		Energy->CreateFrameAnimationCutTexture("IdleEnergy", MyFrameAnimation_DESC("HUD_Soulorb_fills_soul_idle0000-Sheet.png", 0, 5, 0.15f, true));
		Energy->CreateFrameAnimationCutTexture("FullEnergy", MyFrameAnimation_DESC("HUD Cln_soul_orb_glow0000.png", 0, 0, 0.1f, false));
		Energy->ChangeMaskFrameAnimation("IdleEnergy", "HUD Cln_soul_orb_shape.png");
		Energy->renderOption.IsMask = 1;
		Energy->renderOption.EnergyGage = 0.0;
		Energy->ScaleToCutTexture(0);
		Energy->GetTransform().SetLocalPosition({ -833, 405.0f, 1 });
		Energy->ChangeCamera(CAMERAORDER::UICAMERA);
	}

	{
		EnergyUIEffect = CreateComponent<GameEngineMyRenderer>();
		EnergyUIEffect->SetTexture("HUD Cln_soul_orb_darken.png");
		EnergyUIEffect->ScaleToTexture();
		EnergyUIEffect->GetTransform().SetLocalPosition({ -833, 405.0f, 1 });
		EnergyUIEffect->ChangeCamera(CAMERAORDER::UICAMERA);
		EnergyUIEffect->Off();

	}


}
 
void TopUI::Update(float _DeltaTime)
{
	//if (Player::GetMainPlayer()->PlayerEnergyGage > 0.7f)
	//{
	//	EnergyUIEffect->On();
	//}
	//else if (Player::GetMainPlayer()->PlayerEnergyGage >= 1.0f)
	//{
	//	EnergyUIEffect->Off();
	//}
	//else
	//{
	//	EnergyUIEffect->Off();
	//}

	HealthUpdate();
	EnergyUpdate();
	CoinUpdate();
}
void TopUI::CoinUpdate()
{

	if (CurCoin != Player::GetMainPlayer()->PlayerGeoCoin)
	{
		CurGeoCoin = std::to_string(Player::GetMainPlayer()->PlayerGeoCoin);
		CurCoin = Player::GetMainPlayer()->PlayerGeoCoin;
		GeoCoinFont->SetText(CurGeoCoin, "Noto Serif KR Black");
	}
}

void TopUI::LevelStartUpdate()
{
	{
		GeoCoinFont->SetText(CurGeoCoin, "Noto Serif KR Black");
		GeoCoinFont->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		GeoCoinFont->SetSize(40.0f);
		GeoCoinFont->SetScreenPostion({ 220, 115, 1 });
		GeoCoinFont->ChangeCamera(CAMERAORDER::UICAMERA);
	}
}
void TopUI::EnergyUpdate()
{
	Energy->renderOption.EnergyGage = Player::GetMainPlayer()->PlayerEnergyGage;
	if (Player::GetMainPlayer()->PlayerEnergyGage >= 1.0f)
	{
		Energy->ChangeMaskFrameAnimation("FullEnergy", "HUD Cln_soul_orb_shape.png");
		Energy->ScaleToTexture();
	}
	else
	{
		Energy->ChangeMaskFrameAnimation("IdleEnergy", "HUD Cln_soul_orb_shape.png");
		Energy->ScaleToCutTexture(0);
	}
}
void TopUI::HealthUpdate()
{
	AniTime -= 1.0f * GameEngineTime::GetDeltaTime();
	if (AniTime < 0.0f)
	{
		for (int i = 0; i < Player::GetMainPlayer()->PlayerHealth; i++)
		{
			Health[i]->CurAnimationReset();
			AniTime = 10.0f;
		}
	}

	
	if (CurHealth > Player::GetMainPlayer()->PlayerHealth)
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

	if (CurHealth < Player::GetMainPlayer()->PlayerHealth)
	{

		Health[Player::GetMainPlayer()->PlayerHealth - 1]->On();
		Health[Player::GetMainPlayer()->PlayerHealth - 1]->ChangeFrameAnimation("FillHealth");
		Health[Player::GetMainPlayer()->PlayerHealth - 1]->ScaleToCutTexture(0);

		CurHealth = Player::GetMainPlayer()->PlayerHealth;
	}

	for (int i = Player::GetMainPlayer()->PlayerHealth - 1; i < Player::GetMainPlayer()->PlayerFullHealth; i++)
	{
		if (i < 0)
		{
			return;
		}
		Health[i]->AnimationBindEnd("BreakHealth", [=](const FrameAnimation_DESC& _Info)
		{
			Health[i]->Off();
			EmpthyHealth[i]->On();
		});
		Health[i]->AnimationBindEnd("FillHealth", [=](const FrameAnimation_DESC& _Info)
		{
			Health[i]->ChangeFrameAnimation("IdleHealth");
			Health[i]->ScaleToCutTexture(0);
			EmpthyHealth[i]->Off();
		});
	}


}

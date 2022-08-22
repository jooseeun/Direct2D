#include "TopUI.h"
#include "PreCompile.h"

TopUI* TopUI::MainTopUI = nullptr;

TopUI::TopUI() 
	:EnergyUI(nullptr)
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
		for (int i = 0; i < 5; i++)
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
}

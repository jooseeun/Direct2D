#include "ThornTrap.h"
#include "PreCompile.h"

ThornTrap::ThornTrap()
	:ThornTrapCol(nullptr)
	,ThornTrapRenderer(nullptr)
{
}

ThornTrap::~ThornTrap() 
{
}

void ThornTrap::Start()
{
	ThornTrapRenderer = CreateComponent<GameEngineTextureRenderer>();
	ThornTrapRenderer->SetOrder((int)OBJECTORDER::Player);
	ThornTrapRenderer->SetPivot(PIVOTMODE::CENTER);
	ThornTrapRenderer->SetTexture("spikes_still.png");
	ThornTrapRenderer->ScaleToTexture();

	ThornTrapCol = CreateComponent<GameEngineCollision>();
	ThornTrapCol->GetTransform().SetLocalScale({ 139,105,1000.0f });
	ThornTrapCol->ChangeOrder((int)(OBJECTORDER::TrapObject));
	ThornTrapCol->SetDebugSetting(CollisionType::CT_AABB, { 1.0,0,0,1 });

}
void ThornTrap::Update(float _DeltaTime)
{
	
}
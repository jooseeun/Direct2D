#include "FallenGround.h"

FallenGround::FallenGround() 
	: Trigger(nullptr)
	, GroundRenderer1(nullptr)
	, GroundRenderer2(nullptr)
	, GroundCol1(nullptr)
	, GroundCol2(nullptr)
{
}

FallenGround::~FallenGround() 
{
}

void FallenGround::Start()
{
	GroundRenderer1 = CreateComponent<GameEngineTextureRenderer>();
	GroundRenderer1->SetOrder((int)OBJECTORDER::Player);
	GroundRenderer1->SetPivot(PIVOTMODE::BOT);
	GroundRenderer1->GetTransform().SetLocalPosition({ -250,0,0 });
	GroundRenderer1->SetTexture("FallenGround.png");
	GroundRenderer1->ScaleToTexture();
	GroundRenderer1->GetTransform().PixLocalNegativeX();

	GroundRenderer2 = CreateComponent<GameEngineTextureRenderer>();
	GroundRenderer2->SetOrder((int)OBJECTORDER::Player);
	GroundRenderer2->SetPivot(PIVOTMODE::BOT);
	GroundRenderer2->GetTransform().SetLocalPosition({ 250,0,0 });
	GroundRenderer2->SetTexture("FallenGround.png");
	GroundRenderer2->ScaleToTexture();

}
void FallenGround::Update(float _DeltaTime)
{


}
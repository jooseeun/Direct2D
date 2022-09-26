#include "SmallBreakDoor.h"
#include "PreCompile.h"

SmallBreakDoor::SmallBreakDoor() 
	:DoorRenderer(nullptr)
	,DoorCollision(nullptr)
{
}

SmallBreakDoor::~SmallBreakDoor() 
{
}

void SmallBreakDoor::Start()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });

	{
		DoorRenderer = CreateComponent< GameEngineTextureRenderer>();
		DoorRenderer->SetTexture("tut_door_02.png");
		DoorRenderer->ScaleToTexture();
		DoorRenderer->SetPivot(PIVOTMODE::BOT);
	}
	{
		DoorCollision = CreateComponent<GameEngineCollision>();
		DoorCollision->GetTransform().SetLocalScale({ 95,252,1000.0f });
		DoorCollision->ChangeOrder((int)(OBJECTORDER::StopObject));
	}
}

void SmallBreakDoor::Update(float _DeltaTime)
{
	DoorCollision->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Skill, CollisionType::CT_OBB2D,
		std::bind(&SmallBreakDoor::CheckDemage, this, std::placeholders::_1, std::placeholders::_2)
	);
}

CollisionReturn SmallBreakDoor::CheckDemage(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	Death();

	return CollisionReturn::Break;
}
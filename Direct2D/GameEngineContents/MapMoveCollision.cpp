#include "MapMoveCollision.h"
#include "PreCompile.h"
MapMoveCollision::MapMoveCollision() 
	: MoveCol1(nullptr)
	, MoveCol2(nullptr)
	, MoveCol3(nullptr)
	, MoveCol4(nullptr)
	, MoveLevel1()
	, MoveLevel2()
	, MoveLevel3()
	, MoveLevel4()
{
}

MapMoveCollision::~MapMoveCollision() 
{
}

void MapMoveCollision::Start()
{
	MoveCol1 = CreateComponent<GameEngineCollision>();
	MoveCol2 = CreateComponent<GameEngineCollision>();
	MoveCol3 = CreateComponent<GameEngineCollision>();
	MoveCol4 = CreateComponent<GameEngineCollision>();
}

void MapMoveCollision::Update(float _DeltaTime)
{

	MoveCol1->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&MapMoveCollision::Move1Level, this, std::placeholders::_1, std::placeholders::_2)
	);

	MoveCol2->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&MapMoveCollision::Move2Level, this, std::placeholders::_1, std::placeholders::_2)
	);

	MoveCol3->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&MapMoveCollision::Move3Level, this, std::placeholders::_1, std::placeholders::_2)
	);

	MoveCol4->IsCollision(CollisionType::CT_OBB2D, OBJECTORDER::Player, CollisionType::CT_OBB2D,
		std::bind(&MapMoveCollision::Move4Level, this, std::placeholders::_1, std::placeholders::_2)
	);
}


bool MapMoveCollision::Move1Level(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	GEngine::ChangeLevel(MoveLevel1);
	return true;
}
bool MapMoveCollision::Move2Level(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	GEngine::ChangeLevel(MoveLevel2);
	return true;
}
bool MapMoveCollision::Move3Level(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	GEngine::ChangeLevel(MoveLevel3);
	return true;
}
bool MapMoveCollision::Move4Level(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	GEngine::ChangeLevel(MoveLevel4);
	return true;
}
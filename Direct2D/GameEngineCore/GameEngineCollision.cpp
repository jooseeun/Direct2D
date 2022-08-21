#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineCoreDebug.h"

bool (*GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_MAX)][static_cast<int>(CollisionType::CT_MAX)])(const GameEngineTransform& _Left, const GameEngineTransform& _Right);

class GameEngineCollisionFunctionInit
{
public:
	GameEngineCollisionFunctionInit() 
	{
		memset(GameEngineCollision::CollisionFunction, 0, sizeof(GameEngineCollision::CollisionFunction));

		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_SPHERE)][static_cast<int>(CollisionType::CT_SPHERE)] = &GameEngineTransform::SphereToSphere;
		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_AABB)][static_cast<int>(CollisionType::CT_AABB)] = &GameEngineTransform::AABBToAABB;
		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_OBB)][static_cast<int>(CollisionType::CT_OBB)] = &GameEngineTransform::OBBToOBB;

		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_SPHERE2D)][static_cast<int>(CollisionType::CT_SPHERE2D)] = &GameEngineTransform::Sphere2DToSphere2D;
		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_AABB2D)][static_cast<int>(CollisionType::CT_AABB2D)] = &GameEngineTransform::AABB2DToAABB2D;
		GameEngineCollision::CollisionFunction[static_cast<int>(CollisionType::CT_OBB2D)][static_cast<int>(CollisionType::CT_OBB2D)] = &GameEngineTransform::OBB2DToOBB2D;
	}

	~GameEngineCollisionFunctionInit() 
	{

	}
};

GameEngineCollisionFunctionInit Inst;

GameEngineCollision::GameEngineCollision() 
	: DebugType(CollisionType::CT_SPHERE)
	, Color(1.0f, 0.0f, 0.0f, 0.5f)
{
}

GameEngineCollision::~GameEngineCollision() 
{
}


void GameEngineCollision::Start()
{
	GetActor()->GetLevel()->PushCollision(this, GetOrder());
}

void GameEngineCollision::ChangeOrder(int _Order)
{
	GetActor()->GetLevel()->PushCollision(this, _Order);
}

bool GameEngineCollision::IsCollision(CollisionType _ThisType, int _GroupOrder
	, CollisionType _OtherType
	, std::function<bool(GameEngineCollision* _This, GameEngineCollision* _Other)> _Function /*= nullptr*/)
{
	if (false == IsUpdate())
	{
		return false;
	}

	int ThisType = static_cast<int>(_ThisType);
	int OtherType = static_cast<int>(_OtherType);

	if (nullptr == GameEngineCollision::CollisionFunction[ThisType][OtherType])
	{
		MsgBoxAssert("���� ��������� ���� �ݸ��� ó���Դϴ�");
	}

	std::map<int, std::list<GameEngineCollision*>>& AllCollisions = GetActor()->GetLevel()->AllCollisions;

	std::list<GameEngineCollision*>& Collisions = AllCollisions[_GroupOrder];

	for (GameEngineCollision* Collision : Collisions)
	{
		if (false == Collision->IsUpdate())
		{
			continue;
		}

		if (true == GameEngineCollision::CollisionFunction[ThisType][OtherType](GetTransform(), Collision->GetTransform()))
		{
			if (nullptr != _Function)
			{
				// �־���� �Ѵٸ� ����ϴ� �̴ϴ�.
				if (true == _Function(this, Collision))
				{
					return true;
				}
			}
			else {
				return true;
			}
			// return true; �̺κ� �߸��ƾ��.
		}
	}

	return false;
}

void GameEngineCollision::DebugRender()
{
	switch (DebugType)
	{
	case CollisionType::CT_POINT2D:
		break;
	case CollisionType::CT_SPHERE2D:
		GameEngineDebug::DrawSphere(GetTransform(), Color);
		break;
	case CollisionType::CT_AABB2D:
		GameEngineDebug::DrawBox(GetTransform(), Color);
		break;
	case CollisionType::CT_OBB2D:
		GameEngineDebug::DrawBox(GetTransform(), Color);
		break;
	case CollisionType::CT_POINT:
		break;
	case CollisionType::CT_SPHERE:
		GameEngineDebug::DrawSphere(GetTransform(), Color);
		break;
	case CollisionType::CT_AABB:
		GameEngineDebug::DrawBox(GetTransform(), Color);
		break;
	case CollisionType::CT_OBB:
		GameEngineDebug::DrawBox(GetTransform(), Color);
		break;
	case CollisionType::CT_MAX:
		break;
	default:
		break;
	}

}
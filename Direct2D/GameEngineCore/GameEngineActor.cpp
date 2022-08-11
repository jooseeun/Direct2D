#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

GameEngineActor::GameEngineActor()
	: ParentLevel(nullptr)
	, IsLevelOver(false)
{

}

GameEngineActor::~GameEngineActor()
{
}

void GameEngineActor::Start() {}
void GameEngineActor::Update(float _DeltaTime) {}
void GameEngineActor::End() {}

void GameEngineActor::DetachObject()
{
	if (nullptr == GetParent())
	{
		MsgBoxAssert("부모가 존재하지 않는 오브젝트는 떼어낼 수 없습니다");
	}

	GameEngineUpdateObject::DetachObject();

	GetTransform().DetachTransform();

	GetLevel()->PushActor(this, this->GetOrder());
}

void GameEngineActor::SetParent(GameEngineUpdateObject* _Object)
{
	if (nullptr == GetParent())
	{
		GetLevel()->RemoveActor(this);
	}

	GameEngineUpdateObject::SetParent(_Object);

	{
		GameEngineTransformBase* Actor = nullptr;
		if (Actor = dynamic_cast<GameEngineTransformBase*>(_Object))
		{
			GetTransform().SetParentTransform(Actor->GetTransform());
			return;
		}
	}

	MsgBoxAssert("트랜스폼이 없는 컴포넌트에 트랜스폼이 있는 부모를 붙이려고 했습니다.");
}

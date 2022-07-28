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

void GameEngineActor::AllUpdate(float _ScaleDeltaTime, float _DeltaTime)
{
	AddAccTime(_DeltaTime);
	ReleaseUpdate(_DeltaTime);
	Update(_ScaleDeltaTime);

	for (GameEngineUpdateObject* Com : Childs)
	{
		Com->AddAccTime(_DeltaTime);
		Com->ReleaseUpdate(_DeltaTime);
		if (false == Com->IsUpdate())
		{
			continue;
		}

		Com->Update(_ScaleDeltaTime);
	}
}
void GameEngineActor::DetachObject()
{
	GameEngineUpdateObject::DetachObject();

	GetTransform().DetachTransform();
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

